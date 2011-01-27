#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libxfcegui4/libxfcegui4.h>

#ifdef HAVE_DBUS
#include <dbus/dbus.h>
#endif

#include "alsa.h"

int main(int argc, char **argv) {
	GtkWidget	*window;
	GError		*error = NULL;
	int i, j;

	if (alsa_init() == -1) {
		alsa_cleanup();
		return 1;		
	}

	for (i = 0; i < alsa_device_count; i++) {
		printf("card: %d - %s\n", alsa_device_list[i]->card_num, snd_ctl_card_info_get_id(alsa_device_list[i]->info));

		for (j = 0; j < alsa_device_list[i]->mixer_count; j++) {
			printf("mixer(%d): %s\n", alsa_device_list[i]->card_num, snd_mixer_selem_id_get_name(alsa_device_list[i]->mixers[j]->sid));
		}
	}


	alsa_cleanup();
	return 0;
}
