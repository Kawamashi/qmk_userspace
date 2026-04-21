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

#include "tap_hold_utilities.h"


bool on_left_hand(keypos_t pos) {
#ifdef SPLIT_KEYBOARD
  return pos.row < MATRIX_ROWS / 2;
#else
  return (MATRIX_COLS > MATRIX_ROWS) ? pos.col < MATRIX_COLS / 2
                                     : pos.row < MATRIX_ROWS / 2;
#endif
}


bool process_custom_tap_hold(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {    // On press
      tap_code16(keycode);
      return false;
  }
  return true;
}


static uint8_t is_tapped[(MATRIX_ROWS * MATRIX_COLS + 7) / 8] = {0};

/* static uint16_t get_tap_keycode(uint16_t keycode) {
  switch (keycode) {
    case QK_MOD_TAP ... QK_MOD_TAP_MAX:
      return QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
  }
  return keycode;
} */

void process_tap_flow(uint16_t keycode, keyrecord_t* record) {
  const keypos_t pos = record->event.key;

  if (IS_KEYEVENT(record->event) && pos.row < MATRIX_ROWS && pos.col < MATRIX_COLS &&
      IS_QK_MOD_TAP(keycode)) {
    // The event is on an MT with a valid matrix position.
    const uint16_t tap_keycode = tap_hold_extractor(keycode);

    // Determine the key's index in the bit arrays.
    const uint16_t index = pos.row * MATRIX_COLS + pos.col;
    const uint16_t array_index = index / 8;
    const uint8_t bit_mask = UINT8_C(1) << (index % 8);

    if (record->event.pressed) {  // On press.

      if (get_idle_time() < TAP_INTERVAL) {
        // Rewrite the event as a press of the tap keycode. This way, it
        // bypasses the usual action_tapping logic.
        record->keycode = tap_keycode;
        // Record this key as tapped.
        is_tapped[array_index] |= bit_mask;
      }

    } else if ((is_tapped[array_index] & bit_mask) != 0) {  // On tap release.

      // Rewrite the event as a release of the tap keycode.
      record->keycode = tap_keycode;
      // Record the key as released.
      is_tapped[array_index] &= ~bit_mask;
    }
  }
}