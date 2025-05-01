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


void get_clever_keycode(uint16_t* next_keycode, keyrecord_t* record) {

  uint16_t prev_keycode = recent[RECENT_SIZE - 1];

    if (isLetter(*next_keycode) || isSendStringMacro(*next_keycode)) {
      switch (prev_keycode) {
          case PG_EXCL:
          case PG_QUES:
          case PG_3PTS:
          case PG_2PTS:
            // Add space between punctuation and letters.
            invoke_key(KC_SPC, record);
            set_last_keycode(*next_keycode);

          case KC_SPC:
            switch (recent[RECENT_SIZE - 2]) {
              case PG_EXCL:
              case PG_QUES:
              case PG_3PTS:
              case PG_POIN:
                // Add OS shift at the beginning of sentences.
                if (!is_caps_lock_on()) { set_oneshot_mods(MOD_BIT(KC_LSFT)); }
                break;
            }
      }
    }

  
  switch (prev_keycode) {

    case PG_Q:
      switch (*next_keycode) {
        
        // Ajout automatique du "u" après le "q"
        case PG_E:
        case PG_I:
        case PG_A:
        case PG_O:
        case PG_EACU:
        case PG_APOS:
          invoke_key(PG_U, record);
          set_last_keycode(*next_keycode);
          break;

        // Raccourci pour "quelq"
        case PG_Q:
          process_word((uint16_t[]) {PG_U, PG_E, PG_L}, 3, record);
          break;

        // Raccourci pour "quoi", ça évite un aller-retour sur la main gauche.
        case PG_Z:
          finish_word((uint16_t[]) {PG_U, PG_O, PG_I}, 3, next_keycode, record);
          break;

        // Raccourci pour "quand"
        case PG_D:
          process_word((uint16_t[]) {PG_U, PG_A, PG_N}, 3, record);
          set_last_keycode(*next_keycode);
          break;
      }
      break;

    case PG_P:
      switch (*next_keycode) {

        case PG_M:
          // "par"
          return finish_word((uint16_t[]) {PG_A, PG_R}, 2, next_keycode, record);
        
        case PG_C:
          // "pas"
          return finish_word((uint16_t[]) {PG_A, PG_S}, 2, next_keycode, record);
        
        case PG_APOS:
          if (!isLetter(recent[RECENT_SIZE - 2])) {
            // "pour"
            return finish_word((uint16_t[]) {PG_O, PG_U, PG_R}, 3, next_keycode, record);
          }
          break;

        case PG_X:
          // "plus"
          return finish_word((uint16_t[]) {PG_L, PG_U, PG_S}, 3, next_keycode, record);
      }
      break;
    
/*     case PG_CCED:
      if (!isLetter(*next_keycode)) {
        invoke_key(PG_A, record);
        set_last_keycode(*next_keycode);
      }
      break; */
  }

  
  switch (*next_keycode) {

    case PG_QUES:
    case PG_EXCL:
      // On ajoute un espace insécable s'il n'a pas été entré avant le point d'exclamation.
      // Il ne faut pas tester cette fonctionnalité avec Word, qui ajoute cet espace automatiquement.
      if (isLetter(recent[RECENT_SIZE - 1])) {
        invoke_key(KC_SPC, record);
        return replace_ongoing_key(*next_keycode, next_keycode, record);
      }
      break;

    case MAGIC:
      switch (prev_keycode) {

        case PG_U:
          // ui SFB
          bkspc_countdown = 0;
          return replace_ongoing_key(PG_I, next_keycode, record);

        case PG_EACU:
          // éa SFB
          bkspc_countdown = 0;
          return replace_ongoing_key(PG_A, next_keycode, record);

        case PG_S:
          // sc SFB
          bkspc_countdown = 0;
          return replace_ongoing_key(PG_C, next_keycode, record);

        case PG_L:
          // là SFB
          return finish_word((uint16_t[]) {PG_ODK, PG_A}, 2, next_keycode, record);

        case PG_N:
          // sc SFB
          bkspc_countdown = 0;
          return replace_ongoing_key(PG_VIRG, next_keycode, record);

        case PG_P:
          // ph SFB
          bkspc_countdown = 0;
          return replace_ongoing_key(PG_H, next_keycode, record);

        case PG_Q:
          return finish_word((uint16_t[]) {PG_U, PG_EACU}, 2, next_keycode, record);

        case PG_T:
          invoke_key(PG_I, record);
        case PG_I:
          return finish_word((uint16_t[]) {PG_O, PG_N}, 2, next_keycode, record);
        
/*         case PG_D:
          // "c’est" 
          return finish_word((uint16_t[]) {PG_APOS, PG_E, PG_T, PG_N}, 4, next_keycode, record); 
        
        case PG_H:
          // "dans"
          return finish_word((uint16_t[]) {PG_A, PG_S, PG_T}, 3, next_keycode, record);
        
        case PG_P:
          // "plus"
          return finish_word((uint16_t[]) {PG_C, PG_L, PG_T}, 3, next_keycode, record);
        
        case PG_A:
          // "avec"
          return finish_word((uint16_t[]) {PG_G, PG_E, PG_D}, 3, next_keycode, record); */
        
/*         case PG_B:
          // "bonjour"
          process_word((uint16_t[]) {PG_O, PG_S, PG_J}, 3, record);
        
        case PG_J:
          // "jour"
          return finish_word((uint16_t[]) {PG_O, PG_L, PG_R}, 3, next_keycode, record); */
        
        case PG_M:
          if (isLetter(recent[RECENT_SIZE - 2])) {
            // "ment"
            return finish_word((uint16_t[]) {PG_E, PG_N, PG_T}, 3, next_keycode, record);
          } else {
            // "même"
            return finish_word((uint16_t[]) {PG_ODK, PG_O, PG_M, PG_E}, 4, next_keycode, record);
          }
        
        default:
          return;
      }

    case PG_AROB:
      if (!isLetter(recent[RECENT_SIZE - 2])) {
        switch (prev_keycode) {

/*           case PG_N:
            // "t@" -> "toujours"
            layer_off(_ODK);
            return finish_word((uint16_t[]) {PG_O, PG_L, PG_J, PG_O, PG_L, PG_R, PG_T}, 7, next_keycode, record); */

          case PG_P:
            // "p@" -> "peut-être"
            layer_off(_ODK);
            return finish_word((uint16_t[]) {PG_E, PG_U, PG_T, PG_MOIN, PG_ODK, PG_O, PG_T, PG_R, PG_E}, 9, next_keycode, record);

          case PG_A:
            // "a@" -> "aujourd'hui"
            layer_off(_ODK);
            return finish_word((uint16_t[]) {PG_U, PG_J, PG_O, PG_U, PG_R, PG_D, PG_APOS, PG_H, PG_U, PG_I}, 10, next_keycode, record);
          
          case PG_B:
            // "b@" -> "beaucoup"
            layer_off(_ODK);
            return finish_word((uint16_t[]) {PG_E, PG_A, PG_U, PG_C, PG_O, PG_U, PG_P}, 7, next_keycode, record);
          
/*           case PG_E:
            // "e@" -> "est-ce qu"
            layer_off(_ODK);
            return finish_word((uint16_t[]) {PG_T, PG_N, PG_MOIN, PG_D, PG_E, KC_SPC, PG_Q}, 7, next_keycode, record); */
          
          case PG_D:
            // "d@" -> "déjà"
            layer_off(_ODK);
            return finish_word((uint16_t[]) {PG_EACU, PG_J, PG_ODK, PG_A}, 4, next_keycode, record);
        }          
      }
      break;

    case PG_Q:
      if (prev_keycode == PG_J) {
          // "jq" -> "jusqu"
          process_word((uint16_t[]) {PG_U, PG_S}, 2, record);
          set_last_keycode(*next_keycode);
          return;
      }
      break;

      case PG_M:
      if (prev_keycode == PG_C) {
          // "cm" -> "ch"
          bkspc_countdown = 0;
          return replace_ongoing_key(PG_H, next_keycode, record);
      }
      break;

    case PG_A:
      //if (prev_keycode == PG_O && !isCaps) {
      if (prev_keycode == PG_O) {
        // "oa" -> "oi"
        bkspc_countdown = 0;
        return replace_ongoing_key(PG_I, next_keycode, record);
      }
      break;

    case PG_I:
      //if (prev_keycode == PG_O && !isCaps && recent[RECENT_SIZE - 3] != PG_Q) {
      if (prev_keycode == PG_O && recent[RECENT_SIZE - 3] != PG_Q) {
        // "oi" -> "oa", for "keyboard"
        bkspc_countdown = 0;
        return replace_ongoing_key(PG_A, next_keycode, record);
      }
      break;

    case OU_GRV:
      layer_off(_ODK);
      return finish_word((uint16_t[]) {PG_O, PG_ODK, PG_N}, 3, next_keycode, record);

    case PG_APOS:
      if (is_apos_dr) { return replace_ongoing_key(PG_APOD, next_keycode, record); }
      break;

/*     case CA_CED:
      layer_off(_ODK);
      return finish_word((uint16_t[]) {PG_CCED, PG_A}, 2, next_keycode, record); */

/*     case AGRV_SPC:
      layer_off(_ODK);
      return finish_word((uint16_t[]) {PG_AGR, KC_SPC}, 2, next_keycode, record); */
  }

  //return KC_NO; // Process next keycode normally
}