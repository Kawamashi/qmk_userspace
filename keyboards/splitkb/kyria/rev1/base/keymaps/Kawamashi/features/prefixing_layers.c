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

static bool previous_1dk = false;

bool process_prefixing_layers(uint16_t keycode, keyrecord_t *record) {

    if (!record->event.pressed) { 
        if (previous_1dk && get_repeat_key_count() > 0) {
            clear_oneshot_layer_state(ONESHOT_PRESSED); 
            previous_1dk = false;
        }
        return true; 
    }

    // OS4A keys behave like one-shot shifts for the opposite side of the keyboard
    if (IS_LAYER_ON(_L_MODS) || IS_LAYER_ON(_R_MODS)) {
        if (should_add_shift(keycode, record)) {
            set_oneshot_mods(MOD_BIT(KC_LSFT));
            if (!is_letter(keycode)) { set_last_keycode(S(keycode)); }
            disable_layerword(get_layerword_layer());  // To correct a bug producing Capsword when typing OS4A + roll between a letter and apostrophe.
        }
    }

    if (previous_1dk) {
        if (get_repeat_key_count() > 0) { 
            set_oneshot_layer(_1DK, ONESHOT_START);
        } else {
            previous_1dk = false;
        }
    }

    // Handling keys and layers related to the One Dead Key (1DK)
    switch (keycode) {
        case PG_1DK:
        case NUM_1DK:
          return deferred_shift_after_dead_key(keycode);
    }

    if (IS_LAYER_ON(_1DK)) {
        previous_1dk = true;

        switch (keycode) {
            case PG_K:
            case PG_B:
            case PG_H:
            case PG_APOS:
            case OU_GRV:
            case PG_UNDS:
            case PG_AGRV:
            case PG_ECIR:
            //case KC_SPC:    // When space is added by Clever Keys
            case CNL_1DK:
              return true;

            case PG_U:
                if (get_recent_keycode(-1) == PG_Q) { return true; }
                
            default:
              return deferred_shift_after_dead_key(keycode);
        }
    }
    return true;
}


bool deferred_shift_after_dead_key(uint16_t keycode) {
    // Special behaviour of PG_1DK when shifted
    // Shift must apply to the keycode following PG_1DK.
    const bool is_shifted = (get_mods() | get_weak_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;

    if (is_shifted) {
        del_weak_mods(MOD_MASK_SHIFT);
        del_oneshot_mods(MOD_MASK_SHIFT);
        unregister_mods(MOD_MASK_SHIFT);
    }

    tap_code(PG_1DK);
    if (is_shifted) { set_oneshot_mods(MOD_BIT(KC_LSFT)); }    // Don't use weak mods !

    return keycode != PG_1DK;
}


bool should_add_shift(uint16_t keycode, keyrecord_t *record) {

  // Shift shouldn't be added if other mods are active
  if (get_mods() | get_oneshot_mods()) { return false; }

  // Combos and encoder events.
  if (!IS_KEYEVENT(record->event)) { return true; }

  // Specific exceptions
  if (not_to_be_shifted(keycode)) { return false; }

  // Otherwise, add shift if the key is on the other side of the keyboard.
  //return (layerword_layer == _R_MODS) == on_left_hand(record->event.key);
  return IS_LAYER_ON(_R_MODS) == on_left_hand(record->event.key);
}