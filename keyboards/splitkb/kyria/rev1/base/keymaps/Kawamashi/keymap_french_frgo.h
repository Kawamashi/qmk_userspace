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
 * │ Ç │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ / │ * │       │
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
 * │     │ X │ , │ É │ P │ B │ F │ M │ L │ ' │ . │ = │ + │     │
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐    │
 * │      │ O │ A │ I │ T │ G │ V │ S │ N │ R │ U │ - │ ’ │    │
 * ├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴────┤
 * │    │ E │ Q │ Z │ Y │ D │ J │ K │ C │ H │ W │ **│          │
 * ├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤
 * │    │    │    │                        │    │    │    │    │
 * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘
 */

// Row 1
#define FG_CCED KC_GRV  // ç
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
#define FG_SLSH KC_MINS // /
#define FG_ASTX KC_EQL  // *

// Row 2
#define FG_X    KC_Q    // X
#define FG_VIRG KC_W    // ,    
#define FG_EACU KC_E    // É
#define FG_P    KC_R    // P
#define FG_B    KC_T    // B
#define FG_F    KC_Y    // F
#define FG_M    KC_U    // M
#define FG_L    KC_I    // L
#define FG_APOS KC_O    // ’
#define FG_POIN KC_P    // .
#define FG_EGAL KC_LBRC // =
#define FG_PLUS KC_RBRC // +

// Row 3
#define FG_O    KC_A    // O
#define FG_A    KC_S    // A
#define FG_I    KC_D    // I
#define FG_T    KC_F    // T
#define FG_G    KC_G    // G
#define FG_V    KC_H    // V
#define FG_S    KC_J    // S
#define FG_N    KC_K    // N
#define FG_R    KC_L    // R
#define FG_U    KC_SCLN // U
#define FG_MOIN KC_QUOT // -
#define FG_APOD KC_NUHS // '

// Row 4
#define FG_E    KC_NUBS // E
#define FG_Q    KC_Z    // Q
#define FG_Z    KC_X    // Z
#define FG_Y    KC_C    // Y
#define FG_D    KC_V    // D
#define FG_J    KC_B    // J
#define FG_K    KC_N    // K
#define FG_C    KC_M    // C
#define FG_H    KC_COMM // H
#define FG_W    KC_DOT  // W
#define FG_TYPO KC_SLSH // **


/* Shifted symbols
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
 * │   │   │   │   │   │   │   │   │   │   │   │   │   │       │
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
 * │     │   │ ! │   │   │   │   │   │   │ ? │ : │ ≠ │   │     │
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐    │
 * │      │   │   │   │   │   │   │   │   │   │   │   │ ? │    │
 * ├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴────┤
 * │    │   │   │   │   │   │   │   │   │   │   │ ; │          │
 * ├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤
 * │    │    │    │                        │    │    │    │    │
 * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘
 */
// Row 2
#define FG_EXLM S(FG_VIRG) // !
#define FG_QUES S(FG_APOS) // ?
#define FG_2PTS S(FG_POIN) // :
#define FG_DIFF S(FG_EGAL) // ≠

// Row 4
//#define FG_PVIR S(FG_TYPO)

// Row 5
#define FG_NBSP S(KC_SPC)  // Espace insecable



/* AltGr symbols
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
 * │   │ ₁ │ ₂ │ ₃ │ ₄ │ ₅ │ ₆ │ ₇ │ ₈ │ ₉ │ ₀ │ à │   │       │
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
#define FG_AGR ALGR(FG_SLSH)

// Row 2
#define FG_ACIR ALGR(FG_X)    // ^
//#define FG_EXLM ALGR(FG_VIRG) // !
//#define FG_EGAL ALGR(FG_EACU) // =
#define FG_DLR  ALGR(FG_P)    // $
#define FG_PERC ALGR(FG_B)    // %
#define FG_HASH ALGR(FG_F)    // #
#define FG_PVIR ALGR(FG_M)    // ;
//#define FG_QUES ALGR(FG_APOS) // ?
//#define FG_2PTS ALGR(FG_POIN) // :

// Row 3
/* #define FG_ASTX ALGR(FG_O)    // *
#define FG_PLUS ALGR(FG_A)    // +
#define FG_SLSH ALGR(FG_T)    // /
#define FG_MOIN ALGR(FG_I)    // - */
#define FG_BSLS ALGR(FG_G)    // '\'
#define FG_GRV  ALGR(FG_V)    // `
#define FG_LPRN ALGR(FG_S)    // (
#define FG_RPRN ALGR(FG_N)    // )
#define FG_INF ALGR(FG_R)    // <
#define FG_SUP ALGR(FG_U)    // >
//#define FG_DQUO ALGR(FG_U)    // "
#define FG_AROB ALGR(FG_J)

// Row 4
#define FG_UNDS ALGR(FG_E)    // _
#define FG_TILD ALGR(FG_Q)    // ~
#define FG_PIPE ALGR(FG_Z)    // |
#define FG_ESPR ALGR(FG_Y)    // &
#define FG_DQUO ALGR(FG_D)    // "
#define FG_LCBR ALGR(FG_C)    // {
#define FG_RCBR ALGR(FG_H)    // }
#define FG_LSBR ALGR(FG_W)    // [
#define FG_RSBR ALGR(FG_TYPO) // ]


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

 #define FG_CARN S(ALGR(FG_POIN)) // ˇ (dead)
/* // Row 1
#define FG_IBRV S(ALGR(FG_AROB))   //  ̑ (dead)
#define FG_DACU S(ALGR(FG_LPRN)) // ˝ (dead)
#define FG_DGRV S(ALGR(FG_RPRN)) //  ̏ (dead)
#define FG_MDSH S(ALGR(FG_RSQU)) // —
#define FG_LSAQ S(ALGR(FG_LDAQ)) // ‹
#define FG_RSAQ S(ALGR(FG_RDAQ)) // ›
#define FG_IQUE S(ALGR(FG_APOS)) // ¿
// Row 2
#define FG_TM   S(ALGR(FG_T))    // ™
#define FG_DOTB S(ALGR(FG_I))    //  ̣ (dead)
#define FG_PERM S(ALGR(FG_P))    // ‰
#define FG_NBHY S(ALGR(FG_MOIN)) // ‑ (non-breaking hyphen)
#define FG_DDAG S(ALGR(FG_PLUS)) // ‡
// Row 3
#define FG_MACB S(ALGR(FG_H))    // ˍ (dead)
#define FG_SQRT S(ALGR(FG_SLSH)) // √
#define FG_QRTR S(ALGR(FG_ASTX)) // ¼
// Row 4
#define FG_GEQL S(ALGR(FG_INF)) // ≥
#define FG_OGON S(ALGR(FG_V))    // ˛ (dead)
#define FG_IEXL S(ALGR(FG_VIRG)) //  ̦ (dead)
//#define FG_NEQL S(ALGR(FG_EGAL)) // ≠ */
