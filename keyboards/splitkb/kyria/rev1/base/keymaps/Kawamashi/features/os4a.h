/* Copyright 2025 @Kawamashi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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
 

#pragma once

#include "quantum.h"
#include "keymap.h"

#ifdef __cplusplus
extern "C" {
#endif

void os4a_task(void);

uint8_t get_os4a_layer(void);

void os4a_layer_off(uint8_t layer);
void os4a_layer_on(uint8_t layer);

void os4a_tap(uint16_t keycode);
bool process_os4a_keys(uint16_t keycode, keyrecord_t *record);

bool should_add_shift(uint16_t keycode, keyrecord_t *record);

void mouse_mods_key_up(uint16_t keycode, keyrecord_t *record);

bool process_os4a(uint16_t keycode, keyrecord_t *record);

uint8_t os4a_layer_from_trigger(uint16_t keycode);
bool should_stay_os4a_layer(uint16_t keycode);
bool not_to_be_shifted(uint16_t keycode);
//bool mods_for_mouse(uint16_t keycode);

#ifdef __cplusplus
}
#endif
