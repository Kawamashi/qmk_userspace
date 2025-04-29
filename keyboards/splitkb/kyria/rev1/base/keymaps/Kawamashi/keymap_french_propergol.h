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
#include "keycodes.h"
//#include "keymap.h"

// clang-format off

/*
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
 * │   │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ / │ * │       │
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
 * │     │ . │ É │ U │ P │ B │ V │ M │ C │ ' │ X │ = │ + │     │
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐    │
 * │      │ O │ A │ I │ N │ , │ G │ T │ S │ R │ L │ - │ ’ │    │
 * ├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴────┤
 * │    │ E │ Q │ Z │ Y │ H │ J │ K │ D │ F │ W │ **│          │
 * ├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤
 * │    │    │    │                        │    │    │    │    │
 * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘
 */

// Row 1
#define PG_CCED KC_GRV  // ç
#define FG_1 KC_1       // 1
#define FG_2 KC_2       // 2
#define FG_3 KC_3       // 3
#define FG_4 KC_4       // 4
#define FG_5 KC_5       // 5
#define FG_6 KC_6       // 6
#define FG_7 KC_7       // 7
#define FG_8 KC_8       // 8
#define FG_9 KC_9       // 9
#define FG_0 KC_0       // 0
#define PG_TLSH KC_MINS // /
#define PG_ASTX KC_EQL  // *

// Row 2
#define PG_POIN KC_Q    // .
#define PG_EACU KC_W    // É    
#define PG_U    KC_E    // U
#define PG_P    KC_R    // P
#define PG_B    KC_T    // B
#define PG_V    KC_Y    // V
#define PG_M    KC_U    // M
#define PG_C    KC_I    // C
#define PG_APOS KC_O    // ’
#define PG_X    KC_P    // X
#define PG_EGAL KC_LBRC // =
#define PG_PLUS KC_RBRC // +

// Row 3
#define PG_O    KC_A    // O
#define PG_A    KC_S    // A
#define PG_I    KC_D    // I
#define PG_N    KC_F    // N
#define PG_VIRG KC_G    // ,
#define PG_G    KC_H    // G
#define PG_T    KC_J    // T
#define PG_S    KC_K    // S
#define PG_R    KC_L    // R
#define PG_L    KC_SCLN // L
#define PG_MOIN KC_QUOT // -
#define PG_APOD KC_NUHS // '

// Row 4
#define PG_E    KC_NUBS // E
#define PG_Q    KC_Z    // Q
#define PG_Z    KC_X    // Z
#define PG_Y    KC_C    // Y
#define PG_H    KC_V    // H
#define PG_J    KC_B    // J
#define PG_K    KC_N    // K
#define PG_D    KC_M    // C
#define PG_F    KC_COMM // H
#define PG_W    KC_DOT  // W
#define PG_ODK  KC_SLSH // **


/* Shifted symbols
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
 * │   │   │   │   │   │   │   │   │   │   │ … │   │   │       │
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
 * │     │ ? │   │   │   │   │   │   │   │ ! │   │ ≠ │   │     │
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐    │
 * │      │   │   │   │   │ : │   │   │   │   │   │   │ ? │    │
 * ├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴────┤
 * │    │   │   │   │   │   │   │   │   │   │   │ ; │          │
 * ├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤
 * │    │    │    │                        │    │    │    │    │
 * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘
 */

// Row 3
#define PG_3PTS S(FG_0) // …

// Row 2
#define PG_EXCL S(PG_APOS) // !
#define PG_QUES S(PG_POIN) // ?
#define PG_2PTS S(PG_VIRG) // :
#define PG_DIFF S(PG_EGAL) // ≠


// Row 4
//#define PG_PVIR S(PG_ODK)

// Row 5
#define PG_NBSP S(KC_SPC)  // Espace insecable



/* AltGr symbols
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
 * │   │ ₁ │ ₂ │ ₃ │ ₄ │ ₅ │ ₆ │ ₇ │ ₈ │ ₉ │ ₀ │ à │ ê │       │
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
 * │     │ ^ │ ! │ = │ $ │ % │ # │ ; │ ' │ ? │ : │   │   │     │
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐    │
 * │      │ * │ + │ - │ / │ \ │ ` │ ( │ ) │ < │ > │   │   │    │
 * ├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴────┤
 * │    │ _ │ ~ │ | │ & │ " │ @ │   │ { │ } │ [ │ ] │          │
 * ├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤
 * │    │    │    │                        │    │    │    │    │
 * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘
 */


// Row 1
#define PG_AGR ALGR(PG_TLSH)
#define PG_ECIR ALGR(PG_ASTX)

// Row 2
#define PG_ACIR ALGR(PG_POIN)    // ^
//#define PG_EXCL ALGR(PG_EACU) // !
//#define PG_EGAL ALGR(PG_U) // =
#define PG_DLR  ALGR(PG_P)    // $
#define PG_PERC ALGR(PG_B)    // %
#define PG_HASH ALGR(PG_V)    // #
#define PG_PVIR ALGR(PG_M)    // ;
//#define PG_QUES ALGR(PG_APOS) // ?
//#define PG_2PTS ALGR(PG_X) // :

// Row 3
/* #define PG_ASTX ALGR(PG_O)    // *
#define PG_PLUS ALGR(PG_A)    // +
#define PG_TLSH ALGR(PG_N)    // /
#define PG_MOIN ALGR(PG_I)    // - */
#define PG_BSLS ALGR(PG_VIRG)    // '\'
#define PG_GRV  ALGR(PG_G)    // `
#define PG_LPRN ALGR(PG_T)    // (
#define PG_RPRN ALGR(PG_S)    // )
#define PG_INF ALGR(PG_R)    // <
#define PG_SUP ALGR(PG_L)    // >
//#define PG_HQUO ALGR(PG_L)    // "
#define PG_AROB ALGR(PG_J)

// Row 4
#define PG_UNDS ALGR(PG_E)    // _
#define PG_TILD ALGR(PG_Q)    // ~
#define PG_PIPE ALGR(PG_Z)    // |
#define PG_ESPR ALGR(PG_Y)    // &
#define PG_DQUO ALGR(PG_H)    // "
#define PG_LCBR ALGR(PG_D)    // {
#define PG_RCBR ALGR(PG_F)    // }
#define PG_LSBR ALGR(PG_W)    // [
#define PG_RSBR ALGR(PG_ODK) // ]


/* Shift+AltGr symbols
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
 * │  ̑ │   │   │   │   │   │  ̏ │   │   │   │   │   │   │       │
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
 * │     │ *^│ ¬ │ ≠ │ *¤│ *˚│   │ ≤ │ ≥ │   │ *ˇ│   │   │     │
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐    │
 * │      │ × │ ± │ *¯│ ÷ │   │ *`│ *´│   │ *˘│ *”│   │   │    │
 * ├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴────┤
 * │    │   │ ‰ │ *~│ ¦ │   │   │   │ *¸│ *,│ *˛│   │          │
 * ├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤
 * │    │    │    │                        │    │    │    │    │
 * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘
 */

 #define PG_CARN S(ALGR(PG_X)) // ˇ (dead)
/* // Row 1
#define PG_IBRV S(ALGR(PG_AROB))   //  ̑ (dead)
#define PG_HACU S(ALGR(PG_LPRN)) // ˝ (dead)
#define PG_HGRV S(ALGR(PG_RPRN)) //  ̏ (dead)
#define PG_MDSH S(ALGR(PG_RSQU)) // —
#define PG_CSAQ S(ALGR(PG_CDAQ)) // ‹
#define PG_RSAQ S(ALGR(PG_RDAQ)) // ›
#define PG_IQUE S(ALGR(PG_APOS)) // ¿
// Row 2
#define PG_NM   S(ALGR(PG_N))    // ™
#define PG_HOTB S(ALGR(PG_I))    //  ̣ (dead)
#define PG_PERM S(ALGR(PG_P))    // ‰
#define PG_SBHY S(ALGR(PG_MOIN)) // ‑ (non-breaking hyphen)
#define PG_HDAG S(ALGR(PG_PLUS)) // ‡
// Row 3
#define PG_MACB S(ALGR(PG_F))    // ˍ (dead)
#define PG_TQRT S(ALGR(PG_TLSH)) // √
#define PG_QRTR S(ALGR(PG_ASTX)) // ¼
// Row 4
#define PG_VIRGEQL S(ALGR(PG_INF)) // ≥
#define PG_OGON S(ALGR(PG_G))    // ˛ (dead)
#define PG_IEXL S(ALGR(PG_EACU)) //  ̦ (dead)
//#define PG_SEQL S(ALGR(PG_EGAL)) // ≠ */
