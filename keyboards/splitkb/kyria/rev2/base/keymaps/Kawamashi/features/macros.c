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

#include "macros.h"

static bool is_apos_dr = false;
static bool sel_word = false;

bool replace_apos(void) {
  return is_apos_dr;
}

bool is_select_word(void) {
  return sel_word;
}

void end_select_word(void) {
  clear_mods();
  sel_word = false;
}

bool process_macros(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {    // Handling of other macros (on press).
        switch (keycode) {

            case SEL_WORD:
                if (!sel_word) {
                    register_mods(MOD_BIT_LCTRL);
                    tap_code(KC_RIGHT);
                    tap_code(KC_LEFT);
                    register_mods(MOD_BIT_LSHIFT);
                    //tap_code(KC_LEFT);
                    tap_code(KC_RIGHT);
                    sel_word = true;
                } else {
                    end_select_word();
                }
                return false;

            case RAZ:
                if (is_caps_lock_on()) { tap_code(KC_CAPS); }
                if (!host_keyboard_led_state().num_lock) { tap_code(KC_NUM_LOCK); }

                layer_clear();
                clear_mods();
                //clear_oneshot_mods();
                //clear_weak_mods();
                caps_word_off();
                disable_num_word();
                clear_recent_keys();
                break;

            case TG_APOS:
                is_apos_dr = !is_apos_dr;
                return false;

            case PG_DEG:
                tap_code(PG_ODK);
                tap_code(KC_9);
                return false;
        }
    }
    return true; // Process all other keycodes normally
}
