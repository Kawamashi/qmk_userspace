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

bool is_letter(uint16_t keycode) {
  switch (keycode) {
    case KC_A ... KC_F:
    case KC_H ... KC_P:
    case KC_R ... KC_S:
    case KC_U ... KC_Z:
    case PG_L:
    case PG_E:
    case KC_GRV ... KC_DOT:
      return true;

    default:
      return false;
  }
}

bool is_send_string_macro(uint16_t keycode) {
  switch (keycode) {
    case OU_GRV:
    case MAGIC:
    //case PG_DEG:
      return true;
    
    default:
      return false;
  }
}

bool is_followed_by_apos(uint16_t keycode, uint16_t prev_keycode) {
  switch (keycode) {
    case PG_Q:
      return true;
      
    case PG_L:
    case PG_T:
    case PG_D:
    case PG_C:
    case PG_N:
    case PG_S:
    case PG_M:
    case PG_Y:
    case PG_J:
      if (!is_letter(prev_keycode)) { return true; }
  }
  return false;
}

// This function extracts the base keycode of MT and LT,
// even if the tap/hold key is a custom one, with non-basic tap keycode.
uint16_t tap_hold_extractor(uint16_t keycode) {
  switch (keycode) {
    case LT_NBSPC:
      return NNB_SPC;
    default:
      return keycode &= 0xff;
  }
}

bool process_custom_tap_hold(uint16_t keycode, keyrecord_t *record) {

  if (record->tap.count) {    // Handling of special tap-hold keys (on tap).
    switch (keycode) {

        case RCTL_T(FEN_B):
          return process_tap_hold(LWIN(KC_DOWN), record);

        case SFT_T(COPY):
          return process_tap_hold(C(PG_C), record);

        case LT_NBSPC:
          return process_tap_hold(NNB_SPC, record);

        case LT_REPT:
          repeat_key_invoke(&record->event);
          return false;

        case LT_MGC:
          alt_repeat_key_invoke(&record->event);
          return false;
    }
  }
  return true; // Process all other keycodes normally
}


// Caps Word

bool caps_word_press_user(uint16_t keycode) {

  // Caps Word shouldn't be applied with Alt-gr
  // Managing underscore and slash on alt gr + E/T.
  // Underscore and slash must continue Caps Word, without shifting.
/*   if ((get_mods() & MOD_BIT(KC_ALGR))) {
    switch (keycode) {
      case PG_E:
      case PG_T:
        return true;
      default:
        return false;
    }
  } */

  if (IS_LAYER_ON(_ODK)) {
    switch (keycode) {

      case PG_VIRG:
        add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
        return true;
  
      case PG_Y:
      case PG_T:
        return true;

      case PG_POIN:
        return false;
    }
  }

  // Keycodes that continue Caps Word, with shift applied.
  // @ must be shifted, bc of CleverKeys using it.
  if (is_letter(keycode) || is_send_string_macro(keycode) || keycode == PG_AROB) {
    add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
    return true;
  } 

  switch (keycode) {
    // Keycodes that continue Caps Word, without shifting.
    case PG_ODK:
    //case PG_GRV:
    case PG_UNDS:
    case PG_TIRE:
    case PG_SLSH:
    case KC_1 ... KC_0:
    //case KC_LEFT:
    //case KC_RIGHT:
    case KC_BSPC:
    case LCTL(KC_BSPC):
    case KC_DEL:
    case PG_APOS:
      return true;

    default:
      return false;  // Deactivate Caps Word.
    }
}


// Clever keys configuration

uint16_t get_ongoing_keycode_user(uint16_t keycode) {
  // Handles custom keycodes to be processed for Clever Keys

  if (is_send_string_macro(keycode)) { return keycode; }

  if (IS_LAYER_ON(_ODK)) {
    switch (keycode) {
      case PG_K:
      case PG_B:
      case PG_AROB:
      //case PG_3PTS:
      case KC_SPC:  // In order to uppercase J after '?' for ex.
        return keycode;

      case PG_POIN:
        return PG_3PTS;

      default:
        clear_recent_keys();
        return KC_NO;
    }
  }
  return KC_TRNS;
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
    case TG_FA:
    case OS_RSA:
    case NUM_ODK:
      return true;
    default:
      return false;
  }
}

bool to_be_shifted(uint16_t keycode, keyrecord_t *record) {
  // Combos and encoder events.
  if (!IS_KEYEVENT(record->event)) { return true; }
  
  switch (keycode) {
/*     case OS_ODK:
      is_shifted = true; */
    case KC_CAPS:
    case CAPSWORD:
    case CAPSLIST:
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

  const uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
  //if (keycode == OS_ODK && (mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_ALGR)))) { return true; }
  //if (keycode == OS_ODK && (mods & ~MOD_BIT(KC_ALGR))) { return true; }

  switch (keycode) {
    case OS_ODK:
      // On veut que la touche typo soit ignorée par tous les Callum mods sauf Alt-gr.
      // Ça permet de transmettre les mods à la touche suivante, par ex pour faire Ctrl + K. 
      // Alt-gr doit pouvoir s’appliquer à la touche typo, pour permettre de faire la touche morte "~" avec.
      // OS_ODK ne doit être ignored_key que lorsqu’elle est employée avec Alt-gr
      // sous peine de ne pas pouvoir faire shift + typo + touche de l'autre côté
      if (mods & ~MOD_BIT(KC_ALGR)) { return true; }
      break;
    case L_OS4A:
    case R_OS4A:
    case OS_SHFT:
    case OS_CTRL:
    case OS_RALT:
    case OS_LALT:
    case OS_WIN:
    case OS_FA:
    case NUMWORD:
    case TG_FA:
      return true;
/*     default:
      return false; */
  }
  return false;
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

  if (recent[RECENT_SIZE - 1] != KC_NO) { return MAGIC; }
  if (get_last_keycode() == KC_NO) { return MAGIC; }
  
  return KC_TRNS;  // Defer to default definitions.
}