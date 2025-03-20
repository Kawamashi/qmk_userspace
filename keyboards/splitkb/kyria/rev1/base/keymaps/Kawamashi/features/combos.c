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
  NUMWRD,
  ALTTAB,
  CTRLALT
};

const uint16_t PROGMEM del_combo_d[] = {FR_S, FR_N, COMBO_END};
const uint16_t PROGMEM bkspc_combo_d[] = {FR_N, FR_R, COMBO_END};
const uint16_t PROGMEM del_word_combo[] = {FR_M, FR_L, COMBO_END};
const uint16_t PROGMEM bk_word_combo[] = {FR_L, FR_APOS, COMBO_END};
const uint16_t PROGMEM enter_combo[] = {FR_P, FR_EACU, COMBO_END};
const uint16_t PROGMEM tab_combo[] = {FR_T, FR_I, COMBO_END};
const uint16_t PROGMEM esc_combo[] = {FR_T, FR_A, COMBO_END};
const uint16_t PROGMEM bkspc_combo_g[] = {FR_A, FR_I, COMBO_END};
const uint16_t PROGMEM home_combo[] = {FR_Z, FR_Y, COMBO_END};
const uint16_t PROGMEM end_combo[] = {FR_EACU, FR_VIRG, COMBO_END};
const uint16_t PROGMEM help_combo[] = {FR_VIRG, FR_APOS, COMBO_END};
const uint16_t PROGMEM panic_combo[] = {FR_EACU, FR_L, COMBO_END};
const uint16_t PROGMEM numword_combo[] = {FR_S, FR_R, COMBO_END};
const uint16_t PROGMEM alttab_combo[] = {FR_D, FR_Y, COMBO_END};
const uint16_t PROGMEM ctrlaFR_Combo[] = {FR_A, FR_I, FR_T, COMBO_END};

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
    [NUMWRD] = COMBO(numword_combo, NUMWORD),
    [ALTTAB] = COMBO(alttab_combo, KC_NO),
    [CTRLALT] = COMBO(ctrlaFR_Combo, RCTL(RALT(KC_DEL)))
    };

/* uint16_t get_combo_term(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case HOME:
            return COMBO_TERM - 10;
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
              case FR_Y:
                  tap_code16(S(KC_TAB));
                  return true;
              case FR_D:
                  tap_code(KC_TAB);
                  return true;
          }
  }
  return false;
}