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


uint16_t global_quick_tap_timer = 0;


// Tap-hold configuration

bool forbidden_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record) {
  switch (tap_hold_keycode) {
    case LT_REPT:
    case LT_MGC:
      return false;
  }

  // Otherwise, follow the opposite hands rule.
  return same_side_combination(tap_hold_record, other_record);
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {

    if (record->event.key.col != next_record.event.key.col) {

      // La ligne suivante n'est nécessaire que si on a besoin de doubler rapidement un caractère présent sur la moitié droite du clavier.
      // Ce n'est pas nécessaire pour l'instant, vu que les guillemets sont passés à gauche.
      //if (keycode == OS_ODK) { return true; }

      if (forbidden_chord(keycode, record, next_keycode, &next_record)) {
          // When a layer-tap key overlaps with another key on the same hand, send its base keycode.
          tap_converter(keycode, record);
      }
    }
    return false;
}

/* bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
  return !forbidden_chord(keycode, record, next_keycode, &next_record);
} */


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

  // Custom behaviour of the typo dead-key
  if (!process_odk_layer(keycode, record)) { return false; }

    // Clever keys
  if (!process_clever_keys(keycode, record)) { return false; }

    // Process all other keycodes normally
  return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t* record) {
  
  //if (os4a_layer != 0 && exit_os4a_layer) { os4a_layer_off(os4a_layer); }
  os4a_layer_exit_check();
  numword_exit_check();
  //odk_layer_exit_check(keycode);
  end_CK(record);
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
      KC_NO, PG_POIN, PG_EACU, PG_U,  PG_P,   PG_B,                                    PG_V,   PG_M,    PG_C, PG_APOS, PG_X,   KC_NO,
      KC_NO, PG_O,    PG_A,    PG_I,  PG_N,   PG_VIRG,                                 PG_G,   PG_T,    PG_S, PG_R,    PG_L,   KC_NO,
      KC_NO, PG_Q,    PG_Z,    PG_Y,  PG_H,   KC_NO,   KC_NO, KC_NO,  KC_NO,   KC_NO,  KC_NO,  PG_D,    PG_F, PG_W,    OS_ODK, KC_NO,
                               KC_NO, KC_SPC, L_OS4A,  LT_E,  LT_MGC, LT_REPT, LT_SPC, R_OS4A, KC_RGUI, KC_NO
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
      _______, _______, _______, _______, _______, _______,                                     KC_NO,   KC_RGUI, OS_WIN,  TG_APOS, KC_NO,   KC_NO,
      _______, _______, _______, _______, _______, _______,                                     TT_FA,   OS_SHFT, OS_CTRL, NUMWORD, PG_ODK,  KC_NO,
      _______, _______, _______, _______, _______, _______, _______, _______, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   OS_FA,   OS_LALT, KC_NO,
                                 _______, _______, _______, _______, _______, _______, _______, _______, KC_NO,   KC_NO
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
    [_NUMBERS] = LAYOUT(
       _______, PG_DLR, PG_MOIN, PG_PLUS, KC_7,    S(PG_ACIR),                                        PG_PERC, _______, PG_EGAL, PG_ASTX, _______, _______,
       _______, KC_4,    KC_3,    KC_2,    MT_1,    PG_CARN,                                           _______, MT_SLSH, KC_6,    KC_7,    KC_8,    _______,
       _______, _______, _______, _______, KC_5,    _______,    _______, _______,    _______, _______, _______, KC_9,    KC_6,    _______, PG_ODK,  _______,
                                  _______, _______, KC_PDOT,    KC_0   , LT_NUMWORD, LT_REPT, KC_SPC,  _______, _______, _______
     ),


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
    [_ODK] = LAYOUT(
       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
       _______, OU_GRV,  _______, _______, PG_R,    PG_3PTS,                                     _______, PG_J,    _______, PG_AROB, _______, _______,
       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, PG_K,    CNL_ODK, _______,
                                  _______, _______, _______, _______, PG_O,    _______, PG_T,    _______, _______, _______
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
    [_SHORTNAV] = LAYOUT(
      _______, KC_BSPC, LWIN(KC_TAB), LWIN(PG_V), RCS(PG_V),   KC_VOLU,                                      KC_PGUP, C(KC_LEFT), KC_UP,      C(KC_RGHT), _______, _______,
      _______, C(PG_A), C(PG_X),      C(PG_V),    SFT_T(COPY), KC_VOLD,                                      KC_PGDN, KC_LEFT,    KC_DOWN,    KC_RIGHT,   KC_F2  , _______,
      _______, KC_SPC,  KC_SPC,       KC_MUTE,    C(PG_Z),     C(PG_Y),  _______, _______, _______, _______, _______, C(KC_PGUP), C(KC_PGDN), C(PG_W),    _______, _______,
                                      _______,    _______,     _______,  _______, _______, _______, _______, _______, _______,    _______
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
      _______, ALT_T(KC_F10), KC_F3, KC_F2,   KC_F1,        _______,    _______, _______, _______, _______, _______, _______,       _______,       _______,        _______,   _______,
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