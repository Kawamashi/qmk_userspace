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

// Returns true if `pos` on the left hand of the keyboard, false if right.
bool on_left_hand(keypos_t pos);

// Handles the tap function of tap-hold keys using non-basic keycodes
bool process_custom_tap_hold(uint16_t keycode, keyrecord_t *record);

// Checks if the event is on a home-row MT with a valid matrix position.
bool is_hrm_event(uint16_t keycode, keyrecord_t* record, keypos_t pos);

// Disables the hold behaviour of HRM during typing.
void process_flow_tap(uint16_t keycode, keyrecord_t* record);

#ifdef __cplusplus
}
#endif
