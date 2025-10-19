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

#include "word_selection.h"


static bool sel_word = false;

bool is_select_word(void) {
  return sel_word;
}

void end_select_word(void) {
  clear_weak_mods();
  sel_word = false;
}

bool process_select_word(uint16_t keycode, keyrecord_t *record) {

  switch (keycode) {
    case SEL_WORD:
        if (record->event.pressed) {    // Handling of other macros (on press).
            if (!sel_word) {
                tap_code16(C(KC_RIGHT));
                tap_code16(C(KC_LEFT));
                tap_code16(RCS(KC_RIGHT));
                sel_word = true;
            } else {
                end_select_word();
            }
        }
        return false;
    
    case SEL_LINE:
        if (record->event.pressed) {    // Handling of other macros (on press).
            if (!sel_word) {
                tap_code(KC_HOME);
                tap_code16(S(KC_END));
                sel_word = true;
            } else {
                end_select_word();
            }
        }
        return false;
  }

  if (!sel_word) { return true;}

  clear_weak_mods();

  if (!record->event.pressed) { return true; }

  switch (keycode) {
    case KC_LEFT:
    case KC_RIGHT:
        set_weak_mods(MOD_BIT_LCTRL);

    case KC_DOWN:
    case KC_UP:
    case C(KC_LEFT):
    case C(KC_RGHT):
        add_weak_mods(MOD_BIT_LSHIFT);
        return true;
  }

  end_select_word();
  return true; // Process all other keycodes normally
}
