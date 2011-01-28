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

	for (i = 0; i < alsa_get_card_count(); i++) {
		printf("card: %d - %s\n", i, alsa_get_card_name(i));

		for (j = 0; j < alsa_get_mixer_count(i); j++) {
			printf("mixer(%d): %s\n", i, alsa_get_mixer_name(i, j));
		}
	}


	alsa_cleanup();
	return 0;
}
