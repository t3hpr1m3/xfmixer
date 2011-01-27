#include <alsa/asoundlib.h>
#include <alsa/control.h>

typedef struct {
	snd_mixer_selem_id_t    *sid;
} alsa_mixer_info;

typedef struct {
	int                     card_num;
	snd_ctl_t               *handle;
	snd_hctl_t				*hctl;
	snd_ctl_card_info_t     *info;
	snd_mixer_t             *mixer_handle;
	alsa_mixer_info         **mixers;
	int                     mixer_count;
} alsa_device_info;

extern alsa_device_info **alsa_device_list;
extern int              alsa_device_count;

int alsa_init();
void alsa_cleanup();
