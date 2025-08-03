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

bool is_apos_dr = false;

bool process_macros(uint16_t keycode, keyrecord_t *record) {
    //const uint8_t mods = get_mods();
    //static bool is_shifted = false;

    if (record->event.pressed) {    // Handling of other macros (on press).
        switch (keycode) {
/*             case ALT_TAB:
                return process_swapper(KC_TAB);
            case REV_TAB:
                return process_swapper(S(KC_TAB)); */

            case AIDE_MEM:                    
                switch(get_highest_layer(layer_state|default_layer_state)) {
                    case _BASE:
                        tap_code(KC_F13);
                        return false;
/*                     case _SYMBOLS:
                        tap_code(KC_F14); */
                        return false;
                    case _SHORTNAV:
                        tap_code(KC_F15);
                        return false;
                    case _FUNCAPPS:
                        tap_code(KC_F16);
                        return false;
                }

            case RAZ:
                //led_t led_usb_state = host_keyboard_led_state();
                if (is_caps_lock_on()) { tap_code(KC_CAPS); }
                if (!host_keyboard_led_state().num_lock) { tap_code(KC_NUM_LOCK); }

                layer_clear();
                clear_oneshot_mods();
                caps_word_off();
                disable_num_word();
                clear_recent_keys();
                break;

            case KC_CAPS:
                caps_word_off();
                return true;

            case CAPSWORD:
                // I can't use CW_TOGG because QMK dosn't reach process_record_user when processing it.
                // Deactivating Caps Lock when Caps Word activates.
                if (is_caps_lock_on()) { tap_code(KC_CAPS); }
                caps_word_toggle();
                return false;

            case TG_APOS:
                is_apos_dr = !is_apos_dr;
                return false;

            case NUM_ODK:
                bool is_shifted = (get_mods() | get_weak_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
                if (is_shifted) {
                    del_weak_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    unregister_mods(MOD_MASK_SHIFT);
                }
                tap_code(PG_ODK);

                if (is_shifted) {
                    is_shifted = false;
                    set_oneshot_mods(MOD_BIT(KC_LSFT));
                }
                return true;
        }
    }
    return true; // Process all other keycodes normally
}
