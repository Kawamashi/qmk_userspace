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


const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
    LAYOUT_split_3x5_3(
        'L', 'L', 'L', 'L', 'L',           'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L',           'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L',           'R', 'R', 'R', 'R', '*',
                       '*', 'L', '*', '*', 'R', '*'
    );

bool get_speculative_hold(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {  // Enable speculative holding for these keys.
    case M(PG_I):
    case I(PG_N):
      return true;
  }
  return false;  // Disable otherwise.
}

// Housekeeping

void housekeeping_task_user(void) {
  housekeeping_task_tap_flow();
  recent_keys_task();
  modword_task();
  layerword_task();
  oneshot_task();
}


// Key processing

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {

  process_flow_tap(keycode, record);
  //pre_process_speculative_hold(keycode, record);

  return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // Speculative Hold
  //if (!process_record_speculative_hold(keycode, record)) { return false; }

  // LT Repeat and Magic keys
  if (!process_macros_I(keycode, record)) { return false; }

  // 
  if (!process_custom_oneshot(keycode, record)) { return false; }

  // Layer word
  if (!process_layerword(keycode, record)) { return false; }

  // Prefixed layers
  if (!process_prefixing_layers(keycode, record)) { return false; }

  // Clever keys
  process_clever_keys(keycode, record);

  // Caps Word
  if (!process_modword(keycode, record)) {return false; }

  // Other macros
  if (!process_macros_II(keycode, record)) { return false; }

  // Process all other keycodes normally
  return true;
}


// Keymap

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: ALPHAS
 *»\
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |   ,  |   É  |   U  |   P  |   -  |                              |   V  |   M  |   C  |   J  |   X  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |   O  |   A  |   I  |   N  |   .  |                              |   G  |   T  |   S  |   R  |   L  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |   Q  |   È  |   Y  |   H  |      |      |      |  |      |      |      |   D  |   F  |   W  |  1DK |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      | OS4A |   E  | Magic|  |Repeat| Space| OS4A |      |      |
 *                        |      |      |      | Symb |  Nav |  | Num  | Symb |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_BASE] = LAYOUT_split_3x5_3(
      PG_VIRG, PG_EACU, PG_U,    PG_P,    PG_TIRE,                 PG_V,   PG_M,    PG_C,    PG_J,    PG_X,   
      P(PG_O), R(PG_A), M(PG_I), I(PG_N), PG_POIN,                 PG_G,   I(PG_T), M(PG_S), R(PG_R), P(PG_L),
      PG_Q,    PG_EGRV, PG_Y,    PG_H,    KC_T,                    KC_NO,  PG_D,    PG_F,    PG_W,    OS_1DK, 
                                 OS_LSFT, LT_E,   LT_MGC, LT_REPT, LT_SPC, NUMWORD
    ),


/*
 * Layer 1 : Numbers
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |  -   |  +   |  €   |  %   |                              |Expos.|  °   |  =   |      |  1DK |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  4   |  3   |  2   |  1   |  :   |                              |Indice|  6   |  7   |  8   |  9   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |  H   |  5   |      |      |      |  |      |      |      |  /   |  -   |  +   |  *   |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |  .   |  0   |NbSpc |  |      |Space |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NUMROW] = LAYOUT_split_3x5_3(
      // S(KC_4), S(KC_3) and S(PG_EGAL) are here to give easy access to ⅔, ¾ and ≠.
      PG_VIRG, PG_MOIN, PG_PLUS, PG_EURO, PG_PERC,                   PG_EXP,  PG_DEG,  PG_EGAL, S(PG_EGAL), NUMPAD, 
      KC_4,    KC_3,    M(KC_2), KC_1,    PG_POIN,                   PG_IND,  KC_6,    M(KC_7), KC_8,       KC_9,   
      S(KC_4), S(KC_3), PG_H,    KC_5,    _______,                   _______, PG_SLSH, PG_MOIN, PG_PLUS,    PG_ASTX,
                                 _______, LT_0   , LT_PDOT, NNB_SPC, LT_SPC,  NUMWORD
     ),

/*
 * Layer : Symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  ^   |  {   |  }   |  $   |  %   |                              |   #  |  "   |  =   |  !   |  `   |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  ?   |  (   |  )   |  ;   |  :   |                              |   \  |  /   |  -   |  +   |  *   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  <   |  [   |  ]   |  >   |      |      |      |  |      |      |      |  '   |  &   |  |   |  ~   |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |Space |      |  |      |Space |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYMBOLS] = LAYOUT_split_3x5_3(
      PG_ACIR,    PG_LCBR, PG_RCBR, PG_DLR,  PG_PERC,                  PG_HASH, PG_DQUO, PG_EGAL,    ALGR(PG_J), PG_GRV, 
      ALGR(PG_O), PG_LPRN, PG_RPRN, PG_PVIR, PG_2PTS,                  PG_BSLS, PG_SLSH, M(PG_MOIN), PG_PLUS,    PG_ASTX,
      PG_INF,     PG_LSBR, PG_RSBR, PG_SUP,  _______,                  _______, PG_APOD, PG_ESPR,    PG_PIPE,    PG_TILD,
                                    OS_RSA,  KC_SPC,  OS_NUM,  OS_NUM, _______, OS_NUM
    ),

/*
 * Layer : Numpad
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |  -   |  +   |  €   |  %   |                              |Expos.|  °   |  =   |      |  1DK |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  4   |  3   |  2   |  1   |  :   |                              |Indice|  6   |  7   |  8   |  9   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |  H   |  5   |      |      |      |  |      |      |      |  /   |  -   |  +   |  *   |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |  .   |  0   |NbSpc |  |      |Space |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NUMPAD] = LAYOUT_split_3x5_3(
      PG_VIRG, PG_MOIN, PG_PLUS,  PG_EURO, PG_PERC,                   PG_EXP,  PG_DEG,  PG_EGAL,  _______, NUMROW, 
      KC_P4,   KC_P3,   M(KC_P2), KC_P1,   PG_POIN,                   PG_IND,  KC_P6,   M(KC_P7), KC_P8,   KC_P9,  
      _______, _______, PG_H,     KC_P5,   _______,                   _______, PG_SLSH, PG_MOIN,  PG_PLUS, PG_ASTX,
                                  _______, LT_P0  , LT_PDOT, NNB_SPC, LT_SPC,  NUMWORD
     ),

/*
 * Layer : One Dead Key
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  Œ   |  Æ   |  Û   |  Ñ   |  ‑   |                              |   –  |  “   |  ”   |  ↔   |  ←   |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  Ô   |  Â   |  Î   |  Z   |  …   |                              |Greek |  K   |Umlaut|  →   |  £   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  OÙ  |  @   |  Ï   |  H   |      |      |      |  |      |      |      |  B   |  ß   |  Ç   |  Cnl |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |  Ê   |Magic |  |   ’  |   _  |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_1DK] = LAYOUT_split_3x5_3(
       _______, _______, _______, N_TILD,  PG_X,                      PG_W,    _______, _______, _______, _______,
       _______, _______, _______, PG_Z,    _______,                   _______, PG_K,    PG_D,    _______, _______,
       OU_GRV,  PG_J,    _______, PG_H,    _______,                   _______, PG_B,    _______, PG_S,    CNL_1DK,
                                  _______, PG_ECIR, PG_AGRV, LT_APOS, PG_UNDS, OS_NUM
     ),


/*
 * Layer 3 : Symbols + function keys
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  F1  |  F2  |  F3  |  F4  |  F5  |                              |      |   ;  |   !  |   #  |   °  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |Ctrl A|Ctrl X|Ctrl V|Ctrl C| LOCK |                              | Mute |   (  |   )  |   @  |   &  |  Mute  |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  F6  |  F7  |  F8  |  F9  |  F10 |      |      |  |      |      |      |Ctrl Z|Ctrl Y|  F11 |  F12 |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  | !!!  |   ,  |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SHORTNAV] = LAYOUT_split_3x5_3(
      SEL_LINE, OS_WINM,   LGUI(PG_V), RCS(PG_V), KC_PGUP,                   PG_LPRN, C(KC_LEFT), KC_UP,      C(KC_RGHT), CAPSLOCK,
      C(PG_A),  C(PG_X),   M(C(PG_V)), C(PG_C),   KC_PGDN,                   PG_RPRN, KC_LEFT,    KC_DOWN,    KC_RIGHT,   CAPSWORD,
      SEL_WORD, KC_MUTE,   KC_F2,      C(PG_Z),   _______,                   _______, C(KC_PGUP), C(KC_PGDN), C(PG_W),    CAPSLIST,
                                       _______,   _______, _______, NAVWORD, _______, OS_WNUM
    ),

/*
 * Layer 4 : Function keys + windows management
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  F12 |  F7  |  F8  |  F9  |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  F11 |  F4  |  F5  |  F6  | LOCK |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  F10 |  F1  |  F2  |  F3  |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |au
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_FUNCAPPS] = LAYOUT_split_3x5_3(
      KC_F12,        KC_F9, KC_F8,   KC_F7,        QK_BOOT,                     _______, LSG(KC_LEFT),  LGUI(KC_UP),   LSG(KC_RIGHT),  _______,
      KC_F11,        KC_F6, KC_F5,   SFT_T(KC_F4), C(KC_PAUS),                  _______, LGUI(KC_LEFT), LGUI(KC_DOWN), LGUI(KC_RIGHT), _______,
      ALT_T(KC_F10), KC_F3, KC_F2,   KC_F1,        _______,                     _______, _______,       _______,       _______,        _______,
                                     _______,      _______,   _______, _______, _______, _______
    ),

/*
 * Layer 1 : Numbers
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |  -   |  +   |  €   |  %   |                              |Expos.|  °   |  =   |      |  1DK |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  4   |  3   |  2   |  1   |  :   |                              |Indice|  6   |  7   |  8   |  9   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |  H   |  5   |      |      |      |  |      |      |      |  /   |  -   |  +   |  *   |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |  .   |  0   |NbSpc |  |      |Space |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_FUNCTIONS] = LAYOUT_split_3x5_3(
      // S(KC_4), S(KC_3) and S(PG_EGAL) are here to give easy access to ⅔, ¾ and ≠.
       _______,  _______, _______,  _______,  KC_VOLU,                    QK_BOOT, _______,  _______,  OS_WNUM, _______, 
       P(KC_F4), KC_F3,   M(KC_F2), I(KC_F1), KC_VOLD,                    KC_NUM,  I(KC_F6), M(KC_F7), KC_F8,   P(KC_F9),
       _______,  KC_MUTE, _______,  KC_F5,    _______,                    _______, KC_F10,   KC_F11,   KC_F12,  _______, 
                                    CAPSWORD, _______,  FUNWORD, _______, _______, _______
     ),

// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};