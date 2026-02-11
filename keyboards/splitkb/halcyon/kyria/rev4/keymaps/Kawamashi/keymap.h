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
#include "features/oneshot.h"
#include "features/prefixing_layers.h"


enum layers {
    _BASE = 0,
    // OS4A layers should be as closed as base layer as possible
    _1DK,
    _NUMBERS,
    _SYMBOLS,
    _SHORTNAV,
    _FUNCTIONS,
};

enum custom_keycodes {
  NUMWORD = SAFE_RANGE,
  NAVWORD,
  FUNWORD,
  CAPSWORD,
  CAPSLIST,
  CAPSLOCK,
  SEL_WORD,
  SEL_LINE,
  COPY,
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
#define LT_REPT LT(_NUMBERS, KC_1)
#define LT_MGC LT(_SHORTNAV, KC_1)
#define LT_0 LT(_SYMBOLS, KC_0)

#define OS_1DK OSL(_1DK)

  // Mods
#define HRM_O CTL_T(PG_O)
#define HRM_A OPT_T(PG_A)
#define HRM_I CMD_T(PG_I)
#define HRM_N SFT_T(PG_N)
#define HRM_T SFT_T(PG_T)
#define HRM_S CMD_T(PG_S)
#define HRM_R OPT_T(PG_R)
#define HRM_L CTL_T(PG_L)
#define HRM_H LT(_FUNCTIONS, PG_H)
#define HRM_D LT(_FUNCTIONS, PG_D)

#define MT_SLSH SFT_T(PG_SLSH)
#define MT_1 SFT_T(KC_1)
#define MT_6 SFT_T(KC_6)