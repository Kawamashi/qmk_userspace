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

bool isLetter(uint16_t keycode);
bool isSendStringMacro(uint16_t keycode);
bool is_caps_lock_on(void);
uint16_t tap_hold_extractor(uint16_t keycode);

uint8_t get_os4a_layer(uint16_t keycode);
bool os4a_layer_changer(uint16_t keycode);
bool to_be_shifted(uint16_t keycode, keyrecord_t *record);
//bool mods_for_mouse(uint16_t keycode);