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

//#define TAPPING_TOGGLE 1

// combo
#define COMBO_TERM 40
//#define COMBO_TERM_PER_COMBO
#define COMBO_ONLY_FROM_LAYER 0
#define COMBO_SHOULD_TRIGGER
#define COMBO_PROCESS_KEY_REPRESS 
#define TAP_INTERVAL 300

// mod tap
#define TAPPING_TERM 250
#define PERMISSIVE_HOLD
#define FLOW_TAP_TERM 150
#define CHORDAL_HOLD
#define SPECULATIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY


// Timeouts in milliseconds.
#define RECENT_KEYS_TIMEOUT 5000
#define CAPS_WORD_IDLE_TIMEOUT 3000
#define OS4A_EXIT_TIMEOUT 3000
#define ONESHOT_TIMEOUT 3000