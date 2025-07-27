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
#include "features/layer_lock.h"


bool is_caps_lock_on(void) { return host_keyboard_led_state().caps_lock; }

bool is_letter(uint16_t keycode) {
  switch (keycode) {
    case KC_A ... KC_L:
    case PG_M:
    case KC_N ... KC_Z:
    case PG_AGRV:
    case PG_U:
    case PG_EGRV:
    case PG_CCED:
      return true;

    default:
      return false;
  }
}

// Achordion

uint16_t achordion_timeout(uint16_t tap_hold_keycode) { return 500; }

bool achordion_eager_mod(uint8_t mod) {
  switch (mod) {
    case MOD_LSFT:
    case MOD_RSFT:
    case MOD_LCTL:
    case MOD_RCTL:
      return true;  // Eagerly apply Shift and Ctrl mods.

    default:
      return false;
  }
}


// Caps Word

bool caps_word_press_user(uint16_t keycode) {
  // Keycodes that continue Caps Word, with shift applied.
  if (is_letter(keycode)) {
    add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
    return true;
  } else {
    switch (keycode) {
        // Keycodes that continue Caps Word, without shifting.
        case PG_MOIN:
        case KC_KP_1 ... KC_KP_0:
        case KC_LEFT:
        case KC_RIGHT:
        case KC_BSPC:
        case KC_DEL:
        case PG_APOS:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
  }
}

// Combo
combo_t key_combos[] = {};
uint16_t COMBO_LEN = 0;


// Custom altGr keys

const custom_altgr_key_t custom_altgr_keys[] = {
  {PG_AGRV, PG_AE},
  {PG_B, PG_TS},
  {PG_A, PG_CDAQ},
  {PG_I, PG_RDAQ},
  {PG_N, PG_ESPR},
  {PG_T, PG_AROB},
  {ALGR_T(PG_A), PG_CDAQ},
  {LCTL_T(PG_I), PG_RDAQ},
  {RCTL_T(PG_N), PG_ESPR},
  {ALGR_T(PG_T), PG_AROB},
  {PG_POIN, PG_NM},
  {KC_KP_8, PG_INFN},
  {PG_F, PG_HASH},
  {PG_G, PG_HEG},
  {PG_CCED, PG_DEDL},
  {PG_Q, PG_TECT},
  {PG_X, PG_PVIR},
  {PG_D, PG_DOPY},
};

uint8_t NUM_CUSTOM_ALTGR_KEYS =
    sizeof(custom_altgr_keys) / sizeof(custom_altgr_key_t);


void matrix_scan_user(void) {
  //achordion_task();
  recent_keys_task();
  swapper_task();
}

// Tap-hold configuration

// Handle keyrecord before quantum processing
static uint16_t next_keycode;
static keyrecord_t next_record;

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t prev_keycode;
    static bool tap_condition;

    if (record->event.pressed) {
        // Store the previous keycode for instant tap decision
        prev_keycode = next_keycode;
        // Cache the next input for mod-tap decisions
        next_keycode = keycode;
        next_record  = *record;
    }

    // Match mod-tap keys. Tweak this to limit conditions that matches your keyboard and habits.
    tap_condition = ((IS_LAYER_ON(_BASE)) && IS_QK_MOD_TAP(keycode) && !IS_QK_LAYER_TAP(prev_keycode) && !is_mod_tap_control(prev_keycode));
    return process_instant_tap(keycode, record, prev_keycode, tap_condition);
}

bool forbidden_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record) {
  switch (tap_hold_keycode) {
    case LT_VIRG:
    case OSM(MOD_LSFT):
    case OSM(MOD_RSFT):
      return false;   

    default:
      // Otherwise, follow the opposite hands rule.
      return same_side_combination(tap_hold_record, other_record);
  }
}

bool first_of_chorded_mods(uint16_t keycode) {
  switch (keycode) {
    case LT_TAB:    // Pour pouvoir faire OSM shift + LT_TAB (win + shift + flèche).
    case PG_CCED:   // Pour pouvoir faire Alt + F4, Alt + F11.
    case LCTL_T(PG_I):
    case RCTL_T(PG_N):
    case OSM(MOD_LSFT):   // Pour pouvoir faire OSM shift + LT_TAB (win + shift + flèche).
      return true;

    default:
      return false;
  }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {

    if (record->event.key.col != next_record.event.key.col) {
      // Sinon on a des effets de bord quand on veut taper des chiffres. 
      if (IS_LAYER_ON(_BASE)) {
        // When a mod-tap key overlaps with another non-Ctrl key on the same hand, send its base keycode
        if (forbidden_chord(keycode, record, next_keycode, &next_record) && !first_of_chorded_mods(keycode)) {
          tap_converter(keycode, record);
        }
      }
    }
    return false;
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {

    if (forbidden_chord(keycode, record, next_keycode, &next_record)) {
      tap_converter(keycode, record);
      return false;
    }
    return true;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // Achordion
  //if (!process_achordion(keycode, record)) { return false; }
  
  // Custom alt gr
  if (!process_custom_altgr_keys(keycode, record)) { return false; }

    // Recent keys
  if (!process_clever_keys(keycode, record)) { return false; }

    // Numword
  if (!process_numword(keycode, record)) { return false; }

    // Layer lock
  if (!process_layer_lock(keycode, record, LAYER_LCK)) { return false; }

  // Select word
  if (!process_select_word(keycode, record, SELWORD)) { return false; }

    // Macros
  if (!process_macros(keycode, record)) { return false; }

  return true; // Process all other keycodes normally
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: ALPHAS
 *»\
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * | Helpsc |   À  |   B  |   É  |   .  |   -  |                              |   ^  |   V  |   L  |   M  |   X  |    W   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|0
 * |  Enter |   O  |   U  |A/AltG|I/Ctrl|   J  |                              |   G  |   T  |   S  |   N  |   R  |    F   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |  Tab   |   Q  |   Y  |   È  |P/Win |  ""  | Bksp | End  |  | Home |Delete|   K  |   D  |   Z  |   H  |   C  | Ç/Alt  |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |NavNum| Space|Shift |   E  | ,    |  |  '   | Space|Shift| Win  | Mute |
 *                        |      |      |      |NavNum|Symb. |  |Funct.|NavNum|     |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_BASE] = LAYOUT(
      KC_ESC, PG_AGRV,     PG_B,        PG_U,      PG_X,       PG_MOIN,                                                               PG_ACIR,       PG_G,         PG_C,         PG_M,        PG_POIN,         PG_W,
      KC_ENT, ALT_T(PG_O), SFT_T(PG_L), ALGR_T(PG_A), LCTL_T(PG_I),  PG_J,                                                                  PG_VIRG,          RCTL_T(PG_N), ALGR_T(PG_T), RSFT_T(PG_S), ALT_T(PG_R), PG_V,
      LT_TAB, PG_Q,        PG_Y,        PG_EGRV,      LWIN_T(PG_P),  PG_HQUO,       KC_BSPC,          KC_END,  KC_HOME, KC_DEL,             PG_K,          RWIN_T(PG_H), PG_Z,         PG_F,        PG_D,         PG_CCED,
                                        TG(_SYMBOLS),  KC_SPC,        OSM(MOD_LSFT), LT(_SYMBOLS,PG_E), LT_VIRG, LT_APOS, LT(_SYMBOLS,KC_SPC), OSM(MOD_RSFT), KC_RGUI,      KC_MUTE
    ),

	  
/*
 * Layer 1 : Numpad + symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * | Helpsc |  !   |  ?   |  &   |  ;   |  |   |                              |   ⁻  |  7   |  8   |  9   |  *   |NumLock |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  {   |  }   |  (   |  )   | LOCK |                              |   =  |  4   |  5   |  6   |  /   |   \    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  [   |  ]   |  <   |  >   |Indice|      |      |  |      |      |Expos.|  1   |  2   |  3   |  +   |   %    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |   0  |   .  |      |      |
 *                        |      |      |      |      |      |  |   ,  |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYMBOLS] = LAYOUT(
      KC_ESC,  PG_EXCL, PG_QUES,        PG_ESPR,         PG_PVIR, PG_PIPE,                                       PG_MOIN, KC_P7,         KC_P8,         KC_P9, PG_ASTX, KC_NUM,
      _______, PG_CACL, SFT_T(PG_RACL), ALGR_T(PG_LPRN), PG_RPRN, LAYER_LCK,                                     PG_EGAL, RCTL_T(KC_P4), ALGR_T(KC_P5), KC_P6, PG_TLSH, PG_BSLS,
      _______, PG_CBKT, PG_RBKT,        PG_INF,          PG_SUP,  PG_CARN,   _______, _______, _______, _______, PG_ACIR, KC_P1,         KC_P2,         KC_P3, PG_PLUS, PG_PERC,
                                        _______,         _______, _______,   KC_SPC,  PG_EACU, NUMWORD, KC_P0,   KC_PDOT, _______      , _______
    ),
/*
 * Layer 2 : Symbols + function keys
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * | Helpsc |  F1  |  F2  |  F3  |  F4  |  F5  |                              |      |   ;  |   !  |   #  |   °  |        |
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
      KC_ESC,  A(KC_F4), ALT_TAB,      LWIN(PG_G), _______, _______,                                           _______, C(KC_LEFT),      KC_UP,   C(KC_RIGHT), _______, _______,
      _______, C(PG_A),  C(PG_POIN),      C(PG_G),    C(PG_D), LAYER_LCK,                                         KC_MUTE, RCTL_T(KC_LEFT), KC_DOWN, KC_RIGHT,    KC_F2  , KC_MUTE,
      _______, SELWORD,  LWIN(KC_TAB), REV_TAB,    ALT_TAB, _______,  _______, S(KC_END), S(KC_HOME), _______, _______, C(PG_Z),         C(PG_Y), _______,     _______, _______,
                                       _______,    _______, _______,  QUES_PT, QUES_PT,   EXCL_PT,    EXCL_PT, _______, _______,         _______
    ),

/*
 * Layer 3 : Function keys + windows management
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  F12 |  F7  |  F8  |  F9  |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  F11 |  F4  |  F5  |  F6  | LOCK |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  F10 |  F1  |  F2  |  F3  |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_FUNCAPPS] = LAYOUT(
      KC_ESC,  KC_F12,        KC_F9,         KC_F8,   KC_F7,   C(KC_PAUS),                                     _______, SWIN(KC_LEFT), LWIN(KC_UP),   SWIN(KC_RIGHT), _______, QK_BOOT,
      _______, ALT_T(KC_F11), SFT_T(KC_F6),  KC_F5,   KC_F4,   LAYER_LCK,                                      _______, RCTL_T(FEN_G), LWIN(KC_DOWN), LWIN(KC_RIGHT), _______, _______,
      _______, KC_F10,        KC_F3,         KC_F2,   KC_F1,   _______,    _______, _______, _______, _______, _______, LWIN(PG_H),    LWIN(KC_HOME), _______,        _______, _______,
                                             _______, _______, _______,    _______, _______, _______, _______, _______, _______,       _______
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