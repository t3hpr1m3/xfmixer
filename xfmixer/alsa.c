#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "alsa.h"

alsa_device_info **alsa_device_list = NULL;
int              alsa_device_count = 0;

int alsa_load_devices();
int alsa_load_mixers();
void alsa_alloc_device_info(alsa_device_info **device_info);
void alsa_alloc_mixer_info(alsa_mixer_info **mixer_info);
void alsa_free_device_info(alsa_device_info *device_info);
void alsa_free_mixer_info(alsa_mixer_info *mixer_info);

int alsa_init() {
	if (alsa_load_devices() == 0) {
		return alsa_load_mixers();
	}
	return 0;
}

void alsa_cleanup() {
	int i, j;
	for (i = 0; i < alsa_device_count; i++) {
		alsa_free_device_info(alsa_device_list[i]);
	}
	free(alsa_device_list);
	alsa_device_list = NULL;
	snd_config_update_free_global();
}

int alsa_get_card_count() {
	return alsa_device_count;
}

int alsa_get_mixer_count(int card_idx) {
	if (card_idx < 0 || card_idx >= alsa_device_count)
		return -1;
	return alsa_device_list[card_idx]->mixer_count;
}

const char* alsa_get_card_name(int card_idx) {
	if (card_idx < 0 || card_idx >= alsa_device_count)
		return NULL;

	if (alsa_device_list[card_idx]->info == NULL)
		return NULL;

	return snd_ctl_card_info_get_id(alsa_device_list[card_idx]->info);
}

const char* alsa_get_mixer_name(int card_idx, int mixer_idx) {
	if (card_idx < 0 || card_idx >= alsa_device_count)
		return NULL;

	if (alsa_device_list[card_idx]->mixers == NULL)
		return NULL;

	if (mixer_idx < 0 || mixer_idx >= alsa_device_list[card_idx]->mixer_count)
		return NULL;

	if (alsa_device_list[card_idx]->mixers[mixer_idx]->sid == NULL)
		return NULL;

	return snd_mixer_selem_id_get_name(alsa_device_list[card_idx]->mixers[mixer_idx]->sid);
}

int alsa_load_devices() {
	int err, dev;
	char name[32];
	int card = -1;
	alsa_device_info *device_info;

	if (snd_card_next(&card) < 0 || card < 0) {
		printf("No cards found...\n");
		return -1;
	}

	while (card >= 0) {
		alsa_alloc_device_info(&device_info);
		device_info->card_num = card;
		sprintf(name, "hw:%d", card);
		if ((err = snd_ctl_open(&device_info->ctl, name, 0)) < 0) {
			alsa_free_device_info(device_info);
			printf("control open (%d): %s\n", card, snd_strerror(err));
			return -1;
		}
		if ((err = snd_hctl_open_ctl(&device_info->hctl, device_info->ctl)) < 0) {
			alsa_free_device_info(device_info);
			printf("hcontrol open (%d): %s\n", card, snd_strerror(err));
			return -1;
		}
		if ((err = snd_ctl_card_info(device_info->ctl, device_info->info)) < 0) {
			alsa_free_device_info(device_info);
			printf("control hardware info (%d): %s\n", card, snd_strerror(err));
			return -1;
		}

		alsa_device_list = (alsa_device_info**)realloc(alsa_device_list,
				(alsa_device_count + 1) * sizeof(alsa_device_info*));
		alsa_device_list[alsa_device_count] = device_info;
		alsa_device_count++;

		if (snd_card_next(&card) < 0) {
			break;
		}
	}
	return 0;
}

int alsa_load_mixers() {
	int err;
	alsa_device_info *dev;
	alsa_mixer_info *mixer_info;
	snd_mixer_elem_t *elem;
	int i;
	for (i = 0; i < alsa_device_count; i++) {
		dev = alsa_device_list[i];
		if ((err = snd_mixer_open(&(dev->mixer_handle), 0)) < 0) {
			printf("Mixer %d open error: %s\n", dev->card_num, snd_strerror(err));
			return -1;
		}
		if ((err = snd_mixer_attach_hctl(dev->mixer_handle, dev->hctl)) < 0) {
			snd_mixer_close(dev->mixer_handle);
			printf("Mixer %d attach error: %s\n", dev->card_num, snd_strerror(err));
			return -1;
		}
		if ((err = snd_mixer_selem_register(dev->mixer_handle, NULL, NULL)) < 0) {
			snd_mixer_close(dev->mixer_handle);
			printf("Mixer %d register error: %s\n", dev->card_num, snd_strerror(err));
			return -1;
		}
		if ((err = snd_mixer_load(dev->mixer_handle)) < 0) {
			snd_mixer_close(dev->mixer_handle);
			printf("Mixer %d load error: %s\n", dev->card_num, snd_strerror(err));
			return -1;
		}
		for (elem = snd_mixer_first_elem(dev->mixer_handle); elem; elem = snd_mixer_elem_next(elem)) {
			if (!snd_mixer_selem_is_active(elem))
				continue;
			alsa_alloc_mixer_info(&mixer_info);
			snd_mixer_selem_get_id(elem, mixer_info->sid);
			dev->mixers = (alsa_mixer_info**)realloc(dev->mixers,
					(dev->mixer_count + 1) * sizeof(alsa_mixer_info*));
			dev->mixers[dev->mixer_count] = mixer_info;
			dev->mixer_count++;
		}
	}
	return 0;
}

void alsa_alloc_device_info(alsa_device_info **device_info) {
	*device_info = (alsa_device_info*)malloc(sizeof(alsa_device_info));
	(*device_info)->ctl = NULL;
	(*device_info)->info = NULL;
	(*device_info)->hctl = NULL;
	(*device_info)->mixers = NULL;
	(*device_info)->mixer_handle = NULL;
	(*device_info)->mixer_count = 0;
	snd_ctl_card_info_malloc(&((*device_info)->info));
}

void alsa_alloc_mixer_info(alsa_mixer_info **mixer_info) {
	*mixer_info = (alsa_mixer_info*)malloc(sizeof(alsa_mixer_info));
	(*mixer_info)->sid = NULL;
	snd_mixer_selem_id_malloc(&((*mixer_info)->sid));
}

void alsa_free_device_info(alsa_device_info *device_info) {
	if (device_info->mixer_count > 0) {
		int i;
		for (i = 0; i < device_info->mixer_count; i++) {
			alsa_free_mixer_info(device_info->mixers[i]);
		}
		free(device_info->mixers);
	}
	if (device_info->mixer_handle != NULL) {
		snd_mixer_detach_hctl(device_info->mixer_handle, device_info->hctl);
		snd_mixer_close(device_info->mixer_handle);
	}
	if (device_info->info != NULL)
		snd_ctl_card_info_free(device_info->info);
	if (device_info->hctl)
		snd_hctl_close(device_info->hctl);
	else if (device_info->ctl)
		snd_ctl_close(device_info->ctl);
	free(device_info);
}

void alsa_free_mixer_info(alsa_mixer_info *mixer_info) {
	if (mixer_info->sid != NULL)
		snd_mixer_selem_id_free(mixer_info->sid);
	free(mixer_info);
}
