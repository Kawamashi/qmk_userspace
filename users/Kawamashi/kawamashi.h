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

#pragma once

#include QMK_KEYBOARD_H

#include "keymap_french_propergol.h"
#include "features/modword.h"
#include "../../modules/Kawamashi/layer_word/layer_word.h"
#include "../../modules/Kawamashi/oneshots_on_steroids/oneshots_on_steroids.h"
#include "../../modules/Kawamashi/clever_keys/clever_keys.h"


enum layers {
    _BASE = 0,
    _1DK,
    _NUMROW,
    _SYMBOLS,
    _SHORTNAV,
    _NUMPAD,
    _FUNCAPPS,
    _FUNCTIONS
};

enum custom_keycodes {
  NUMPAD = SAFE_RANGE,
  NUMROW,
  NAVWORD,
  FUNWORD,
  CAPSWORD,
  CAPSLIST,
  CAPSLOCK,
  SEL_WORD,
  SEL_LINE,
  OS_WINM,
  OS_WNUM,
  A_CIRC,
  U_CIRC,
  N_TILD,
  MAGIC,
  LETTER_1DK,
  SYMBOL_1DK,
  PG_DEG,
};

  // Layer changers
#define LT_SPC LT(_SYMBOLS, KC_SPC)
#define LT_E LT(_SYMBOLS, PG_E)
#define LT_REPT LT(_FUNCTIONS, KC_1)
#define LT_MGC LT(_SHORTNAV, KC_1)
#define LT_NUMW LT(_NUMROW, KC_2)
#define LT_0 LT(_SYMBOLS, KC_0)
#define LT_P0 LT(_SYMBOLS, KC_P0)
#define LT_PDOT LT(_SHORTNAV, KC_PDOT)

//#define OS_1DK OSL(_1DK)
#define OS_SHFT KC_RCTL
#define OS_1DK MO(_1DK)
#define OS_NUM OSL(_NUMPAD)


  // HRM
#ifdef KRYPTON_ENABLE_HRM
  #define P(k) SFT_T(k)
  #define R(k) LALT_T(k)
  #define M(k) LGUI_T(k)
  #define I(k) LCTL_T(k)
#else
  #define P(k) k
  #define R(k) k
  #define M(k) k
  #define I(k) k
#endif

#define P_MOD KC_LSFT
#define R_MOD KC_LALT
#define M_MOD KC_LGUI
#define I_MOD KC_LCTL


// conf_words

// Returns true for macros used to type sequence of letters
bool is_send_string_macro(uint16_t keycode);

// Returns true for letters that can be followed by an apostrophe (in french)
bool is_followed_by_apos(uint16_t keycode, uint16_t prev_keycode);


// conf_features

// Returns true if `pos` on the left hand of the keyboard, false if right.
bool on_left_hand(keypos_t pos);

// Handles the tap function of tap-hold keys using non-basic keycodes
bool process_custom_tap_hold(uint16_t keycode, keyrecord_t *record);

// Set whether numbers from numrow are to be replaced by numbers from numpad 
void set_numpad(bool target);

// Returns whether numbers from numrow are to be replaced by numbers from numpad
bool replace_numpad(void);

// This function extracts the base keycode of MT and LT,
// even if the tap/hold key is a custom one, with non-basic tap keycode.
uint16_t tap_hold_extractor(uint16_t keycode);

// Macros to be executed at the beginning of process_record_user :
// Layer-tap Repeat and Magic keys
bool process_macros_I(uint16_t keycode, keyrecord_t *record);

// Other macros, to be executed at the end of process_record_user
bool process_macros_II(uint16_t keycode, keyrecord_t *record);


// Prefixing Layers

/* Handler function for prefixing layers

 * My custom layout uses the One Dead Key (1DK) concept.
 * <https://github.com/OneDeadKey/1dk/blob/master/1dk.md>
 * In my keymap, I use a prefixing layer to implement it.
 * This approach allows more flexibility than to use the 1DK directly.
 * This way, on my 1DK layer, I can use macros or symbols that don’t need the 1DK.
 * 
 * I also use One Shot for All (OS4A) keys and layers. 
 * On their side of the keyboard, OS4A keys behave like one-shot layers, mainly to access Callum mods.
 * OS4A keys also act like one-shot shifts for the opposite side of the keyboard.
 * The latter behaviour is implemented with a prefixing layer.
*/
bool process_prefixing_layers(uint16_t keycode, keyrecord_t *record);

// Function to add the One Dead Key to the keys of the 1DK layer
// Also, applies shift to the key following the dead key :
// when I want to shift a key on the 1DK layer,
// I prefer to press shift then the one-shot key to the 1DK layer than the opposite.
bool insert_1dk(uint16_t keycode);