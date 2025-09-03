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
    case KC_A ... KC_F:
    case KC_H ... KC_N:
    case KC_R ... KC_Z:
    case PG_L:
    case PG_X:
    case PG_E:
    //case PG_AGR:
    //case PG_ECIR:
    case KC_GRV ... KC_DOT:
      return true;

    default:
      return false;
  }
}

bool isSendStringMacro(uint16_t keycode) {
  switch (keycode) {
    //case AGRV_SPC:
    //case CA_CED:
/*     case L_APOS:
    case D_APOS: */
    case OU_GRV:
    //case J_APOS:
    //case PG_BL:
    case MAGIC:
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

  // Caps Word shouldn't be applied with Alt-gr
  // Managing underscore and slash on alt gr + E/T.
  // Underscore and slash must continue Caps Word, without shifting.
  if ((get_mods() & MOD_BIT(KC_ALGR))) {
    switch (keycode) {
      case PG_E:
      case PG_T:
        return true;
      default:
        return false;
    }
  }

  if (IS_LAYER_ON(_ODK)) {
    switch (keycode) {
      case PG_EACU:
      case PG_B:
        add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
        return true;
      case PG_I:
      case PG_F:
      case PG_T:
        return true;
      case PG_L:
      case PG_H:
      case PG_VIRG:
      case PG_V:
      case PG_M:
      case PG_C:
      //case PG_T:
      case PG_S:
        return false;
    }
  }

  // Keycodes that continue Caps Word, with shift applied.
  // @ must be shifted, bc of CleverKeys using it.
  if (isLetter(keycode) || isSendStringMacro(keycode) || keycode == PG_AROB) {
    add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
    return true;
  } 

  switch (keycode) {
    // Keycodes that continue Caps Word, without shifting.
    case PG_ODK:
    //case PG_GRV:
    case PG_UNDS:
    case PG_MOIN:
    case KC_KP_1 ... KC_KP_0:
    case KC_LEFT:
    case KC_RIGHT:
    case KC_BSPC:
    case KC_DEL:
    case PG_APOS:
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
    //case TG_APOD:
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
  uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
  //if (keycode == OS_ODK && (mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_ALGR)))) { return true;}
  if (keycode == OS_ODK && (mods & ~MOD_BIT(KC_ALGR))) { return true;}

  switch (keycode) {
    //case OS_ODK:  /!\ A ne pas remettre, sous peine de ne pas pouvoir faire shift + typo + touche de l'autre côté
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
    //case PG_ODK:
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
    case C(PG_Z):
      return C(PG_Y);
    case C(PG_Y):
      return C(PG_Z);
  }
/*   if ((get_mods() | get_weak_mods()) & MOD_BIT(KC_ALGR)) {
    return KC_SPC;
  }  */

  if (recent[RECENT_SIZE - 1] != KC_NO) { return MAGIC; }

/*   keycode = tap_hold_extractor(keycode);
  if (isLetter(keycode)) { return MAGIC; }

  switch (keycode) {
  case PG_APOS:
  case KC_SPC:
  case 

  } */

  return KC_TRNS;  // Defer to default definitions.
}