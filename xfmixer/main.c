#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libxfcegui4/libxfcegui4.h>

#ifdef HAVE_DBUS
#include <dbus/dbus.h>
#endif

#include <alsa/asoundlib.h>
#include <alsa/control.h>

int main(int argc, char **argv) {
	GtkWidget	*window;
	GError		*error = NULL;

	//g_set_application_name(_("XfMixer"));
	//gtk_init(&argc, &argv);

	/*
	if (G_UNLIKELY(!xfconf_init(&error))) {
		if (G_LIKELY(error != NULL)) {
			g_print(_("Failed to initialize xfconf: %s"), error->message);
			g_error_free(error);
		}
	}

	gtk_window_set_default_icon_name("multimedia-volume-control");
	*/

	void **hints, **n;
	char *name;
	const char *filter;
	if (snd_device_name_hint(-1, "pcm", &hints) >= 0) {
		n = hints;
		filter = "Output";
		while (*n != NULL) {
			name = snd_device_name_get_hint(*n, "NAME");
			if (name != NULL) {
				printf("%s\n", name);
				free(name);
			}
			n++;
		}
		snd_device_name_free_hint(hints);
	}
	printf("Hello World!\n");
	return 0;
}
