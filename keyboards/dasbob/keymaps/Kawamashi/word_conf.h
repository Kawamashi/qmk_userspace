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

// Returns whether a keycode is a letter or not
bool is_letter(uint16_t keycode);

// Returns true for macros used to type sequence of letters
bool is_send_string_macro(uint16_t keycode);

// Returns true for letters that can be followed by an apostrophe (in french)
bool is_followed_by_apos(uint16_t keycode, uint16_t prev_keycode);