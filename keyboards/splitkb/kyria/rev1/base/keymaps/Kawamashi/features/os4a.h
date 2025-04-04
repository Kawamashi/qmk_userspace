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

extern uint8_t os4a_layer;
//extern bool exit_os4a_layer;
//extern bool pending_OSL;

void os4a_layer_off(uint8_t layer);
void os4a_layer_on(uint8_t layer);

void os4a_tap(uint16_t keycode);

bool process_os4a_keys(uint16_t keycode, keyrecord_t *record);

void update_osl(uint16_t keycode);

bool process_os4a_layers(uint16_t keycode, keyrecord_t *record);

void mouse_mods_key_up(uint16_t keycode, keyrecord_t *record);

bool process_mods(uint16_t keycode, keyrecord_t *record);

void os4a_layer_exit_check(void);

#ifdef __cplusplus
}
#endif
