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
#ifndef __ALSA_H__
#define __ALSA_H__

int alsa_init();
void alsa_cleanup();
int alsa_get_card_count();
int alsa_get_mixer_count(int card_idx);
const char* alsa_get_card_name(int card_idx);
const char* alsa_get_mixer_name(int card_idx, int mixer_idx);

#endif /* !__ALSA_H__ */
