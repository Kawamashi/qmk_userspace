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

 #include "typo_layer.h"
 //#include "print.h"

 oneshot_state os_typo_state = os_up_unqueued;
 static bool is_shifted = false;
 //static uint16_t typo_keycode = KC_NO;
//bool exit_typo = false;

bool update_oneshot_typo(oneshot_state *state, uint16_t keycode, keyrecord_t *record) {

    const uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();

    if (keycode == OS_TYPO) {
        // Handle the custom OSL that go with this feature
        // It's timerless, to avoid problems when rolling with an other key, when shift is on.
        if (record->event.pressed) {    // On press
            // Custom behaviour when alt-gr
            if (mods & MOD_BIT(KC_ALGR)) {
                tap_code16(ALGR(FG_TYPO));
                return false;
            }
            is_shifted = mods & MOD_MASK_SHIFT;
            if (is_shifted) {
                del_weak_mods(MOD_MASK_SHIFT);
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);
            }
            layer_on(_TYPO);
            //*state = os_down_unused;
            *state = os_up_queued;
            return false;
        } else {    // On release
            return true;
        }

    //} else if (keycode == CNL_TYPO) {
        

    } else if (keycode == FG_TYPO) {
        // Special behaviour of FR_TYPO when shifted
        // Shift must apply to the next keycode
/*             is_shifted = mods & MOD_MASK_SHIFT;
        if (is_shifted) {
            del_weak_mods(MOD_MASK_SHIFT);
            del_oneshot_mods(MOD_MASK_SHIFT);
            unregister_mods(MOD_MASK_SHIFT);
        } */
        //tap_code(FR_TYPO);
        //*state = os_up_queued;
        return true;

    } else {
        if (record->event.pressed) {
            //uint8_t key_layer = read_source_layers_cache(record->event.key);
            //if (key_layer == _TYPO) {
            if (IS_LAYER_ON(_TYPO)) {
                // On non-ignored keyup, consider the oneshot used.
                switch (*state) {
/*                     case os_down_unused:
                    *state = os_down_used;
                    dprintf("key down, layer: %d, os_down_unused -> os_down_used\n", _TYPO);
                    return true; */
                case os_up_queued:
                    *state = os_up_queued_used;
                    dprintf("key down, layer: %d, os_up_queued -> os_up_queued_used\n", _TYPO);
                    return true;
                case os_up_queued_used:
                    *state = os_up_unqueued;
                    layer_off(_TYPO);
                    dprintf("key down (off), layer: %d, os_up_queued_used -> os_up_unqueued\n", _TYPO);
                    //return false;
                default:
                    break;
                }
            }
        } else {
            // Ignore key ups from other layers
            //uint8_t key_layer = read_source_layers_cache(record->event.key);
            //if (key_layer == _TYPO) {
            if (IS_LAYER_ON(_TYPO)) {
                // On non-ignored keyup, consider the oneshot used.
                switch (*state) {
                case os_up_queued:
                    *state = os_up_unqueued;
                    layer_off(_TYPO);
                    dprintf("key up (off), layer: %d, os_up_queued -> os_up_unqueued\n", _TYPO);
                    return true;
                case os_up_queued_used:
                    *state = os_up_unqueued;
                    layer_off(_TYPO);
                    dprintf("key up (off), layer: %d, os_up_queued_used -> os_up_unqueued\n", _TYPO);
                    return true;
                default:
                    break;
                }
            }
        }
    }
    return true;
}

bool process_typo_layer(uint16_t keycode, keyrecord_t *record) {

    //if (keycode == FG_TYPO) { return true; }

    if (!update_oneshot_typo(&os_typo_state, keycode, record)) { return false; }


    if (IS_LAYER_ON(_TYPO)) {
        if (record->event.pressed) {    // On press            

            switch (keycode) {
                case FG_AROB:
                case FG_K:
                case FG_J:
                case OU_GRV:
                case FG_CCED:
                case AGRV_SPC:
                case KC_SPC:    // When space is added by Clever Keys
                case CNL_TYPO:
                    break;
          
                default:
/*                     is_shifted = mods & MOD_MASK_SHIFT;
                    if (is_shifted) {
                        del_weak_mods(MOD_MASK_SHIFT);
                        del_oneshot_mods(MOD_MASK_SHIFT);
                        unregister_mods(MOD_MASK_SHIFT);
                    } */
                    tap_code(FG_TYPO);
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
            //exit_typo = true;
        }
    }
    return true;
}

/* void typo_layer_off(void) {
    layer_off(_TYPO);
    exit_typo = false;
} */