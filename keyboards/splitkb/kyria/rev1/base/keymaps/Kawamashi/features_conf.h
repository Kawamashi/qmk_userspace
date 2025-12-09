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

// Returns whether typographic apostrophe is to be replaced by typewritten apostrophe
bool replace_apos(void);

// This function extracts the base keycode of MT and LT,
// even if the tap/hold key is a custom one, with non-basic tap keycode.
uint16_t tap_hold_extractor(uint16_t keycode);

// Macros to be executed at the beginning of process_record_user :
// Layer-tap Repeat and Magic keys
bool process_macros_I(uint16_t keycode, keyrecord_t *record);

// Other macros, to be executed at the end of process_record_user
bool process_macros_II(uint16_t keycode, keyrecord_t *record);