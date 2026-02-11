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

#include "word_conf.h"

bool is_letter(uint16_t keycode) {

  // Taking 1DK layer into account
  if (IS_LAYER_ON(_1DK)) {
    switch (keycode) {

      case PG_VIRG:
        return true;
  
      case PG_EGRV:    // arrobe
      case PG_V:
      case PG_M:
      case PG_C:
      case PG_J:
      case PG_X:
      case PG_G:    // greek dead key
      case PG_T:
      case PG_R:    // pour le trait d’union insécable
      case PG_L:
      case PG_D:    // pour le tréma
      case PG_W:
        return false;
    }
  }

  switch (keycode) {
    case KC_A ... KC_F:
    case KC_H ... KC_P:
    case KC_R ... KC_S:
    case KC_U ... KC_Z:
    case PG_L:
    case PG_E:
    case KC_GRV ... KC_DOT:
    case PG_AGRV:
    case PG_EGRV:
    case PG_ECIR:
    case LETTER_1DK:
      return true;

    default:
      return false;
  }
}

bool is_send_string_macro(uint16_t keycode) {
  switch (keycode) {
    case OU_GRV:
    case N_TILD:
    case MAGIC:
    //case PG_AROB:   // because of Clever Keys
      return true;
    
    default:
      return false;
  }
}

bool is_followed_by_apos(uint16_t keycode, uint16_t prev_keycode) {
  
  switch (keycode) {
    case PG_L:
    case PG_T:
    case PG_D:
    case PG_C:
    case PG_N:
    case PG_S:
    case PG_M:
    case PG_Y:
    case PG_J:
      if (is_letter(prev_keycode)) { return false; }
    case PG_Q:
      return true;

    default:
      return false;
  }
}


// Caps Word

bool caps_word_press_user(uint16_t keycode) {

  if (IS_LAYER_ON(_1DK)) {
    switch (keycode) {  
      case PG_D:    // pour le tréma
      case PG_R:    // pour le trait d’union insécable
      //case PG_APOS:
        return true;
    }
  }

  // Keycodes that continue Caps Word, with shift applied.
  // @ must be shifted, bc of CleverKeys using it.
  if (is_letter(keycode) || is_send_string_macro(keycode)) {
    add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
    return true;
  } 

  switch (keycode) {
    // Keycodes that continue Caps Word, without shifting.
    case PG_1DK:
    case PG_UNDS:
    case PG_TIRE:
    case PG_SLSH:
    case KC_1 ... KC_0:
    case KC_BSPC:
    case LCTL(KC_BSPC):
    case KC_DEL:
    case PG_APOS:
      return true;

    default:
      return false;  // Deactivate Caps Word.
    }
}


bool should_continue_caps_list(uint16_t keycode, keyrecord_t* record) {

    // Keycodes that continue Caps List, but not Caps Word.
    // These keycodes trigger the counter to deactivate Caps List.
    switch (keycode) {
      case KC_BSPC:
        return update_capslist_counter(-1);
      case PG_VIRG:
      case KC_SPC:
          return update_capslist_counter(1);
    }

    if (is_letter(keycode) || is_send_string_macro(keycode)) { return update_capslist_counter(1); }

    // This condition can't be merged with the previous one
    // because caps_word_press_user adds shift to letters and send-string macros.
    if (caps_word_press_user(keycode)) { return update_capslist_counter(1); }

    return false;  // Deactivate Caps List.
}


bool list_separator(void) {

    // Words that continue Caps List.
    if (get_recent_keycode(-1) == KC_SPC) {

        if (get_recent_keycode(-2) == PG_VIRG) { return true; }

        if (word_check((uint16_t[]) {KC_SPC, PG_E, PG_T}, 3, 2)) { return true; }

        if (word_check((uint16_t[]) {KC_SPC, PG_O, PG_U}, 3, 2)) { return true; }

        if (word_check((uint16_t[]) {PG_X, PG_C, PG_G}, 3, 2)) { return true; }
    }
    return false;
}

void word_selection_press_user(uint16_t keycode) {

  switch (keycode) {
    case KC_LEFT:
    case C(KC_LEFT):
        select_word(-1);
        set_weak_mods(MOD_BIT_LCTRL | MOD_BIT_LSHIFT);
        break;

    case KC_RIGHT:
    case C(KC_RGHT):
        select_word(1);
        set_weak_mods(MOD_BIT_LCTRL | MOD_BIT_LSHIFT);
        break;

    case KC_DOWN:
        select_line(1);
        add_weak_mods(MOD_BIT_LSHIFT);
        break;

    case KC_UP:
        select_line(-1);
        add_weak_mods(MOD_BIT_LSHIFT);
        break;

    case KC_HOME:
    case KC_END:
        add_weak_mods(MOD_BIT_LSHIFT);
        break;

    case NAVWORD:
        break;

    default:
      disable_modword(selectword);
  }
}


// Layer Word

uint8_t layerword_layer_from_trigger(uint16_t keycode) {

  switch (keycode) {
    case NUMWORD: return _NUMBERS;
    case NAVWORD: return _SHORTNAV;
    case FUNWORD: return _FUNCTIONS;
    default: return 0;
  }
}

uint16_t layerword_exit_timeout(uint8_t layer) {

  switch (layer) {
    case _NUMBERS:
    case _SHORTNAV:
        return 3000;
    case _FUNCTIONS:
        return 30000;
    default:
        return 0;
  }
}

bool should_continue_layerword(uint8_t layer, uint16_t keycode, keyrecord_t *record) {

  switch (layer) {

    case _NUMBERS:
      switch (keycode) {
        // Keycodes that should not disable num word.
        // Numpad keycodes
        case KC_1 ... KC_0:
        case KC_PDOT:
        case PG_MOIN:
        case PG_ASTX: 
        case PG_PLUS:
        case PG_SLSH:
        case PG_EGAL:
        case PG_EXP:
        case PG_IND:
        case PG_H:
        case PG_2PTS:
        //case NNB_SPC:

        // Misc
        case KC_BSPC:
        case PG_1DK:   // Not to exit Numword when chording it with 1DK
        //case NUMWORD:   // For the combo NUMWORD to work
            return true; 
        default:
            return false;
      }

    case _SHORTNAV:
      switch (keycode) {
        case SEL_WORD:
        case SEL_LINE:
          return true;
      }
      keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);
      switch (keycode) {
        case KC_LEFT:
        case KC_RIGHT:
        case KC_DOWN:
        case KC_UP:
        case KC_PGUP:
        case KC_PGDN:
        case KC_HOME:
        case KC_END:
            return true;
        default:
            return false;
      }

    case _FUNCTIONS:
      switch (keycode) {
        case KC_F8:
            return true;
        default:
            disable_layerword(_FUNCTIONS);
            return false;
      }
  }
  return false;
}