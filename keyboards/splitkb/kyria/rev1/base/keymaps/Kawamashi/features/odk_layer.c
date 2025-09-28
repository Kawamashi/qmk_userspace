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

bool process_odk_layer(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {    // On press

        const uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
        //bool mod_odk = false;
        //const uint8_t mods = get_mods() | get_oneshot_mods();

        if (keycode == OS_ODK) {
            // Custom behaviour when alt-gr
            if (mods & MOD_BIT(KC_ALGR)) {
                tap_code16(ALGR(PG_ODK));
                return false;
            }
            return true;

        } else if (keycode == PG_ODK) {
            //mod_odk = true;
            return true;

        } else if (IS_LAYER_ON(_ODK)) {
            switch (keycode) {
                case PG_PVIR:
                case PG_AROB:
                case PG_K:
                case PG_B:
                case PG_APOS:
                case OU_GRV:
                case KC_SPC:    // When space is added by Clever Keys
                case CNL_ODK:
                case PG_ODK:
                    return true;
        
                default:
                    mod_odk();
                    //mod_odk = true;
                    //process_odk_layer(MOD_ODK, record);
                    // Don't use tap_code, it doesn't go through process_record.
                    // therefore it doesn't trigger the special behaviour of PG_ODK described above
                    //invoke_key(PG_ODK, record);
            }
        }

/*         if (mod_odk) {
            // Special behaviour of PG_ODK when shifted
            // Shift must apply to the next keycode
            mod_odk = false;
            bool is_shifted = false;

            if (mods & MOD_MASK_SHIFT) {
                del_weak_mods(MOD_MASK_SHIFT);
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);
                is_shifted = true;
            }

            tap_code(PG_ODK);

            if (is_shifted) {
                set_oneshot_mods(MOD_BIT(KC_LSFT));     // Don't use weak mods !
                is_shifted = false;
            }
            if (keycode == MOD_ODK) { return false; }
        } */
    }
    return true;
}

void mod_odk(void) {
    // Special behaviour of PG_ODK when shifted
    // Shift must apply to the next keycode
    bool is_shifted = false;
    const uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();

    if (mods & MOD_MASK_SHIFT) {
        del_weak_mods(MOD_MASK_SHIFT);
        del_oneshot_mods(MOD_MASK_SHIFT);
        unregister_mods(MOD_MASK_SHIFT);
        is_shifted = true;
    }

    tap_code(PG_ODK);

    if (is_shifted) {
        set_oneshot_mods(MOD_BIT(KC_LSFT));     // Don't use weak mods !
        is_shifted = false;
    }
}