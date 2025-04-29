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
 * │ À │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ / │ * │       │
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
 * │     │ X │ , │ É │ P │ B │ F │ M │ L │ ' │ . │ = │ + │     │
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐    │
 * │      │ O │ A │ I │ T │ G │ V │ S │ N │ R │ U │ Ù │ Ç │    │
 * ├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴────┤
 * │    │ E │ Q │ Z │ Y │ D │ J │ K │ C │ H │ W │ **│          │
 * ├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤
 * │    │    │    │                        │    │    │    │    │
 * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘
 */

// Row 1
#define FR_AGRV KC_GRV  // à
#define FR_1 KC_1       // 1
#define FR_2 KC_2       // 2
#define FR_3 KC_3       // 3
#define FR_4 KC_4       // 4
#define FR_5 KC_5       // 5
#define FR_6 KC_6       // 6
#define FR_7 KC_7       // 7
#define FR_8 KC_8       // 8
#define FR_9 KC_9       // 9
#define FR_0 KC_0       // 0
#define FR_SLSH KC_MINS // /
#define FR_ASTX KC_EQL  // *

// Row 2
#define FR_X    KC_Q    // X
#define FR_VIRG KC_W    // ,    
#define FR_EACU KC_E    // É
#define FR_P    KC_R    // P
#define FR_B    KC_T    // B
#define FR_F    KC_Y    // F
#define FR_M    KC_U    // M
#define FR_L    KC_I    // L
#define FR_APOS KC_O    // '
#define FR_POIN KC_P    // .
#define FR_EGAL KC_LBRC // =
#define FR_PLUS KC_RBRC // +

// Row 3
#define FR_O    KC_A    // O
#define FR_A    KC_S    // A
#define FR_I    KC_D    // I
#define FR_T    KC_F    // T
#define FR_G    KC_G    // G
#define FR_V    KC_H    // V
#define FR_S    KC_J    // S
#define FR_N    KC_K    // N
#define FR_R    KC_L    // R
#define FR_U    KC_SCLN // U
#define FR_UGRV KC_QUOT // Ù
#define FR_CCED KC_NUHS // Ç

// Row 4
#define FR_E    KC_NUBS // E
#define FR_Q    KC_Z    // Q
#define FR_Z    KC_X    // Z
#define FR_Y    KC_C    // Y
#define FR_D    KC_V    // D
#define FR_J    KC_B    // J
#define FR_K    KC_N    // K
#define FR_C    KC_M    // C
#define FR_H    KC_COMM // H
#define FR_W    KC_DOT  // W
#define FR_TYPO KC_SLSH // **


/* Shifted symbols
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
 * │   │   │   │   │   │   │   │   │   │   │   │   │   │       │
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
 * │     │   │ ! │   │   │   │   │   │   │ ? │ : │ ≠ │   │     │
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐    │
 * │      │   │   │   │   │   │   │   │   │   │   │   │   │    │
 * ├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴────┤
 * │    │   │   │   │   │   │   │   │   │   │   │   │          │
 * ├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤
 * │    │    │    │                        │    │    │    │    │
 * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘
 */
// Row 2
#define FR_EXLM S(FR_VIRG) // !
#define FR_QUES S(FR_APOS) // ?
#define FR_2PTS S(FR_POIN) // :
#define FR_DIFF S(FR_EGAL) // ≠

// Row 5
#define FR_NBSP S(KC_SPC)  // Espace insecable



/* AltGr symbols
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
 * │   │ ₁ │ ₂ │ ₃ │ ₄ │ ₅ │ ₆ │ ₇ │ ₈ │ ₉ │ ₀ │   │   │       │
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
 * │     │ ^ │ ! │ = │ $ │ @ │ # │ < │ > │ ? │ : │   │   │     │
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐    │
 * │      │ * │ + │ - │ / │ \ │ ` │ ( │ ) │ ; │ " │   │   │    │
 * ├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴────┤
 * │    │ _ │ % │ ~ │ | │ & │   │   │ { │ } │ [ │ ] │          │
 * ├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤
 * │    │    │    │                        │    │    │    │    │
 * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘
 */

// Row 2
#define FR_ACIR ALGR(FR_X)    // ^
//#define FR_EXLM ALGR(FR_VIRG) // !
//#define FR_EGAL ALGR(FR_EACU) // =
#define FR_DLR  ALGR(FR_P)    // $
#define FR_AROB ALGR(FR_B)    // @
#define FR_HASH ALGR(FR_F)    // #
#define FR_INF ALGR(FR_M)    // <
#define FR_SUP ALGR(FR_L)    // > 
//#define FR_QUES ALGR(FR_APOS) // ?
//#define FR_2PTS ALGR(FR_POIN) // :

// Row 3
/* #define FR_ASTX ALGR(FR_O)    // *
#define FR_PLUS ALGR(FR_A)    // +
#define FR_SLSH ALGR(FR_T)    // / */
#define FR_MOIN ALGR(FR_I)    // -
#define FR_BSLS ALGR(FR_G)    // '\'
#define FR_GRV  ALGR(FR_V)    // `
#define FR_LPRN ALGR(FR_S)    // (
#define FR_RPRN ALGR(FR_N)    // )
#define FR_PVIR ALGR(FR_R)    // ;
#define FR_DQUO ALGR(FR_U)    // "

// Row 4
#define FR_UNDS ALGR(FR_E)    // _
#define FR_PERC ALGR(FR_Q)    // %
#define FR_TILD ALGR(FR_Z)    // ~
#define FR_PIPE ALGR(FR_Y)    // |
#define FR_ESPR ALGR(FR_D)    // &
#define FR_LCBR ALGR(FR_C)    // {
#define FR_RCBR ALGR(FR_H)    // }
#define FR_LSBR ALGR(FR_W)    // [
#define FR_RSBR ALGR(FR_TYPO) // ]


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

 #define FR_CARN S(ALGR(FR_POIN)) // ˇ (dead)
/* // Row 1
#define FR_IBRV S(ALGR(FR_AROB))   //  ̑ (dead)
#define FR_DACU S(ALGR(FR_LPRN)) // ˝ (dead)
#define FR_DGRV S(ALGR(FR_RPRN)) //  ̏ (dead)
#define FR_MDSH S(ALGR(FR_RSQU)) // —
#define FR_LSAQ S(ALGR(FR_LDAQ)) // ‹
#define FR_RSAQ S(ALGR(FR_RDAQ)) // ›
#define FR_IQUE S(ALGR(FR_APOS)) // ¿
// Row 2
#define FR_TM   S(ALGR(FR_T))    // ™
#define FR_DOTB S(ALGR(FR_I))    //  ̣ (dead)
#define FR_PERM S(ALGR(FR_P))    // ‰
#define FR_NBHY S(ALGR(FR_MOIN)) // ‑ (non-breaking hyphen)
#define FR_DDAG S(ALGR(FR_PLUS)) // ‡
// Row 3
#define FR_MACB S(ALGR(FR_H))    // ˍ (dead)
#define FR_SQRT S(ALGR(FR_SLSH)) // √
#define FR_QRTR S(ALGR(FR_ASTX)) // ¼
// Row 4
#define FR_GEQL S(ALGR(FR_INF)) // ≥
#define FR_OGON S(ALGR(FR_V))    // ˛ (dead)
#define FR_IEXL S(ALGR(FR_VIRG)) //  ̦ (dead)
//#define FR_NEQL S(ALGR(FR_EGAL)) // ≠ */
