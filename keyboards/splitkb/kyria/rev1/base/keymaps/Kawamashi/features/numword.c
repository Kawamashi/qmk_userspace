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

#include "numword.h"

//static uint16_t num_word_timer = 0;
//static bool is_num_word_on = false;
bool is_num_word_on = false;
bool exit_num_word = false;

bool is_num_word_enabled(void) {
    return is_num_word_on;
}

void enable_num_word(void) {
    //if (is_num_word_on) return;
    is_num_word_on = true;
    layer_on(_NUMBERS);
}

void disable_num_word(void) {
    //if (!is_num_word_on) return;
    is_num_word_on = false;
    layer_off(_NUMBERS);
    exit_num_word = false;
}

void toggle_num_word(void) {
    if (is_num_word_on) {
        disable_num_word();
    }
    else {
        enable_num_word();
    }
}

bool should_terminate_num_word(uint16_t keycode, const keyrecord_t *record) {

    switch (keycode) {
        // Keycodes which should not disable num word mode.

        // Numpad keycodes
         case KC_1 ... KC_0:
         case KC_PDOT:
         case FG_POIN:
         //case FG_VIRG:
         case FG_MOIN:
         case FG_ASTX: 
         case FG_PLUS:
         case FG_SLSH:
         case FG_ACIR:
         case FG_CARN:

        // Misc
        case KC_BSPC:
        case FG_TYPO:   // Not to exit Numword when chording it with Typo
        case NUMWORD:   // For the combo NUMWORD to work

/*         
        case FG_EGAL:
        case FG_BSLS:*/
            return false;
    }

    return true;
}


bool process_numword(uint16_t keycode, const keyrecord_t *record) {
    // Handle the custom keycodes that go with this feature
    if (keycode == NUMWORD) {
        if (record->event.pressed) {
            toggle_num_word();
            return false;
        }
    }

    // Other than the custom keycodes, nothing else in this feature will activate
    // if the behavior is not on, so allow QMK to handle the event as usual.
    if (!is_num_word_on) { return true; }

    // Get the base keycode of a mod or layer tap key
    switch (keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
            // Earlier return if this has not been considered tapped yet
            if (record->tap.count == 0) { return true; }
            keycode = keycode & 0xFF;
            break;
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:  // `LT(layer, key)` keys.
            // Release event on a held layer-tap key when numword is on.
            if (record->tap.count == 0 && !record->event.pressed && is_num_word_on) {     
                return false;  // Skip default handling so that layer stays on.
            } else {
                keycode = keycode & 0xFF;    // Get tapping keycode.
            }
            break;
/*         default:
            break; */
    }

    exit_num_word = should_terminate_num_word(keycode, record);
    return true;
}
