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
  //static bool is_shifted = false;

    // Apostrophe
    if (is_followed_by_apos(*next_keycode, prev_keycode)) {
      set_last_keycode(PG_APOS);
      //apostrophe = true;
    }

    //if (is_caps_lock_on()) {
/*       if (is_letter(*next_keycode) || is_send_string_macro(*next_keycode)) {
        if (recent[RECENT_SIZE - 3] == PG_E && recent[RECENT_SIZE - 2] == PG_T && prev_keycode == KC_SPC) {
          caps_word_on();
        }
        if (recent[RECENT_SIZE - 2] == PG_VIRG && prev_keycode == KC_SPC && is_letter(*next_keycode)) {
          caps_word_on();
        }
      } */
    //}

  
  switch (prev_keycode) {

    case NNB_SPC:
      switch (*next_keycode) {
        // Shift auto de la ponctuation après une espace fine insécable
        case PG_POIN:
          process_word((uint16_t[]) {KC_BSPC, SAGR(KC_SPC)}, 2, record);
        case PG_TIRE:
        case PG_VIRG:
          return replace_ongoing_key(S(*next_keycode), next_keycode, record);
      }
      break;

    case KC_SPC:
      switch (recent[RECENT_SIZE - 2]) {
        case PG_EXCL:
        case PG_QUES:
        case PG_3PTS:
        case PG_POIN:
          // Shift the letter at the beginning of sentences.
          if (is_letter(*next_keycode) || is_send_string_macro(*next_keycode)) {
            //if (!is_caps_lock_on()) { add_weak_mods(MOD_BIT(KC_LSFT)); }
            add_weak_mods(MOD_BIT(KC_LSFT));
          }
          break;
      }
      break;

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

        // Raccourci pour "quoi"
        case PG_H:
          finish_word((uint16_t[]) {PG_U, PG_O, PG_I}, 3, next_keycode, record);
          break;

        // Raccourci pour "quand"
        case PG_N:
          return finish_word((uint16_t[]) {PG_U, PG_A, PG_N, PG_D}, 4, next_keycode, record);
      }
      break;

    case PG_P:
      switch (*next_keycode) {        
        case PG_C:
          // "pas"
          return finish_word((uint16_t[]) {PG_A, PG_S}, 2, next_keycode, record);
        
        case PG_J:
          if (!is_letter(recent[RECENT_SIZE - 2])) {
            // "pour"
            return finish_word((uint16_t[]) {PG_O, PG_U, PG_R}, 3, next_keycode, record);
          }
          break;

        case PG_X:
          // "plus"
          return finish_word((uint16_t[]) {PG_L, PG_U, PG_S}, 3, next_keycode, record);
      }
      break;
  }

  
  switch (*next_keycode) {

    case MAGIC:
      switch (prev_keycode) {
        case PG_O:
          // oui
          invoke_key(PG_U, record);
        case PG_U:
          // ui SFB
          return replace_ongoing_key(PG_I, next_keycode, record);

/*         case PG_L:
          // là
          return finish_word((uint16_t[]) {PG_ODK, PG_A}, 2, next_keycode, record); */

        case PG_EACU:
          // éa SFB
          return replace_ongoing_key(PG_A, next_keycode, record);

        case PG_S:
          // sc SFB
          return replace_ongoing_key(PG_C, next_keycode, record);

        case PG_C:
          // cs SFB
          return replace_ongoing_key(PG_S, next_keycode, record);

        case PG_N:
          // n. SFB
          return replace_ongoing_key(PG_POIN, next_keycode, record);

        case PG_P:
          // ph SFB
          return replace_ongoing_key(PG_H, next_keycode, record);

        case PG_G:
          // gt SFB
          return replace_ongoing_key(PG_T, next_keycode, record);

        case PG_Q:
          // qué scissor
          return finish_word((uint16_t[]) {PG_U, PG_EACU}, 2, next_keycode, record);

        case PG_Y:
          // you bad redirection
          return finish_word((uint16_t[]) {PG_O, PG_U}, 2, next_keycode, record);

        case PG_T:
          // "the"
          return finish_word((uint16_t[]) {PG_H, PG_E}, 2, next_keycode, record);
          
        case PG_I:
          return finish_word((uint16_t[]) {PG_O, PG_N}, 2, next_keycode, record);
        
        case PG_M:
          if (is_letter(recent[RECENT_SIZE - 2])) {
            // "ment"
            return finish_word((uint16_t[]) {PG_E, PG_N, PG_T}, 3, next_keycode, record);
          } else {
            // "même"
            return finish_word((uint16_t[]) {PG_ODK, PG_O, PG_M, PG_E}, 4, next_keycode, record);
          }

        case PG_B:
          // "beaucoup"
          return finish_word((uint16_t[]) {PG_E, PG_A, PG_U, PG_C, PG_O, PG_U, PG_P}, 7, next_keycode, record);

        case PG_D:
          // "déjà"
          return finish_word((uint16_t[]) {PG_EACU, PG_J, PG_ODK, PG_A}, 4, next_keycode, record);
        
        default:
          // "à"
          process_key(PG_ODK,record);

          if (is_shifted) {
              is_shifted = false;
              add_weak_mods(MOD_BIT(KC_LSFT));
          }
          return replace_ongoing_key(PG_A, next_keycode, record);
      }

    case PG_AROB:
      if (!is_letter(recent[RECENT_SIZE - 2])) {
        switch (prev_keycode) {

          case PG_P:
            // "p@" -> "problème"
            layer_off(_ODK);
            return finish_word((uint16_t[]) {PG_R, PG_O, PG_B, PG_L, PG_ODK, PG_E, PG_M, PG_E}, 8, next_keycode, record);

          case PG_A:
            // "a@" -> "aujourd'hui"
            layer_off(_ODK);
            return finish_word((uint16_t[]) {PG_U, PG_J, PG_O, PG_U, PG_R, PG_D, PG_APOS, PG_H, PG_U, PG_I}, 10, next_keycode, record);
        }
      }
      break;

    case PG_M:
      if (prev_keycode == PG_C) {
        // "cm" -> "ch"
        bkspc_countdown = 0;
        return replace_ongoing_key(PG_H, next_keycode, record);
      }
      break;

    case PG_H:
      if (prev_keycode == PG_M) {
        // "mh" -> "mb"
        bkspc_countdown = 0;
        return replace_ongoing_key(PG_B, next_keycode, record);
      } else if (prev_keycode == PG_I) {
        // "ih" -> "ique"
        return finish_word((uint16_t[]) {PG_Q, PG_U, PG_E}, 3, next_keycode, record);
      }
      break;

/*     case PG_R:
      if (prev_keycode == PG_L) {
          // "lr" -> "l" + 1DK
          //bkspc_countdown = 0;
          return replace_ongoing_key(OS_ODK, next_keycode, record);
      }
      break; */

    case OU_GRV:
      layer_off(_ODK);
      return finish_word((uint16_t[]) {PG_O, PG_ODK, PG_N}, 3, next_keycode, record);

/*      case PG_BL:
      layer_off(_ODK);
      return finish_word((uint16_t[]) {PG_B, PG_L}, 2, next_keycode, record); */

/*     case J_APOS:
      layer_off(_ODK);
      return finish_word((uint16_t[]) {PG_J, PG_APOS}, 2, next_keycode, record); */

    case PG_APOS:
      if (is_apos_dr) { return replace_ongoing_key(PG_APOD, next_keycode, record); }
      break;
  }

}