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

bool on_left_hand(keypos_t pos);

bool same_side_combination(const keyrecord_t* tap_hold_record, const keyrecord_t* other_record);

void tap_converter(uint16_t keycode, keyrecord_t *record);

bool process_custom_tap_hold(uint16_t keycode, keyrecord_t *record);

#ifdef __cplusplus
}
#endif
