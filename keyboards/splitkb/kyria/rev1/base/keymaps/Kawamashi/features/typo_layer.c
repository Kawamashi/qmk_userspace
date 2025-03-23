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


//bool is_typo = false;
bool exit_typo = false;

void typo_layer_off() {
    layer_off(_TYPO);
    exit_typo = false;
}

bool process_typo(uint16_t keycode, keyrecord_t *record) {


    return true;
}

/* bool process_typo(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {
        switch (keycode) {
            case OS_TYPO:
                // Handle the custom OSL that go with this feature
                // It's timerless, to avoid problems when rolling with an other key, when shift is on.
                //is_typo = true;
                if ((get_mods() | get_weak_mods() | get_oneshot_mods()) & MOD_BIT(KC_ALGR)) {
                    tap_code16(ALGR(FG_TYPO));
                } else {
                    layer_on(_TYPO);
                }                
                return false;

             case FG_TYPO:
                const bool is_shifted = (get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
                if (is_shifted) {
                    del_weak_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    unregister_mods(MOD_MASK_SHIFT);
                }
                tap_code(FG_TYPO);
                //set_mods(mods);
                if (is_shifted) {
                    set_oneshot_mods(MOD_BIT(KC_LSFT));
                    //is_shifted = false;
                } 
         }

        if (IS_LAYER_ON(_TYPO)) {
            switch (keycode) {
                case TG_TYPO:
                case FG_AROB:
                case FG_K:
                case FG_J:
                case OU_GRV:
                  break;
          
                default:
                    //tap_code(FG_TYPO);
                  const bool is_shifted = (get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
                  if (is_shifted) {
                      del_weak_mods(MOD_MASK_SHIFT);
                      del_oneshot_mods(MOD_MASK_SHIFT);
                      unregister_mods(MOD_MASK_SHIFT);
                  }
                  tap_code(FG_TYPO);
                  if (is_shifted) { set_oneshot_mods(MOD_BIT(KC_LSFT)); }
            }
            exit_typo = true;
        }
    }
    return true;
}  */



/* bool process_typo(uint16_t keycode, const keyrecord_t *record) {
    // Handle the custom keycodes that go with this feature
    if (keycode == OS_TYPO) {
        if (record->event.pressed) {
            is_typo = true;
            layer_on(_TYPO);
            return false;
        }
    }

    // Other than the custom keycode, nothing else in this feature will activate
    // if the behavior is not on, so allow QMK to handle the event as usual.
    if (!is_typo) { return true; }

    switch (keycode) {
        case TG_TYPO :
        case FG_AROB:
        case FG_K:
        case FG_J:
        case OU_GRV:
          break;
  
        default:
          const bool is_shifted = (get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
          if (is_shifted) {
              del_weak_mods(MOD_MASK_SHIFT);
              del_oneshot_mods(MOD_MASK_SHIFT);
              unregister_mods(MOD_MASK_SHIFT);
          }
          tap_code(FG_TYPO);
          if (is_shifted) { set_oneshot_mods(MOD_BIT(KC_LSFT)); }
      }

    exit_typo = true;
    return true;
} */