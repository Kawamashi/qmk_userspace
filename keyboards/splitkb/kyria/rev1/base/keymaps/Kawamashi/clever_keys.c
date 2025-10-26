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

//#include "clever_keys.h"
#include "features/clever_keys_utilities.h"


void get_clever_keycode(uint16_t* ongoing_keycode, keyrecord_t* record) {

  uint16_t prev_keycode = get_recent_keycode(-1);

    // Apostrophe
    if (is_followed_by_apos(*ongoing_keycode, prev_keycode, record)) {
      set_last_keycode(PG_APOS);
    }
  
  switch (prev_keycode) {

    case NNB_SPC:
      switch (*ongoing_keycode) {
        // Shift auto de la ponctuation après une espace fine insécable
        case PG_POIN:
          // uses less space than process_word
          tap_code(KC_BSPC);
          process_key(SAGR(KC_SPC), record);
        case PG_TIRE:
        case PG_VIRG:
          return replace_ongoing_key(S(*ongoing_keycode), ongoing_keycode, record);
      }
      break;

    case KC_SPC:
      switch (get_recent_keycode(-2)) {
        case PG_EXCL:
        case PG_QUES:
        case PG_3PTS:
        case PG_POIN:
          // Shift the letter at the beginning of sentences.
          if (is_letter(*ongoing_keycode) || is_send_string_macro(*ongoing_keycode)) {
            set_oneshot_mods(MOD_BIT(KC_LSFT));     // Don't use weak mods !
          }
          break;
      }
      break;

    case PG_Q:
      switch (*ongoing_keycode) {
        
        // Ajout automatique du "u" après le "q"
        case E_CIRC:
          tap_code(PG_U);
          break;
        case PG_E:
        case PG_I:
        case PG_A:
        case PG_O:
        case PG_EACU:
        case PG_APOS:
          invoke_key(PG_U, record);
          set_last_keycode(*ongoing_keycode);
          break;

        // Raccourci pour "quoi"
        case PG_H:
          finish_word((uint16_t[]) {PG_U, PG_O, PG_I}, 3, ongoing_keycode, record);
          break;

        // Raccourci pour "quand"
        case PG_N:
          return finish_word((uint16_t[]) {PG_U, PG_A, PG_N, PG_D}, 4, ongoing_keycode, record);
      }
      break;

    case PG_P:
      switch (*ongoing_keycode) {
        case PG_C:
          // "pas"
          return finish_word((uint16_t[]) {PG_A, PG_S}, 2, ongoing_keycode, record);
        
        case PG_J:
          // "pour"
          return finish_word((uint16_t[]) {PG_O, PG_U, PG_R}, 3, ongoing_keycode, record);

        case PG_X:
          // "plus"
          return finish_word((uint16_t[]) {PG_L, PG_U, PG_S}, 3, ongoing_keycode, record);
      }
      break;

    case PG_M:
    case PG_R:
    case PG_A:
    case PG_E:
    case PG_EACU:
    case PG_I:
    case PG_O:
    case PG_U:
    case PG_Y:
    case E_GRV:
      if (*ongoing_keycode == PG_H) {
          update_bkspc_countdown(0);
          return replace_ongoing_key(PG_B, ongoing_keycode, record);
      }
      break;
  }

  
  switch (*ongoing_keycode) {

    case MAGIC:
      switch (prev_keycode) {
        case PG_O:
          // oui
          process_key(PG_U, record);
        case PG_U:
          // ui SFB
          return replace_ongoing_key(PG_I, ongoing_keycode, record);

        case PG_EACU:
          // éa SFB
          return replace_ongoing_key(PG_A, ongoing_keycode, record);

        case PG_S:
          // sc SFB
          return replace_ongoing_key(PG_C, ongoing_keycode, record);

        case PG_C:
          // cs SFB
          return replace_ongoing_key(PG_S, ongoing_keycode, record);

        case PG_N:
          // n. SFB
          return replace_ongoing_key(PG_POIN, ongoing_keycode, record);

        case PG_P:
          // ph SFB
          return replace_ongoing_key(PG_H, ongoing_keycode, record);

        case PG_G:
          // gt SFB
          return replace_ongoing_key(PG_T, ongoing_keycode, record);

        case PG_Q:
          // qué scissor
          //return finish_word((uint16_t[]) {PG_U, PG_EACU}, 2, ongoing_keycode, record);
          process_key(PG_U,record);
          return replace_ongoing_key(PG_EACU, ongoing_keycode, record);

        case PG_Y:
          // you bad redirection
          //return finish_word((uint16_t[]) {PG_O, PG_U}, 2, ongoing_keycode, record);
          process_key(PG_O,record);
          return replace_ongoing_key(PG_U, ongoing_keycode, record);

        case PG_T:
          // "the"
          //return finish_word((uint16_t[]) {PG_H, PG_E}, 2, ongoing_keycode, record);
          process_key(PG_H,record);
          return replace_ongoing_key(PG_E, ongoing_keycode, record);

        case PG_I:
          //return finish_word((uint16_t[]) {PG_O, PG_N}, 2, ongoing_keycode, record);
          process_key(PG_O,record);
          return replace_ongoing_key(PG_N, ongoing_keycode, record);

        case PG_M:
          if (is_letter(get_recent_keycode(-2))) {
            // "ment"
            return finish_word((uint16_t[]) {PG_E, PG_N, PG_T}, 3, ongoing_keycode, record);
          } else {
            // "même"
            return finish_word((uint16_t[]) {PG_ODK, PG_O, PG_M, PG_E}, 4, ongoing_keycode, record);
          }

        case PG_B:
          // "beaucoup"
          return finish_word((uint16_t[]) {PG_E, PG_A, PG_U, PG_C, PG_O, PG_U, PG_P}, 7, ongoing_keycode, record);

/*         case PG_D:
          // "déjà"
          return finish_word((uint16_t[]) {PG_EACU, PG_J, PG_ODK, PG_A}, 4, ongoing_keycode, record); */
        
        default:
          // "à"
          tap_code(PG_ODK);
          //process_key(PG_ODK, record);
          return replace_ongoing_key(PG_A, ongoing_keycode, record);
      }

    case PG_AROB:
      if (!is_letter(get_recent_keycode(-2))) {
        switch (prev_keycode) {

/*           case PG_P:
            // "p@" -> "problème"
            layer_off(_ODK);
            return finish_word((uint16_t[]) {PG_R, PG_O, PG_B, PG_L, PG_ODK, PG_E, PG_M, PG_E}, 8, ongoing_keycode, record); */

          case PG_A:
            // "a@" -> "aujourd'hui"
            layer_off(_ODK);
            return finish_word((uint16_t[]) {PG_U, PG_J, PG_O, PG_U, PG_R, PG_D, PG_APOS, PG_H, PG_U, PG_I}, 10, ongoing_keycode, record);
        }
      }
      break;

    case PG_M:
      if (prev_keycode == PG_C) {
        // "cm" -> "ch"
        update_bkspc_countdown(0);
        return replace_ongoing_key(PG_H, ongoing_keycode, record);
      }
      break;

/*     case PG_H:
      switch (prev_keycode) {
        case PG_M:
        case PG_R:
        case PG_A:
        case PG_E:
        case PG_EACU:
        case PG_I:
        case PG_O:
        case PG_U:
        case PG_Y:
          update_bkspc_countdown(0);
          return replace_ongoing_key(PG_B, ongoing_keycode, record);
      } */
/*       if (prev_keycode == PG_M) {
        // "mh" -> "mb"
        update_bkspc_countdown(0);
        return replace_ongoing_key(PG_B, ongoing_keycode, record);
      } else if (prev_keycode == PG_I) {
        // "ih" -> "ique"
        return finish_word((uint16_t[]) {PG_Q, PG_U, PG_E}, 3, ongoing_keycode, record);
      } */
      break;

    case OU_GRV:
      layer_off(_ODK);
      return finish_word((uint16_t[]) {PG_O, PG_ODK, PG_N}, 3, ongoing_keycode, record);
      //return replace_ongoing_key(prev_keycode, ongoing_keycode, record);

    case N_TILD:
      return replace_ongoing_key(PG_H, ongoing_keycode, record);

    case PG_APOS:
      if (replace_apos()) { return replace_ongoing_key(PG_APOD, ongoing_keycode, record); }
      break;
  }

}