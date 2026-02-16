/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "keymap.h"


static uint16_t next_keycode;
static keyrecord_t next_record;


// Tap-hold configuration

bool approved_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record) {
/*   switch (tap_hold_keycode) {
    case LT_REPT:
    case LT_MGC:
      return true;
  } */

  // Otherwise, follow the opposite hands rule.
  return bilateral_combination(tap_hold_record, other_record);
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT_E:
            return 300;
        default:
            return TAPPING_TERM;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Desabling Permissive Hold for `E` when the other key is on the same side
        case LT_E:
            if (!approved_chord(keycode, record, next_keycode, &next_record)) { return false; }
        default:
            return true;
    }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {

    // Permet de doubler rapidement un caractère présent sur la moitié droite du clavier.
    if (keycode == OS_1DK) { return true; }
    return false;
}

const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
    LAYOUT_split_3x6_3(
        'L', 'L', 'L', 'L', 'L', 'L',                     'R', 'R', 'R', 'R', 'R', 'R', 
        'L', 'L', 'L', 'L', 'L', 'L',                     'R', 'R', 'R', 'R', 'R', 'R', 
        'L', 'L', 'L', 'L', 'L', 'L',                     'R', 'R', 'R', 'R', 'R', 'R', 
                            '*', '*', '*',           '*', 'R', '*'
    );


// Housekeeping

void housekeeping_task_user(void) {
  recent_keys_task();
  modword_task();
  layerword_task();
}


// Key processing

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {

  if (record->event.pressed) {
      // Cache the next input for mod-tap decisions
      next_keycode = keycode;
      next_record  = *record;
  }
  return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  
  // LT Repeat and Magic keys
  if (!process_macros_I(keycode, record)) { return false; }

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
    [_BASE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      MOD_ESC, PG_VIRG, PG_EACU,    PG_U,    PG_P,    PG_B,                         PG_V,    PG_M,    PG_C,    PG_J,    PG_X, MOD_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      MOD_TAB,   HRM_O,   HRM_A,   HRM_I,   HRM_N, PG_POIN,                         PG_G,   HRM_T,   HRM_S,   HRM_R,   HRM_L, MOD_BKP,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      MOD_STB,    PG_Q, PG_EGRV,    PG_Y,   HRM_H, PG_TIRE,                         PG_K,   HRM_D,    PG_F,    PG_W,  OS_1DK, MOD_ENT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          OS_LSFT,    LT_E,  LT_MGC,    LT_REPT, LT_RSPC,  LT_SYM
                                      //`--------------------------'  `--------------------------'

  ),

    [_SHORTNAV] = LAYOUT_split_3x6_3(
  //,-------------------------------------------------------.                    ,-----------------------------------------------------------.
      _______, KC_BSPC, KC_HOME,   KC_UP,  KC_END,   KC_PGUP,                      CAPSLOCK,  KC_DEL,  C(KC_DEL),  XXXXXXX,  XXXXXXX, _______,
  //|--------+--------+--------+--------+--------+----------|                    |---------+--------+-----------+---------+---------+--------|
      _______, C(PG_A), KC_LEFT, KC_DOWN, KC_RGHT,   KC_PGDN,                      CAPSWORD, KC_BSPC, C(KC_BSPC),   KC_ENT,   KC_ESC, _______,
  //|--------+--------+--------+--------+--------+----------|                    |---------+--------+-----------+---------+---------+--------|
      _______, C(PG_Z), C(PG_X), C(PG_C), C(PG_V), LCS(PG_Z),                      CAPSLIST,  KC_TAB,  S(KC_TAB), SEL_LINE, SEL_WORD, _______,
  //|--------+--------+--------+--------+--------+----------+--------|  |--------+---------+--------+-----------+---------+---------+--------|
                                          _______,   LT_LSPC, KC_BSPC,    NAVWORD, KC_BSPC,  _______
                                      //`----------------------------'  `---------------------------'
  ),

    [_NUMBERS] = LAYOUT_split_3x6_3(
  //,-------------------------------------------------------.                    ,-----------------------------------------------------.
      _______,  PG_DLR, _______, _______, PG_EURO,   PG_PERC,                       PG_EXP,  PG_DEG, PG_EGAL, _______,  PG_1DK, _______,
  //|--------+--------+--------+--------+--------+----------|                    |--------+--------+--------+--------+--------+--------|
      _______,    KC_8,    KC_7,    MT_6,    MT_5,   PG_2PTS,                       PG_IND,    MT_1,    MT_2,    KC_3,    KC_4, _______,
  //|--------+--------+--------+--------+--------+----------|                    |--------+--------+--------+--------+--------+--------|
      _______, XXXXXXX, XXXXXXX, XXXXXXX,    KC_9,   XXXXXXX,                      XXXXXXX, PG_SLSH, PG_MOIN, PG_PLUS, PG_ASTX, _______,
  //|--------+--------+--------+--------+--------+----------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, S(KC_SPC), KC_BSPC,    NUMWORD,    LT_0, KC_PDOT
                                      //`----------------------------'  `--------------------------'
  ),

    [_FUNCTIONS] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      QK_BOOT, KC_F1,     KC_F2,   KC_F3,   KC_F4, XXXXXXX,                      XXXXXXX, KC_BRID, KC_BRIU, KC_SLEP, XXXXXXX, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_NUM, KC_F5,     KC_F6,   KC_F7,   KC_F8, XXXXXXX,                      XXXXXXX, KC_VOLD, KC_VOLU, KC_MUTE, XXXXXXX, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_SCRL, KC_F9,    KC_F10,  KC_F11,  KC_F12, XXXXXXX,                      XXXXXXX, KC_MPRV, KC_MNXT, KC_MPLY, XXXXXXX, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    FUNWORD, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

    [_SYMBOLS] = LAYOUT_split_3x6_3(
  //,--------------------------------------------------------.                    ,--------------------------------------------------------.
      _______, PG_ACIR,    PG_LCBR, PG_RCBR, PG_DLR,  PG_PERC,                      PG_HASH, PG_DQUO, PG_EGAL, ALGR(PG_J),  PG_GRV, _______,
  //|--------+-----------+--------+--------+--------+--------|                    |--------+--------+--------+-----------+--------+--------|
      _______, ALGR(PG_O), PG_LPRN, PG_RPRN, PG_PVIR, PG_2PTS,                      PG_BSLS, MT_SLSH, PG_MOIN, PG_PLUS,    PG_ASTX, _______,
  //|--------+-----------+--------+--------+--------+--------|                    |--------+--------+--------+-----------+--------+--------|
      _______, PG_INF,     PG_LSBR, PG_RSBR, PG_SUP,  _______,                      _______, PG_APOD, PG_ESPR, PG_PIPE,    PG_TILD, _______,
  //|--------+-----------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+-----------+--------+--------|
                                              OS_RSA,  KC_SPC, KC_BSPC,    _______, _______, _______
                                         //`--------------------------'  `--------------------------'
  ),

    [_1DK] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, _______, _______, _______, _______,    PG_X,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______,    PG_Z, _______,                      _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, _______,    PG_J, _______, _______, _______,                      _______, _______, _______, _______, CNL_1DK, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, PG_ECIR, PG_AGRV,    PG_APOS, PG_UNDS, _______
                                      //`--------------------------'  `--------------------------'
  )
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
  [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
  [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
  [2] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
  [3] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(KC_RGHT, KC_LEFT), },
};
#endif
