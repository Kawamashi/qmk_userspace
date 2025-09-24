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

#include QMK_KEYBOARD_H

#include "keymap.h"

enum combos {
  R_BKSPC, 
  DELETE, 
  BK_WORD,
  DEL_WORD, 
  L_BKSPC, 
  HOME, 
  END, 
  ENTER,
  TAB,
  ESC, 
  HELP, 
  PANIC,
  //NUMWRD,
  ALTTAB,
  ALTESC
};

const uint16_t PROGMEM del_combo_d[] = {PG_T, PG_S, COMBO_END};
const uint16_t PROGMEM bkspc_combo_d[] = {PG_S, PG_R, COMBO_END};
const uint16_t PROGMEM del_word_combo[] = {PG_M, PG_C, COMBO_END};
const uint16_t PROGMEM bk_word_combo[] = {PG_C, PG_J, COMBO_END};
const uint16_t PROGMEM enter_combo[] = {PG_P, PG_U, COMBO_END};
const uint16_t PROGMEM tab_combo[] = {PG_N, PG_I, COMBO_END};
const uint16_t PROGMEM esc_combo[] = {PG_N, PG_A, COMBO_END};
const uint16_t PROGMEM bkspc_combo_g[] = {PG_A, PG_I, COMBO_END};
const uint16_t PROGMEM home_combo[] = {PG_Z, PG_Y, COMBO_END};
const uint16_t PROGMEM end_combo[] = {PG_U, PG_EACU, COMBO_END};
const uint16_t PROGMEM help_combo[] = {PG_EACU, PG_J, COMBO_END};
const uint16_t PROGMEM panic_combo[] = {PG_U, PG_C, COMBO_END};
//const uint16_t PROGMEM numword_combo[] = {PG_T, PG_R, COMBO_END};
const uint16_t PROGMEM alttab_combo[] = {PG_H, PG_Y, COMBO_END};
const uint16_t PROGMEM altesc_combo[] = {PG_A, PG_I, PG_N, COMBO_END};

combo_t key_combos[] = {
    [R_BKSPC] = COMBO(bkspc_combo_d, KC_BSPC),
    [DELETE] = COMBO(del_combo_d, KC_DEL),
    [BK_WORD] = COMBO(bk_word_combo, LCTL(KC_BSPC)), 
    [DEL_WORD] = COMBO(del_word_combo, LCTL(KC_DEL)), 
    [L_BKSPC] = COMBO(bkspc_combo_g, KC_BSPC),
    [HOME] = COMBO(home_combo, KC_HOME),
    [END] = COMBO(end_combo, KC_END),
    [ENTER] = COMBO(enter_combo, KC_ENT),
    [TAB] = COMBO(tab_combo, KC_TAB),
    [ESC] = COMBO(esc_combo, KC_ESC),
    [HELP] = COMBO(help_combo, AIDE_MEM), 
    [PANIC] = COMBO(panic_combo, RAZ),
    //[NUMWRD] = COMBO(numword_combo, NUMWORD),
    [ALTTAB] = COMBO(alttab_combo, KC_NO),
    [ALTESC] = COMBO(altesc_combo, LALT(KC_ESC))
    };

/* uint16_t get_combo_term(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case L_APOST:
        case D_APOST:
            return 100;
        default:
            return COMBO_TERM;
    }
} */

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    // Chorded mods shouldn't be considered as combos.
    if (os4a_layer != 0) {
      return (os4a_layer == _R_MODS) == on_left_hand(record->event.key);
    }
    // Some combos shouldn't be affected by global_quick_tap_timer.
    switch (combo_index) {
        case R_BKSPC:
        case BK_WORD:
        case ENTER:
        case HOME:
        case END:
          return true;

        default:
          //return timer_elapsed(global_quick_tap_timer) > TAP_INTERVAL;
          if (timer_elapsed(global_quick_tap_timer) < TAP_INTERVAL) {
            return false;
          }
    }
    return true;
}


void process_combo_event(uint16_t combo_index, bool pressed) {
  switch (combo_index) {
      case ALTTAB:
          if (pressed) {
              register_mods(MOD_LALT);
              tap_code(KC_TAB);
          } else {
              unregister_mods(MOD_LALT);
          }
          break;
  }
}

bool process_combo_key_repress(uint16_t combo_index, combo_t *combo, uint8_t key_index, uint16_t keycode) {
  switch (combo_index) {
      case ALTTAB:
          switch (keycode) {
            case PG_Y:
                tap_code16(S(KC_TAB));
                return true;
            case PG_H:
                tap_code(KC_TAB);
                return true;
          }
          break;

  }
  return false;
}