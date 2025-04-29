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

 #include "odk_layer.h"

//static uint16_t odk_keycode = KC_NO;

bool process_odk_layer(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {    // On press

        const uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
        static bool is_shifted = false;

        if (keycode == OS_ODK) {
            // Handle the custom OSL that go with this feature
            // It's timerless, to avoid problems when rolling with an other key, when shift is on.
            // Custom behaviour when alt-gr
            if (mods & MOD_BIT(KC_ALGR)) {
                tap_code16(ALGR(FG_ODK));
                return false;
            }
            is_shifted = mods & MOD_MASK_SHIFT;
            if (is_shifted) {
                del_weak_mods(MOD_MASK_SHIFT);
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);
            }
            /*layer_on(_ODK);
            odk_keycode = KC_NO;
            return false; */

        } else if (keycode == FG_ODK) {
            // Special behaviour of FR_ODK when shifted
            // Shift must apply to the next keycode
/*             is_shifted = mods & MOD_MASK_SHIFT;
            if (is_shifted) {
                del_weak_mods(MOD_MASK_SHIFT);
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);
            } */
            //tap_code(FR_ODK);
            return true;

        } else if (IS_LAYER_ON(_ODK)) {
            //if (odk_keycode == KC_NO) { odk_keycode = keycode; }
                //if (!IS_QK_USER(keycode)) { odk_keycode = keycode; }

            switch (keycode) {
                case FG_AROB:
                case FG_K:
                case FG_J:
                case FG_ECIR:
                case OU_GRV:
                case FG_CCED:
                //case CA_CED:
                //case AGRV_SPC:
                case KC_SPC:    // When space is added by Clever Keys
                case FG_3PTS:
                case CNL_ODK:
                    break;
          
                default:
/*                     is_shifted = mods & MOD_MASK_SHIFT;
                    if (is_shifted) {
                        del_weak_mods(MOD_MASK_SHIFT);
                        del_oneshot_mods(MOD_MASK_SHIFT);
                        unregister_mods(MOD_MASK_SHIFT);
                    } */
                    tap_code(FG_ODK);
            }
            if (!IS_LAYER_ON(_APOS_DR)) {
                switch (keycode) {
                    case FG_M:
                    case FG_L:
                        is_shifted = true;
                }
            }
            if (is_shifted) {
                //set_mods(mods);
                set_oneshot_mods(MOD_BIT(KC_LSFT));
                is_shifted = false;
            }
            //exit_odk = true;
/*         } else {
            odk_keycode = KC_NO; */
        }
/*     } else {    // On release
        switch (keycode) {
            case OS_ODK:
            case FG_ODK:
                break;
            default:
                //if (exit_odk) { odk_layer_exit_check(); }
                if (keycode == odk_keycode) {
                    layer_off(_ODK);
                    odk_keycode = KC_NO;
                }
        } */
    }
    return true;
}

/* void odk_layer_exit_check(uint16_t keycode) {
    if (keycode == odk_keycode) {
        layer_off(_ODK);
        odk_keycode = KC_NO;
    }
} */