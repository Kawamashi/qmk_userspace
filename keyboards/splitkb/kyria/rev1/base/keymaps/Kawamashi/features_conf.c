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

#include "features_conf.h"

bool is_caps_lock_on(void) { return host_keyboard_led_state().caps_lock; }

bool isLetter(uint16_t keycode) {
  switch (keycode) {
    case KC_A ... KC_N:
    case KC_Q ... KC_V:
    case KC_X ... KC_Z:
    case FG_U:
    case FG_E:
    case KC_GRV ... KC_DOT:
      return true;

    default:
      return false;
  }
}

// This function extracts the base keycode of MT and LT,
// even if the tap/hold key is a custom one, with non-basic tap keycode.
uint16_t tap_hold_extractor(uint16_t keycode) {
  switch (keycode) {
    default:
      return keycode &= 0xff;
  }
}


// Caps Word

bool caps_word_press_user(uint16_t keycode) {

  // Caps Word shouldn’t be applied with Alt-gr
  // Managing underscore on alt gr + E.
  // Underscore must continue Caps Word, without shifting.
  if ((get_mods() & MOD_BIT(KC_ALGR))) {
    switch (keycode) {
      case FG_E:
      case FG_T:
        return true;
      default:
        return false;
    }
  }

  if (IS_LAYER_ON(_TYPO)) {
    switch (keycode) {
      case FG_VIRG:
        add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
        return true;
      case FG_I:
      case FG_H:
        return true;
      case FG_U:
      case FG_T:
      case FG_G:
      case FG_B:
      case FG_F:
      case FG_M:
      case FG_L:
      case FG_S:
      case FG_N:
        return false;
    }
  }

  // 

  // Keycodes that continue Caps Word, with shift applied.
  if (isLetter(keycode)) {
    add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
    return true;
  } 

  switch (keycode) {
    // Keycodes that continue Caps Word, without shifting.
    case FG_TYPO:
    //case FG_GRV:
    case FG_MOIN:
    case KC_KP_1 ... KC_KP_0:
    case KC_LEFT:
    case KC_RIGHT:
    case KC_BSPC:
    case KC_DEL:
    case FG_APOS:
      return true;

    default:
      return false;  // Deactivate Caps Word.
    }
}

// One-shot 4 all configuration

uint8_t get_os4a_layer(uint16_t keycode) {
  switch (keycode) {
    case L_OS4A: return _L_MODS;
    case R_OS4A: return _R_MODS;
    default: return 0;
  }
}

bool os4a_layer_changer(uint16_t keycode) {
  switch (keycode) {
    case OS_FA:
    case NUMWORD:
    case TT_FA:
    case TG_APOD:
      return true;
    default:
      return false;
  }
}

bool to_be_shifted(uint16_t keycode, keyrecord_t *record) {
  // Combos and encoder events.
  if (!IS_KEYEVENT(record->event)) { return true; }
  
  switch (keycode) {
    case KC_CAPS:
    case CAPSWORD:
      return false;
    default:
      return (os4a_layer == _R_MODS) == on_left_hand(record->event.key);
  }
}


// Callum mods

bool is_oneshot_cancel_key(uint16_t keycode) {
  switch (keycode) {
    case L_OS4A:
    case R_OS4A:
        return true;
    default:
        return false;
  }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
  // Alt-gr et shift s'appliquent à la touche typo, pour permettre de faire les majuscules plus facilement ainsi que ] avec.
  // Autrement, la touche typo est ignorée par les Callum mods.
  // Ça permet de transmettre les mods à la touche suivante, par ex pour faire Ctrl + K. 
  //uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
  //if (keycode == OS_TYPO && (mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_ALGR)))) { return true;}
  //if (keycode == OS_TYPO && (mods & ~MOD_BIT(KC_ALGR))) { return true;}

  switch (keycode) {
    //case OS_TYPO:  /!\ A ne pas remettre, sous peine de ne pas pouvoir faire shift + typo + touche de l'autre côté
    case L_OS4A:
    case R_OS4A:
    case OS_SHFT:
    case OS_CTRL:
    case OS_RALT:
    case OS_LALT:
    case OS_WIN:
    case OS_FA:
    case NUMWORD:
    case TT_FA:
    case FG_TYPO:
        return true;
    default:
        return false;
  }
}


// Repeat and Magic keys

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record, uint8_t* remembered_mods) {
  switch (keycode) {
    case KC_BSPC:
    case LT_REPT:
    case LT_MGC:
      return false;
  }
  return true;
}

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {

  switch (keycode) {
    case C(FG_Z):
      return C(FG_Y);
    case C(FG_Y):
      return C(FG_Z);
  }

  keycode = tap_hold_extractor(keycode);
  if (isLetter(keycode)) { return MAGIC; }

  return KC_TRNS;  // Defer to default definitions.
}