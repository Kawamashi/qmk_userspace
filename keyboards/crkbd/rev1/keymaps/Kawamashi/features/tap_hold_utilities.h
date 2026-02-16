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

/**
 * Returns true if the args come from keys on opposite hands.
 *
 * @param tap_hold_record keyrecord_t from the tap-hold key's event.
 * @param other_record keyrecord_t from the other key's event.
 * @return True if the keys are on opposite hands.
 */
bool bilateral_combination(const keyrecord_t* tap_hold_record, const keyrecord_t* other_record);

/**
 * Optional callback to customize which key chords are considered "held".
 *
 * This callback is called if while `tap_hold_keycode` is pressed,
 * `other_keycode` is pressed. Return true if the tap-hold key should be
 * considered held, or false to consider it tapped.
 *
 * @param tap_hold_keycode Keycode of the tap-hold key.
 * @param tap_hold_record keyrecord_t from the tap-hold press event.
 * @param other_keycode Keycode of the other key.
 * @param other_record keyrecord_t from the other key's press event.
 * @return True if the tap-hold key should be considered held.
 */
bool approved_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record);

// Handles the tap function of tap-hold keys using non-basic keycodes
bool process_custom_tap_hold(uint16_t keycode, keyrecord_t *record);

#ifdef __cplusplus
}
#endif
