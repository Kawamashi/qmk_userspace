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
        case FR_EXLM:
        case FR_QUES:
        //case FR_3PTS:
        case FR_2PTS:
          // Add space between punctuation and letters.
          invoke_key(KC_SPC, record);

        case KC_SPC:
          switch (recent[RECENT_SIZE - 2]) {
            case FR_EXLM:
            case FR_QUES:
            //case FR_3PTS:
            case FR_POIN:
              // Add OS shift at the beginning of sentences.
              if (!is_caps_lock_on()) { set_oneshot_mods(MOD_BIT(KC_LSFT)); }
              break;
          }
    }
  }

  
  switch (prev_keycode) {

    case FR_Q:
      switch (next_keycode) {
        
        // Ajout automatique du "u" après le "q"
        case FR_E:
        case FR_I:
        case FR_A:
        case FR_O:
        case FR_EACU:
        case E_CIRC:
        case FR_APOS:
          invoke_key(FR_U, record);
          break;

        // Raccourci pour "quelq"
        case FR_Q:
          clear_recent_keys();  // To prevent infinite loop
          process_word((uint16_t[]) {FR_U, FR_E, FR_L}, 3, record);
          break;

        // Raccourci pour "quoi", ça évite un aller-retour sur la main gauche.
        case FR_Z:
          return finish_word((uint16_t[]) {FR_U, FR_O, FR_I}, 3, record);

        // Raccourci pour "quand"
        case FR_D:
          process_word((uint16_t[]) {FR_U, FR_A, FR_N}, 3, record);
          break;
      }
      break;

    case FR_P:
      switch (next_keycode) {

        case FR_M:
          // "pas"
          return finish_word((uint16_t[]) {FR_A, FR_S}, 2, record);
        
        case FR_APOS:
          // "par"
          return finish_word((uint16_t[]) {FR_A, FR_R}, 2, record);
        
        case FR_POIN:
          if (!isLetter(recent[RECENT_SIZE - 2])) {
            // "pour"
            return finish_word((uint16_t[]) {FR_O, FR_U, FR_R}, 3, record);
          }
          break;       
      }        
  }

  
  switch (next_keycode) {

    case FR_QUES:
    case FR_EXLM:
      // On ajoute un espace insécable s'il n'a pas été entré avant le point d'exclamation.
      // Il ne faut pas tester cette fonctionnalité avec Word, qui ajoute cet espace automatiquement.
      if (isLetter(recent[RECENT_SIZE - 1])) {
        
/*         if ((mods | get_oneshot_mods() | get_weak_mods()) & MOD_MASK_SHIFT) {
          del_weak_mods(MOD_MASK_SHIFT);
          del_oneshot_mods(MOD_MASK_SHIFT);
          unregister_mods(MOD_MASK_SHIFT);
        } */
        invoke_key(S(KC_SPC), record);
        //set_mods(mods);
        return replace_next_key(next_keycode, record);
      }
      break;

    case MAGIC:
      switch (prev_keycode) {

        case FR_T:
          invoke_key(FR_I, record);
        case FR_I:
          return finish_magic((uint16_t[]) {FR_O, FR_N}, 2, record);
        
        case FR_C:
          return finish_magic((uint16_t[]) {FR_APOS, FR_E, FR_S, FR_T}, 4, record);
        
        case FR_D:
          return finish_magic((uint16_t[]) {FR_A, FR_N, FR_S}, 3, record);
        
        case FR_P:
          return finish_magic((uint16_t[]) {FR_L, FR_U, FR_S}, 3, record);

        case FR_O:
          return finish_magic((uint16_t[]) {FR_N, FR_T}, 2, record);
        
        case FR_A:
          if (isLetter(recent[RECENT_SIZE - 2])) {
            // "ant"
            return finish_magic((uint16_t[]) {FR_N, FR_T}, 2, record);
          } else {
            // "avec"
            return finish_magic((uint16_t[]) {FR_V, FR_E, FR_C}, 3, record);
          }
        
        case FR_S:
          return finish_magic((uint16_t[]) {FR_U, FR_R}, 2, record);
        
        case FR_B:
          process_word((uint16_t[]) {FR_O, FR_N, FR_J}, 3, record);
        
        case FR_J:
          return finish_magic((uint16_t[]) {FR_O, FR_U, FR_R}, 3, record);
        
        case FR_M:
          if (isLetter(recent[RECENT_SIZE - 2])) {
            // "ment"
            return finish_magic((uint16_t[]) {FR_E, FR_N, FR_T}, 3, record);
          } else {
            // "même"
            return finish_magic((uint16_t[]) {FR_TYPO, FR_O, FR_M, FR_E}, 4, record);
          }
        
        default:
          return false;
      }

    case FR_AROB:
      if (!isLetter(recent[RECENT_SIZE - 2])) {
        switch (prev_keycode) {

          case FR_T:
            // "t@" -> "toujours"
            layer_off(_TYPO);
            return finish_word((uint16_t[]) {FR_O, FR_U, FR_J, FR_O, FR_U, FR_R, FR_S}, 7, record);

          case FR_P:
            // "p@" -> "peut-être"
            layer_off(_TYPO);
            return finish_word((uint16_t[]) {FR_E, FR_U, FR_T, FR_MOIN, FR_TYPO, FR_O, FR_T, FR_R, FR_E}, 9, record);

          case FR_A:
            // "a@" -> "aujourd'hui"
            layer_off(_TYPO);
            return finish_word((uint16_t[]) {FR_U, FR_J, FR_O, FR_U, FR_R, FR_D, FR_APOS, FR_H, FR_U, FR_I}, 10, record);
          
          case FR_B:
            // "b@" -> "beaucoup"
            layer_off(_TYPO);
            return finish_word((uint16_t[]) {FR_E, FR_A, FR_U, FR_C, FR_O, FR_U, FR_P}, 7, record);
          
          case FR_E:
            // "e@" -> "est-ce qu"
            layer_off(_TYPO);
            return finish_word((uint16_t[]) {FR_S, FR_T, FR_MOIN, FR_C, FR_E, KC_SPC, FR_Q}, 7, record);
          
          case FR_D:
            // "d@" -> "déjà"
            layer_off(_TYPO);
            return finish_word((uint16_t[]) {FR_EACU, FR_J, FR_AGRV}, 3, record);
        }          
      }
      break;

    case FR_Q:
      if (prev_keycode == FR_J) {
          // "jq" -> "jusqu"
          process_word((uint16_t[]) {FR_U, FR_S}, 2, record);
      }
      break;

    case FR_A:
      //if (prev_keycode == FR_O && !isCaps) {
      if (prev_keycode == FR_O) {
        // "oa" -> "oi"
        clear_recent_keys();  // To prevent infinite loop
        return replace_next_key(FR_I, record);
      }
      break;

    case FR_I:
      //if (prev_keycode == FR_O && !isCaps && recent[RECENT_SIZE - 3] != FR_Q) {
      if (prev_keycode == FR_O && recent[RECENT_SIZE - 3] != FR_Q) {
        // "oi" -> "oa", for "keyboard"
        clear_recent_keys();  // To prevent infinite loop
        return replace_next_key(FR_A, record);
      }
      break;

    case FR_O:
      if (prev_keycode == FR_U && recent[RECENT_SIZE - 2] != FR_Q) {
        // "uo" -> "un"
        return replace_next_key(FR_N, record);
      }
      break;
    
    case CA_CED:
      layer_off(_TYPO);
      return finish_word((uint16_t[]) {FR_CCED, FR_A}, 2, record);

    case OU_GRV:
      layer_off(_TYPO);
      return finish_word((uint16_t[]) {FR_O, FR_UGRV}, 2, record);

    case AGRV_SPC:
      layer_off(_TYPO);
      return finish_word((uint16_t[]) {FR_AGRV, KC_SPC}, 2, record);

/*    case E_CIRC:
      return process_accent(FR_ACIR, FR_E, record);

    case I_CIRC:
      return process_accent(FR_ACIR, FR_I, record);

    case A_CIRC:
      return process_accent(FR_ACIR, FR_A, record);

    case O_CIRC:
      return process_accent(FR_ACIR, FR_O, record);

    case U_CIRC:
      return process_accent(FR_ACIR, FR_U, record);

     case I_TREM:
      return process_accent(FR_TREM, FR_I, record); 

    case CIRC:
      return process_accent(FR_ACIR, KC_SPC, record);

    case TILDE:
      return process_accent(FR_TILD, KC_SPC, record);

    case GRAVE:
      return process_accent(FR_GRV, KC_SPC, record);*/
  }

  return false; // Process next keycode normally
}