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
#pragma once

#include QMK_KEYBOARD_H

#include "keymap_french_frgo.h"
#include "features_conf.h"
#include "features/custom_altgr_keys.h"
#include "features/tap_hold_utilities.h"
#include "features/next_keycode.h"
#include "features/clever_keys.h"
#include "features/numword.h"
#include "features/macros.h"
#include "features/os4a.h"
#include "features/oneshot.h"


enum layers {
    _BASE = 0,
    // OS4A layers should be as closed as base layer as possible
    _L_MODS,
    _R_MODS,
    _TYPO,
    _NUMBERS,
    _SHORTNAV,
    _FUNCAPPS,
};

enum custom_keycodes {
  NUMWORD = SAFE_RANGE,
  FEN_B,
  COPY,
/*   ALT_TAB,
  REV_TAB, */
  AIDE_MEM,
  OS4A,
  RAZ,
  CAPSWORD,
  AGRV_SPC,
  OU_GRV,
  E_CIRC,
  I_CIRC,
  A_CIRC,
  O_CIRC,
  U_CIRC,
  I_TREM,
  CA_CED,
  CIRC,
  TILDE,
  GRAVE,
  MAGIC,
  OS_SHFT,
  OS_CTRL,
  OS_RALT,
  OS_LALT,
  OS_WIN
};

// Layer taps
#define LT_SPC ALGR_T(KC_SPC)
#define LT_E ALGR_T(FR_E)
#define LT_REPT LT(_NUMBERS, KC_0)
#define LT_MGC LT(_SHORTNAV, KC_0)
#define LT_D FR_D
#define LT_C FR_C
#define LT_VIRG LT(_SHORTNAV, FR_VIRG)
#define OS_TYPO OSL(_TYPO)
#define OS_FA OSL(_FUNCAPPS)
#define TT_FA TT(_FUNCAPPS)
#define TG_ACC TG(_TYPO)

// One shot mods
#define L_OS4A LSFT_T(OS4A)
#define R_OS4A RSFT_T(OS4A)
#define IS_OS4A_KEY(keycode) (get_os4a_layer(keycode) != 0)

bool forbidden_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record);

extern uint16_t global_quick_tap_timer;

extern uint16_t next_keycode;
extern keyrecord_t next_record;