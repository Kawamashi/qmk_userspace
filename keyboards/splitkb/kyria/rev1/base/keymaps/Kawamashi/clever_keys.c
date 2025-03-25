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

// Correction automatique

#include "clever_keys.h"


bool clever_key_finder(uint16_t next_keycode, keyrecord_t* record) {

  uint16_t prev_keycode = recent[RECENT_SIZE - 1];
  //const uint8_t mods = get_mods();
  //const bool isCaps = is_caps_lock_on() || is_caps_word_on();

  if (isLetter(next_keycode) || next_keycode == E_CIRC) {
    switch (prev_keycode) {
        case FG_EXLM:
        case FG_QUES:
        //case FG_3PTS:
        case FG_2PTS:
          // Add space between punctuation and letters.
          invoke_key(KC_SPC, record);

        case KC_SPC:
          switch (recent[RECENT_SIZE - 2]) {
            case FG_EXLM:
            case FG_QUES:
            //case FG_3PTS:
            case FG_POIN:
              // Add OS shift at the beginning of sentences.
              if (!is_caps_lock_on()) { set_oneshot_mods(MOD_BIT(KC_LSFT)); }
              break;
          }
    }
  }

  
  switch (prev_keycode) {

    case FG_Q:
      switch (next_keycode) {
        
        // Ajout automatique du "u" après le "q"
        case FG_E:
        case FG_I:
        case FG_A:
        case FG_O:
        case FG_EACU:
        case E_CIRC:
        case FG_APOS:
          invoke_key(FG_U, record);
          break;

        // Raccourci pour "quelq"
        case FG_Q:
          clear_recent_keys();  // To prevent infinite loop
          process_word((uint16_t[]) {FG_U, FG_E, FG_L}, 3, record);
          break;

        // Raccourci pour "quoi", ça évite un aller-retour sur la main gauche.
        case FG_Z:
          return finish_word((uint16_t[]) {FG_U, FG_O, FG_I}, 3, record);

        // Raccourci pour "quand"
        case FG_D:
          process_word((uint16_t[]) {FG_U, FG_A, FG_N}, 3, record);
          break;
      }
      break;

    case FG_P:
      switch (next_keycode) {

        case FG_M:
          // "pas"
          return finish_word((uint16_t[]) {FG_A, FG_S}, 2, record);
        
        case FG_APOS:
          // "par"
          return finish_word((uint16_t[]) {FG_A, FG_R}, 2, record);
        
        case FG_POIN:
          if (!isLetter(recent[RECENT_SIZE - 2])) {
            // "pour"
            return finish_word((uint16_t[]) {FG_O, FG_U, FG_R}, 3, record);
          }
          break;
      }
      break;
    
    case FG_CCED:
      if (!isLetter(next_keycode)) {
        invoke_key(FG_A, record);
      }
      break;
  }

  
  switch (next_keycode) {

    case FG_QUES:
    case FG_EXLM:
      // On ajoute un espace insécable s'il n'a pas été entré avant le point d'exclamation.
      // Il ne faut pas tester cette fonctionnalité avec Word, qui ajoute cet espace automatiquement.
      if (isLetter(recent[RECENT_SIZE - 1])) {
        
/*         if ((mods | get_oneshot_mods() | get_weak_mods()) & MOD_MASK_SHIFT) {
          del_weak_mods(MOD_MASK_SHIFT);
          del_oneshot_mods(MOD_MASK_SHIFT);
          unregister_mods(MOD_MASK_SHIFT);
        } */
        invoke_key(KC_SPC, record);
        //set_mods(mods);
        return replace_next_key(next_keycode, record);
      }
      break;

/*     case KC_SPC:
      if (get_last_keycode() == FG_TYPO) {
        invoke_key(FG_A, record);
      }
      break; */

    case MAGIC:
      switch (prev_keycode) {

        case FG_T:
          invoke_key(FG_I, record);
        case FG_I:
          return finish_magic((uint16_t[]) {FG_O, FG_N}, 2, record);
        
        case FG_C:
          return finish_magic((uint16_t[]) {FG_APOS, FG_E, FG_S, FG_T}, 4, record);
        
        case FG_D:
          return finish_magic((uint16_t[]) {FG_A, FG_N, FG_S}, 3, record);
        
        case FG_P:
          return finish_magic((uint16_t[]) {FG_L, FG_U, FG_S}, 3, record);

        case FG_O:
          return finish_magic((uint16_t[]) {FG_N, FG_T}, 2, record);
        
        case FG_A:
          if (isLetter(recent[RECENT_SIZE - 2])) {
            // "ant"
            return finish_magic((uint16_t[]) {FG_N, FG_T}, 2, record);
          } else {
            // "avec"
            return finish_magic((uint16_t[]) {FG_V, FG_E, FG_C}, 3, record);
          }
        
        case FG_S:
          return finish_magic((uint16_t[]) {FG_U, FG_R}, 2, record);
        
        case FG_B:
          process_word((uint16_t[]) {FG_O, FG_N, FG_J}, 3, record);
        
        case FG_J:
          return finish_magic((uint16_t[]) {FG_O, FG_U, FG_R}, 3, record);
        
        case FG_M:
          if (isLetter(recent[RECENT_SIZE - 2])) {
            // "ment"
            return finish_magic((uint16_t[]) {FG_E, FG_N, FG_T}, 3, record);
          } else {
            // "même"
            return finish_magic((uint16_t[]) {FG_TYPO, FG_O, FG_M, FG_E}, 4, record);
          }
        
        default:
          return false;
      }

    case FG_AROB:
      if (!isLetter(recent[RECENT_SIZE - 2])) {
        switch (prev_keycode) {

          case FG_T:
            // "t@" -> "toujours"
            layer_off(_TYPO);
            return finish_word((uint16_t[]) {FG_O, FG_U, FG_J, FG_O, FG_U, FG_R, FG_S}, 7, record);

          case FG_P:
            // "p@" -> "peut-être"
            layer_off(_TYPO);
            return finish_word((uint16_t[]) {FG_E, FG_U, FG_T, FG_MOIN, FG_TYPO, FG_O, FG_T, FG_R, FG_E}, 9, record);

          case FG_A:
            // "a@" -> "aujourd'hui"
            layer_off(_TYPO);
            return finish_word((uint16_t[]) {FG_U, FG_J, FG_O, FG_U, FG_R, FG_D, FG_APOS, FG_H, FG_U, FG_I}, 10, record);
          
          case FG_B:
            // "b@" -> "beaucoup"
            layer_off(_TYPO);
            return finish_word((uint16_t[]) {FG_E, FG_A, FG_U, FG_C, FG_O, FG_U, FG_P}, 7, record);
          
          case FG_E:
            // "e@" -> "est-ce qu"
            layer_off(_TYPO);
            return finish_word((uint16_t[]) {FG_S, FG_T, FG_MOIN, FG_C, FG_E, KC_SPC, FG_Q}, 7, record);
          
          case FG_D:
            // "d@" -> "déjà"
            layer_off(_TYPO);
            return finish_word((uint16_t[]) {FG_EACU, FG_J, FG_TYPO, FG_A}, 4, record);
        }          
      }
      break;

    case FG_Q:
      if (prev_keycode == FG_J) {
          // "jq" -> "jusqu"
          process_word((uint16_t[]) {FG_U, FG_S}, 2, record);
      }
      break;

    case FG_A:
      //if (prev_keycode == FG_O && !isCaps) {
      if (prev_keycode == FG_O) {
        // "oa" -> "oi"
        clear_recent_keys();  // To prevent infinite loop
        return replace_next_key(FG_I, record);
      }
      break;

    case FG_I:
      //if (prev_keycode == FG_O && !isCaps && recent[RECENT_SIZE - 3] != FG_Q) {
      if (prev_keycode == FG_O && recent[RECENT_SIZE - 3] != FG_Q) {
        // "oi" -> "oa", for "keyboard"
        clear_recent_keys();  // To prevent infinite loop
        return replace_next_key(FG_A, record);
      }
      break;

    case FG_O:
      if (prev_keycode == FG_U && recent[RECENT_SIZE - 2] != FG_Q) {
        // "uo" -> "un"
        return replace_next_key(FG_N, record);
      }
      break;
    
/*     case CA_CED:
      //layer_off(_TYPO);
      return finish_word((uint16_t[]) {FG_C, FG_A}, 2, record); */

    case OU_GRV:
      layer_off(_TYPO);
      return finish_word((uint16_t[]) {FG_O, FG_TYPO, FG_D}, 3, record);

    case AGRV_SPC:
/*     const bool is_shifted = (get_mods() | get_weak_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
    if (is_shifted) {
        del_weak_mods(MOD_MASK_SHIFT);
        del_oneshot_mods(MOD_MASK_SHIFT);
        unregister_mods(MOD_MASK_SHIFT);
    }
    invoke_key(FG_TYPO, record);
    if (is_shifted) { set_oneshot_mods(MOD_BIT(KC_LSFT)); } */
      layer_off(_TYPO);
      return finish_word((uint16_t[]) {FG_AGR, KC_SPC}, 2, record);
  }

  return false; // Process next keycode normally
}