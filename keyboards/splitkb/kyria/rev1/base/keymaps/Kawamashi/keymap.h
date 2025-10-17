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
//#include "clever_keys.h"
#include "features/tap_hold_utilities.h"
#include "features/clever_keys_utilities.h"
#include "features/numword.h"
#include "features/capslist.h"
#include "features/word_selection.h"
#include "features/os4a.h"
#include "features/oneshot.h"
#include "features/odk_layer.h"


enum layers {
    _BASE = 0,
    // OS4A layers should be as closed as base layer as possible
    _L_MODS,
    _R_MODS,
    _ODK,
    _NUMBERS,
    _SYMBOLS,
    _SHORTNAV,
    _FUNCAPPS,
};

enum custom_keycodes {
  NUMWORD = SAFE_RANGE,
  CAPSLIST,
  FEN_B,
  COPY,
  //AIDE_MEM,
  OS4A,
  CAPSWORD,
  OU_GRV,
  MAGIC,
  OS_SHFT,
  OS_CTRL,
  OS_LALT,
  OS_WIN,
  ODK,
  CNL_ODK,
  TG_APOS,
  PG_DEG,
  SEL_WORD,
  SEL_LINE
};

  // Layer changers
#define LT_SPC LT(_SYMBOLS, KC_SPC)
#define LT_E LT(_SYMBOLS, PG_E)
#define LT_REPT LT(_NUMBERS, KC_1)
#define LT_MGC LT(_SHORTNAV, KC_1)
#define LT_0 LT(_SYMBOLS, KC_0)
#define LT_NBSPC LT(_SHORTNAV, NNB_SPC)

#define OS_ODK OSL(_ODK)
#define NUM_ODK OSL(_NUMBERS)
#define OS_FA OSL(_FUNCAPPS)
#define TG_FA TT(_FUNCAPPS)

  // Mods
#define MT_SLSH SFT_T(PG_SLSH)
#define MT_1 SFT_T(KC_1)
#define OS_RSA OSM(MOD_RALT | MOD_LSFT)

  // OS4A
#define L_OS4A LSFT_T(OS4A)
#define R_OS4A RSFT_T(OS4A)
#define IS_OS4A_KEY(keycode) (os4a_layer_from_trigger(keycode) != 0)

bool forbidden_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record);