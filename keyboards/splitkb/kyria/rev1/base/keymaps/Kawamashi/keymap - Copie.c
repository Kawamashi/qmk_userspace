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

bool isLetter(uint16_t keycode) {
  switch (keycode) {
    case KC_A ... KC_L:
    case FR_M:
    case KC_N ... KC_Z:
    case FR_AGRV:
    case FR_EACU:
    case FR_EGRV:
    case FR_CCED:
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
  if (isLetter(keycode)) {
    add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
    return true;
  } else {
    switch (keycode) {
        // Keycodes that continue Caps Word, without shifting.
        case FR_MOIN:
        case KC_KP_1 ... KC_KP_0:
        case KC_LEFT:
        case KC_RIGHT:
        case KC_BSPC:
        case KC_DEL:
        case FR_APOS:
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
  {FR_AGRV, FR_AE},
  {FR_B, FR_SS},
  {FR_A, FR_LDAQ},
  {FR_I, FR_RDAQ},
  {FR_T, FR_ESPR},
  {FR_S, FR_AROB},
  {ALGR_T(FR_A), FR_LDAQ},
  {LCTL_T(FR_I), FR_RDAQ},
  {RCTL_T(FR_T), FR_ESPR},
  {ALGR_T(FR_S), FR_AROB},
  {FR_X, FR_TM},
  {KC_KP_8, FR_INFN},
  {FR_H, FR_HASH},
  {FR_V, FR_DEG},
  {FR_CCED, FR_CEDL},
  {FR_Q, FR_SECT},
  {FR_POIN, FR_PVIR},
  {FR_C, FR_COPY},
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
    case LT_CCED:   // Pour pouvoir faire Alt + F4, Alt + F11.
    case LCTL_T(FR_I):
    case RCTL_T(FR_T):
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
      KC_ESC, FR_AGRV,     FR_B,        FR_EACU,      FR_POIN,       FR_MOIN,                                                               FR_ACIR,       FR_V,         FR_L,         FR_M,        FR_X,         FR_W,
      KC_ENT, ALT_T(FR_O), SFT_T(FR_U), ALGR_T(FR_A), LCTL_T(FR_I),  FR_J,                                                                  FR_G,          RCTL_T(FR_T), ALGR_T(FR_S), RSFT_T(FR_N), ALT_T(FR_R), FR_F,
      LT_TAB, FR_Q,        FR_Y,        FR_EGRV,      LWIN_T(FR_P),  FR_DQUO,       KC_BSPC,          KC_END,  KC_HOME, KC_DEL,             FR_K,          RWIN_T(FR_D), FR_Z,         FR_H,        FR_C,         LT_CCED,
                                        TG(_SYMNUM),  KC_SPC,        OSM(MOD_LSFT), LT(_SYMNUM,FR_E), LT_VIRG, LT_APOS, LT(_SYMNUM,KC_SPC), OSM(MOD_RSFT), KC_RGUI,      KC_MUTE
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
    [_SYMNUM] = LAYOUT(
      KC_ESC,  FR_EXLM, FR_QUES,        FR_ESPR,         FR_PVIR, FR_PIPE,                                       FR_MOIN, KC_P7,         KC_P8,         KC_P9, FR_ASTX, KC_NUM,
      _______, FR_LACL, SFT_T(FR_RACL), ALGR_T(FR_LPRN), FR_RPRN, LAYER_LCK,                                     FR_EGAL, RCTL_T(KC_P4), ALGR_T(KC_P5), KC_P6, FR_SLSH, FR_BSLS,
      _______, FR_LBKT, FR_RBKT,        FR_INF,          FR_SUP,  FR_CARN,   _______, _______, _______, _______, FR_ACIR, KC_P1,         KC_P2,         KC_P3, FR_PLUS, FR_PERC,
                                        _______,         _______, _______,   KC_SPC,  FR_VIRG, NUMWORD, KC_P0,   KC_PDOT, _______      , _______
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
      KC_ESC,  A(KC_F4), ALT_TAB,      LWIN(FR_V), _______, _______,                                           _______, C(KC_LEFT),      KC_UP,   C(KC_RIGHT), _______, _______,
      _______, C(FR_A),  C(FR_X),      C(FR_V),    C(FR_C), LAYER_LCK,                                         KC_MUTE, RCTL_T(KC_LEFT), KC_DOWN, KC_RIGHT,    KC_F2  , KC_MUTE,
      _______, SELWORD,  LWIN(KC_TAB), REV_TAB,    ALT_TAB, _______,  _______, S(KC_END), S(KC_HOME), _______, _______, C(FR_Z),         C(FR_Y), _______,     _______, _______,
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
      _______, KC_F10,        KC_F3,         KC_F2,   KC_F1,   _______,    _______, _______, _______, _______, _______, LWIN(FR_D),    LWIN(KC_HOME), _______,        _______, _______,
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