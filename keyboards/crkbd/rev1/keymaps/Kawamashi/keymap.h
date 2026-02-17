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
#include "features/prefixing_layers.h"


enum layers {
    _BASE = 0,
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
    MAGIC,
    LETTER_1DK,
    SYMBOL_1DK,
    CNL_1DK,
    TG_APOS,
    PG_DEG
};

  // Layer changers
#define LT_RSPC LT(_SHORTNAV, KC_SPC)
#define LT_LSPC LT(_FUNCTIONS, KC_SPC)
#define LT_E LT(_SHORTNAV, PG_E)
#define LT_REPT LT(_NUMBERS, KC_0)
#define LT_MGC LT(_NUMBERS, KC_1)
#define LT_0 LT(_SYMBOLS, KC_0)
#define LT_SYM LT(_SYMBOLS, KC_0)

#define OS_1DK OSL(_1DK)

#ifdef KRYPTON_MAC_MODS
    #define _GUI_T LALT_T
    #define _CTL_T LGUI_T
    #define _ALT_T LCTL_T
    #define _GUI KC_LALT
    #define _CTL KC_LGUI
    #define _ALT KC_LCTL
#else
    #define _GUI_T LGUI_T
    #define _CTL_T LCTL_T
    #define _ALT_T LALT_T
    #define _GUI KC_LGUI
    #define _CTL KC_LCTL
    #define _ALT KC_LALT
#endif

  // Mods
#ifdef KRYPTON_ENABLE_HRM

    #define HRM_O _GUI_T(PG_O)
    #define HRM_A _ALT_T(PG_A)
    #define HRM_I SFT_T(PG_I)
    #define HRM_N _CTL_T(PG_N)
    #define HRM_T _CTL_T(PG_T)
    #define HRM_S SFT_T(PG_S)
    #define HRM_R _ALT_T(PG_R)
    #define HRM_L _GUI_T(PG_L)

    #define HRM_ALL _GUI_T(C(PG_A))
    #define HRM_LA _ALT_T(KC_LEFT)
    #define HRM_DA SFT_T(KC_DOWN)
    #define HRM_RA _CTL_T(KC_RGHT)
    #define HRM_BKS _CTL_T(KC_BSPC)
    #define HRM_BKW SFT_T(C(KC_BSPC))
    #define HRM_ENT _ALT_T(KC_ENT)
    #define HRM_ESC _GUI_T(KC_ESC)

    #define HRM_SLSH SFT_T(PG_SLSH)
    #define HRM_1 KC_1
    #define HRM_5 KC_5
    #define HRM_2 SFT_T(KC_2)
    #define HRM_6 SFT_T(KC_6)

    #define HRM_VOLU _CTL_T(KC_VOLU)
    #define HRM_VOLD SFT_T(KC_VOLD)
    #define HRM_MUTE _ALT_T(KC_MUTE)
    #define HRM_F5 _CTL_T(KC_F5)
    #define HRM_F6 SFT_T(KC_F6)
    #define HRM_F7 _ALT_T(KC_F7)
    #define HRM_F8 _GUI_T(KC_F8)

#else

    #define HRM_O PG_O
    #define HRM_A PG_A
    #define HRM_I PG_I
    #define HRM_N PG_N
    #define HRM_T PG_T
    #define HRM_S PG_S
    #define HRM_R PG_R
    #define HRM_L PG_L

    #define HRM_ALL C(PG_A)
    #define HRM_LA KC_LEFT
    #define HRM_DA KC_DOWN
    #define HRM_RA KC_RGHT
    #define HRM_BKS KC_BSPC
    #define HRM_BKW C(KC_BSPC)
    #define HRM_ENT KC_ENT
    #define HRM_ESC KC_ESC

    #define HRM_SLSH PG_SLSH
    #define HRM_1 KC_1
    #define HRM_5 KC_5
    #define HRM_2 KC_2
    #define HRM_6 KC_6

    #define HRM_VOLU KC_VOLU
    #define HRM_VOLD KC_VOLD
    #define HRM_MUTE KC_MUTE
    #define HRM_F5 KC_F5
    #define HRM_F6 KC_F6
    #define HRM_F7 KC_F7
    #define HRM_F8 KC_F8

#endif

#define MOD_ENT _ALT_T(KC_ENT)
#define MOD_BKP _CTL_T(KC_BSPC)
#define MOD_DEL _GUI_T(KC_DEL)
#define MOD_STB _ALT_T(S(KC_TAB))
#define MOD_TAB _CTL_T(KC_TAB)
#define MOD_ESC _GUI_T(KC_ESC)