/*
 * Copyright (c) 2011 Josh Williams <theprime@codingprime.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libxfcegui4/libxfcegui4.h>

#ifdef HAVE_DBUS
#include <dbus/dbus.h>
#endif

#include "alsa.h"
#include "xfmixer-window.h"

int main(int argc, char **argv) {
	GtkWidget	*window;
	GError		*error = NULL;
	int i, j;

	if (alsa_init() == -1) {
		alsa_cleanup();
		return 1;		
	}

	gtk_init(&argc, &argv);
	window = xfmixer_window_new();

	for (i = 0; i < alsa_get_card_count(); i++) {
		printf("card: %d - %s\n", i, alsa_get_card_name(i));

		for (j = 0; j < alsa_get_mixer_count(i); j++) {
			printf("mixer(%d): %s\n", i, alsa_get_mixer_name(i, j));
		}
	}

	gtk_widget_show(window);

	gtk_main();

	alsa_cleanup();

	exit(0);
}
