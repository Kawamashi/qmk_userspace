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
#include "features_conf.h"
#include "word_conf.h"
#include "features/tap_hold_utilities.h"
#include "features/clever_keys_utilities.h"
#include "features/layerword.h"
#include "features/modword.h"
#include "features/speculative_hold.h"
#include "features/prefixing_layers.h"


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
  NUMWORD = SAFE_RANGE,
  NUMPAD,
  NUMROW,
  NAVWORD,
  FUNWORD,
  CAPSWORD,
  CAPSLIST,
  CAPSLOCK,
  SEL_WORD,
  SEL_LINE,
  OS4A,
  OU_GRV,
  N_TILD,
  MAGIC,
  LETTER_1DK,
  SYMBOL_1DK,
  CNL_1DK,
  TG_APOS,
  PG_DEG
};

  // Layer changers
#define LT_SPC LT(_SYMBOLS, KC_SPC)
#define LT_E LT(_SYMBOLS, PG_E)
#define LT_REPT LT(_FUNCTIONS, KC_1)
#define LT_MGC LT(_SHORTNAV, KC_1)
#define LT_0 LT(_SYMBOLS, KC_0)
#define LT_P0 LT(_SYMBOLS, KC_P0)
#define LT_PDOT LT(_SHORTNAV, KC_PDOT)
#define LT_APOS LT(_NUMROW, PG_APOS)

#define OS_1DK OSL(_1DK)
#define OS_WINM OSL(_FUNCAPPS)
#define OS_NUM OSL(_NUMPAD)
#define OS_WNUM OSL(_NUMROW)


  // HRM
#ifdef KRYPTON_ENABLE_HRM
  #define P(k) LALT_T(k)
  #define R(k) LGUI_T(k)
  #define M(k) SFT_T(k)
  #define I(k) LCTL_T(k)
#else
  #define P(k) k
  #define R(k) k
  #define M(k) k
  #define I(k) k
#endif

#define P_MOD KC_LALT
#define R_MOD KC_LGUI
#define M_MOD KC_LSFT
#define I_MOD KC_LCTL