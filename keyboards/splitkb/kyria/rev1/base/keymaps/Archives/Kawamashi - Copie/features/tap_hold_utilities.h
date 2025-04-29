// Copyright 2022-2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file achordion.h
 * @brief Achordion: Customizing the tap-hold decision.
 *
 * Overview
 * --------
 *
 * This library customizes when tap-hold keys are considered held vs. tapped
 * based on the next pressed key, like Manna Harbour's Bilateral Combinations or
 * ZMK's positional hold. The library works on top of QMK's existing tap-hold
 * implementation. You define mod-tap and layer-tap keys as usual and use
 * Achordion to fine-tune the behavior.
 *
 * When QMK settles a tap-hold key as held, Achordion intercepts the event.
 * Achordion then revises the event as a tap or passes it along as a hold:
 *
 *  * Chord condition: On the next key press, a customizable `achordion_chord()`
 *    function is called, which takes the tap-hold key and the next key pressed
 *    as args. When the function returns true, the tap-hold key is settled as
 *    held, and otherwise as tapped.
 *
 *  * Timeout: If no other key press occurs within a timeout, the tap-hold key
 *    is settled as held. This is customizable with `achordion_timeout()`.
 *
 * Achordion only changes the behavior when QMK considered the key held. It
 * changes some would-be holds to taps, but no taps to holds.
 *
 * @note Some QMK features handle events before the point where Achordion can
 * intercept them, particularly: Combos, Key Lock, and Dynamic Macros. It's
 * still possible to use these features and Achordion in your keymap, but beware
 * they might behave poorly when used simultaneously with tap-hold keys.
 *
 *
 * For full documentation, see
 * <https://getreuer.info/posts/keyboards/achordion>
 */

#pragma once

#include "quantum.h"
#include "keymap.h"

#ifdef __cplusplus
extern "C" {
#endif

bool on_left_hand(keypos_t pos);

bool same_side_combination(const keyrecord_t* tap_hold_record, const keyrecord_t* other_record);

void tap_converter(uint16_t keycode, keyrecord_t *record);

bool process_custom_tap_hold(uint16_t keycode, keyrecord_t *record);

#ifdef __cplusplus
}
#endif
