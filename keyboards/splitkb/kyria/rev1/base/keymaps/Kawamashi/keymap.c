/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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


uint16_t global_quick_tap_timer = 0;


// Tap-hold configuration

bool forbidden_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record) {
  switch (tap_hold_keycode) {
    case LT_MGC:
      return false;
  }

  // Otherwise, follow the opposite hands rule.
  return same_side_combination(tap_hold_record, other_record);
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {

    if (record->event.key.col != next_record.event.key.col) {

      // J'avais mis cette ligne pour faire les "". Je pense que ce n'est plus nécessaire maintenant que que 
      //if (keycode == OS_ACC) { return true; }

      if (forbidden_chord(keycode, record, next_keycode, &next_record)) {
          // When a layer-tap key overlaps with another key on the same hand, send its base keycode.
          tap_converter(keycode, record);
      }
    }
    return false;
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
  return !forbidden_chord(keycode, record, next_keycode, &next_record);
}


// Matrix scan

void matrix_scan_user(void) {
  recent_keys_task();
  //swapper_task();
}


// Key processing

uint16_t next_keycode;
keyrecord_t next_record;

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {
        // Cache the next input for mod-tap decisions
        next_keycode = keycode;
        next_record  = *record;
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // Global quick tap for combos.
  // IS_KEYEVENT prevents combos from updating global_quick_tap_timer, to allow combos to be chained.
  if ((IS_KEYEVENT(record->event) && get_highest_layer(layer_state) == _BASE) && !IS_OS4A_KEY(keycode)) {
    global_quick_tap_timer = timer_read();
  }

  // Multi One-Shot Mods
  if (!process_mods(keycode, record)) { return false; }

  // Numword
  if (!process_numword(keycode, record)) { return false; }

  // Custom tap-hold keys
  if (!process_custom_tap_hold(keycode, record)) { return false; }

  // Macros
  if (!process_macros(keycode, record)) { return false; }
  
  // Custom alt gr
  if (!process_custom_altgr_keys(keycode, record)) { return false; }

    // Clever keys
  if (!process_clever_keys(keycode, record)) { return false; }

    // Process all other keycodes normally
  return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t* record) {
  // 
  if (os4a_layer != 0 && exit_os4a_layer) {
    os4a_layer_off(os4a_layer);
  }
  if (exit_num_word) {
    disable_num_word();
  }
}


// Keymap

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: ALPHAS
 *»\
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |   X  |   È  |   É  |   .  |   K  |                              |   V  |   B  |   L  |   M  |   X  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |  Enter |   O  |   U  |   A  |   I  |   J  |                              |   G  |   T  |   S  |   N  |   R  |    F   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |  Tab   |   Q  |   Z  |   W  |   P  |      |      |      |  |      |      |      |   D  |   Y  |   H  |   C  |    Ç   |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |Shift |   E  | ,    |  |  '   | Space| Shift|      |      |
 *                        |      |      |      |NavNum|Symb. |  |Funct.|NavNum|      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_BASE] = LAYOUT(
      KC_NO, FR_X,  FR_VIRG, FR_EACU, FR_P,   FR_B,                                   FR_F,   FR_M,    FR_L, FR_APOS, FR_POIN, KC_NO,
      KC_NO, FR_O,  FR_A,    FR_I,    FR_T,   FR_G,                                   FR_V,   FR_S,    FR_N, FR_R,    FR_U,    KC_NO,
      KC_NO, FR_Q,  FR_Z,    FR_Y,    LT_D,   KC_NO,  KC_NO, KC_NO,  KC_NO,   KC_NO,  KC_NO,  LT_C,    FR_H, FR_W,    OS_ACC,  KC_NO,
                             KC_NO,   KC_SPC, L_OS4A, LT_E,  LT_MGC, LT_REPT, LT_SPC, R_OS4A, KC_RGUI, KC_NO
    ),

/*
 * Layer 1 : Mods gauche
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  !   |  ?   |  &   |  ;   |  |   |                              |   ⁻  |  7   |  8   |  9   |  *   |NumLock |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  {   |  }   |  (   |  )   | LOCK |                              |   =  |  4   |  5   |  6   |  /   |   \    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  [   |  ]   |  <   |  >   |Indice|      |      |  |      |      |Expos.|  1   |  2   |  3   |  +   |   %    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |   0  |   .  |      |      |
 *                        |      |      |      |      |      |  |   ,  |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_L_MODS] = LAYOUT(
      KC_NO, KC_NO,   KC_NO,  OS_WIN,  KC_RGUI, KC_NO,                                        _______, _______, _______, _______, _______, _______,
      KC_NO, OS_RALT, OS_FA,  OS_CTRL, OS_SHFT, KC_NO,                                        _______, _______, _______, _______, _______, _______,
      KC_NO, OS_LALT, KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   _______,  _______, _______, _______, _______, _______, _______, _______,
                              KC_NO,   KC_NO,   _______, _______, _______, CAPSWORD, _______, KC_CAPS, _______, _______
    ),

/*
 * Layer  : Mods droite
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  !   |  ?   |  &   |  ;   |  |   |                              |   ⁻  |  7   |  8   |  9   |  *   |NumLock |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  {   |  }   |  (   |  )   | LOCK |                              |   =  |  4   |  5   |  6   |  /   |   \    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  [   |  ]   |  <   |  >   |Indice|      |      |  |      |      |Expos.|  1   |  2   |  3   |  +   |   %    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |   0  |   .  |      |      |
 *                        |      |      |      |      |      |  |   ,  |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_R_MODS] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     KC_NO,   KC_RGUI, OS_WIN,  KC_NO,  KC_NO,   KC_NO,
      _______, _______, _______, _______, _______, _______,                                     TT_FA,   OS_SHFT, OS_CTRL, NUMWORD, OS_RALT, KC_NO,
      _______, _______, _______, _______, _______, _______, _______, _______, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   OS_FA,   OS_LALT, KC_NO,
                                 _______, _______, _______, _______, _______, _______, _______, _______, KC_NO,   KC_NO
    ),

/*
 * Layer 3 : Numpad + symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |  &   |  ;   |  |   |  ^   |                              |   ⁻  |  7   |  8   |  9   |  *   |NumLock |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  {   |  }   |  (   |  )   |  ˇ   |                              |   =  |  4   |  5   |  6   |  /   |   \    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  [   |  ]   |  <   |  >   |      |      |      |  |      |      |      |  1   |  2   |  3   |  +   |   %    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |   0  |   .  |      |      |
 *                        |      |      |      |      |      |  |   ,  |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYMNUM] = LAYOUT(
      KC_NO,   FR_PIPE,         FR_ESPR, FR_UNDS, FR_PVIR,        FR_ACIR,                                     FR_MOIN, KC_P7,   KC_P8,  KC_P9, FR_ASTX,         _______,
      _______, ALGR_T(FR_LACL), FR_RACL, FR_LPRN, SFT_T(FR_RPRN), FR_CARN,                                     FR_EGAL, KC_P4,   KC_P5,  KC_P6, ALGR_T(FR_SLSH), _______,
      _______, FR_LBKT,         FR_RBKT, FR_INF,  FR_SUP,         KC_NO,   _______, _______, _______, _______, KC_NO,   KC_P1,   KC_P2,  KC_P3, FR_PLUS,         _______,
                                         _______, _______,        _______, KC_SPC,  LT_VIRG, NUMWORD, KC_P0,   KC_PDOT, _______, _______
    ),

/*
 * Layer 1 : Numpad
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  !   |  ?   |  &   |  ;   |  |   |                              |   ⁻  |  7   |  8   |  9   |  *   |NumLock |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  {   |  }   |  (   |  )   | LOCK |                              |   =  |  4   |  5   |  6   |  /   |   \    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  [   |  ]   |  <   |  >   |Indice|      |      |  |      |      |Expos.|  1   |  2   |  3   |  +   |   %    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |   0  |   .  |      |      |
 *                        |      |      |      |      |      |  |   ,  |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
/*     [_NUMPAD] = LAYOUT(
      _______, _______, _______, _______, _______, FR_ACIR,                                     FR_MOIN, KC_P7,   KC_P8,  KC_P9, FR_ASTX, _______,
      OSM(MOD_LGUI),  OSM(MOD_LALT), OSM(MOD_RSFT), OSM(MOD_RALT), OSM(MOD_LCTL), FR_CARN,                                     FR_EGAL, KC_P4,   KC_P5,  KC_P6, FR_SLSH, FR_BSLS,
      _______, KC_SPC,  _______, _______, _______, KC_NO,   _______, _______, _______, _______, KC_NO  , KC_P1,   KC_P2,  KC_P3, FR_PLUS, _______,
                                 _______, _______, _______, KC_SPC , FR_VIRG, NUMWORD, KC_P0,   KC_PDOT, _______, _______
    ), */

/*
 * Layer 2 : Symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  !   |  ?   |  &   |  ;   |  |   |                              |   ⁻  |  7   |  8   |  9   |  *   |NumLock |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  {   |  }   |  (   |  )   |Expos.|                              |   =  |  4   |  5   |  6   |  /   |   \    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  [   |  ]   |  <   |  >   |Indice|      |      |  |      |      |      |  1   |  2   |  3   |  +   |   %    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |   0  |   .  |      |      |
 *                        |      |      |      |      |      |  |   ,  |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
/*     [_SYMBOLS] = LAYOUT(
      _______, _______, _______, FR_DQUO, FR_PVIR, FR_ACIR,                                     _______, KC_HOME, KC_UP,   KC_END,           _______, _______,
      _______, FR_LACL, FR_RACL, FR_LPRN, FR_RPRN, FR_CARN,                                     _______, KC_LEFT, KC_DOWN, ALGR_T(KC_RIGHT), KC_F2  , KC_MUTE,
      _______, FR_LBKT, FR_RBKT, FR_INF,  FR_SUP,  KC_NO,   _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
                                 _______, _______, _______, KC_SPC,  FR_VIRG, NUMWORD, KC_P0,   KC_PDOT, _______, _______
    ), */

/*
 * Layer 2 : Symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  !   |  ?   |  &   |  ;   |  |   |                              |   ⁻  |  7   |  8   |  9   |  *   |NumLock |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  {   |  }   |  (   |  )   |Expos.|                              |   =  |  4   |  5   |  6   |  /   |   \    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  [   |  ]   |  <   |  >   |Indice|      |      |  |      |      |      |  1   |  2   |  3   |  +   |   %    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |   0  |   .  |      |      |
 *                        |      |      |      |      |      |  |   ,  |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ACCENTS] = LAYOUT(
      _______, O_CIRC,  A_CIRC,  I_CIRC,  U_CIRC,  FR_LDAQ   ,                                      FR_RDAQ, FR_TREM, FR_GRV,  FR_ACIR, FR_CEDL,   _______,
      _______, OU_GRV,  FR_AGRV, FR_DQUO, FR_MOIN, ALGR(FR_G),                                      _______, FR_J,    _______, FR_AROB, _______,   _______,
      _______, FR_OE,   FR_AE,   I_TREM,  CA_CED,  _______   , _______, _______, _______, _______,  _______, FR_CCED, FR_TREM, FR_K,    TG_ACC,    _______,
                                 _______, _______, _______   , FR_EGRV, E_CIRC,  _______, AGRV_SPC, _______, _______, _______
    ),

/*     [_ACCENTS] = LAYOUT(
      _______, _______, _______, _______, _______, FR_LDAQ   ,                                      FR_RDAQ, U_CIRC,  I_CIRC,  A_CIRC, O_CIRC, _______,
      _______, _______, FR_DQUO, _______, FR_J,    ALGR(FR_G),                                      _______, FR_AROB, FR_MOIN, FR_K,   OU_GRV, _______,
      _______, _______, FR_AGRV, I_TREM, CA_CED,   _______   , _______, _______, _______, _______,  _______, FR_CCED, FR_UNDS, FR_AE,  FR_OE,  _______,
                                 _______, _______, _______   , FR_EGRV, E_CIRC,  _______, AGRV_SPC, _______, _______, _______
    ), */

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
    [_SHORTNAV] = LAYOUT(
      _______, KC_BSPC, LWIN(KC_TAB), LWIN(FR_V), RCS(FR_V),   _______,                                           KC_PGUP, C(KC_LEFT), KC_UP,   C(KC_RGHT), _______, _______,
      _______, C(FR_A), C(FR_X),      C(FR_V),    SFT_T(COPY), _______,                                           KC_PGDN, KC_LEFT,    KC_DOWN, KC_RIGHT,   KC_F2  , _______,
      _______, KC_SPC,  KC_SPC,       KC_SPC,     C(FR_Z),     C(FR_Y),  _______, S(KC_END), S(KC_HOME), _______, _______, KC_VOLD,    KC_VOLU, KC_MUTE,    _______, _______,
                                      _______,    _______,     _______,  _______, _______,   _______,    _______, _______, _______,    _______
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
    [_FUNCAPPS] = LAYOUT(
      _______, KC_F12,        KC_F9, KC_F8,   KC_F7,        QK_BOOT,                                        _______, SWIN(KC_LEFT), LWIN(KC_UP),   SWIN(KC_RIGHT), KC_NUM,    _______,
      _______, KC_F11,        KC_F6, KC_F5,   SFT_T(KC_F4), C(KC_PAUS),                                     TT_FA,   LWIN(KC_LEFT), RCTL_T(FEN_B), LWIN(KC_RIGHT), A(KC_ESC), _______,
      _______, ALT_T(KC_F10), KC_F3, KC_F2,   KC_F1,        _______,    _______, _______, _______, _______, _______, C(KC_PGUP),    A(KC_ESC),     C(KC_PGDN),     _______,   _______,
                                     _______, _______,      _______,    _______, _______, _______, _______, _______, _______,       _______
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