
int alsa_init();
void alsa_cleanup();
int alsa_get_card_count();
int alsa_get_mixer_count(int card_idx);
const char* alsa_get_card_name(int card_idx);
const char* alsa_get_mixer_name(int card_idx, int mixer_idx);
