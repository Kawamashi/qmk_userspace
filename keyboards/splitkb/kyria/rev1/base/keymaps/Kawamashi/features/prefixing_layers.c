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

#include "prefixing_layers.h"

// Keep track of the 1DK, for the Repeat Key
static bool previous_1dk = false;

bool process_prefixing_layers(uint16_t keycode, keyrecord_t *record) {

    if (!record->event.pressed) { return true; }    // Nothing special happens on release

    if (previous_1dk) {
        if (get_repeat_key_count() > 0) { tap_code(PG_1DK); }
        previous_1dk = false;
    }

    // Handling keys and layers related to the One Dead Key (1DK)
    switch (keycode) {
        case PG_1DK:
          return insert_1dk(keycode);
    }

    if (IS_LAYER_ON(_1DK)) {
        //previous_1dk = true;

        switch (keycode) {
            case PG_K:
            case PG_B:
            case PG_H:
            case PG_Z:
            case OU_GRV:
            case PG_UNDS:
            case PG_AGRV:
            case PG_ECIR:
            //case KC_SPC:    // When space is added by Clever Keys
            case CNL_1DK:
              return true;

            case LT_APOS:
                if (record->tap.count) { return true; }
                return insert_1dk(keycode);

            case PG_U:
                if (get_recent_keycode(-1) == PG_Q) { return true; }
                
            default:
              return insert_1dk(keycode);
        }
    }
    return true;
}


bool insert_1dk(uint16_t keycode) {

    // Special behaviour of PG_1DK when shifted
    // Shift must apply to the keycode following PG_1DK.
    const bool shift_mods = get_mods() & MOD_MASK_SHIFT;
    const bool shift_weak_mods = get_weak_mods() & MOD_MASK_SHIFT;
    const bool shift_oneshot_mods = get_oneshot_mods() & MOD_MASK_SHIFT;
    
    if (shift_oneshot_mods) { del_oneshot_mods(MOD_MASK_SHIFT); }
    if (shift_mods) { del_mods(MOD_BIT(KC_LSFT)); }
    if (shift_weak_mods) { del_weak_mods(MOD_MASK_SHIFT); }

    previous_1dk = true;
    tap_code(PG_1DK);
    
    if (shift_oneshot_mods) { set_oneshot_mods(MOD_BIT(KC_LSFT)); }    // Don't use weak mods !
    if (shift_mods) { add_mods(MOD_BIT(KC_LSFT)); }
    if (shift_weak_mods) { add_weak_mods(MOD_BIT(KC_LSFT)); }

    return keycode != PG_1DK;
}