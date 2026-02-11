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

#include "quantum.h"
#include "keymap.h"

#ifdef __cplusplus
extern "C" {
#endif

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
bool deferred_shift_after_dead_key(uint16_t keycode);

#ifdef __cplusplus
}
#endif
