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

#define RECENT_SIZE 8    // Number of keys in `recent` buffer.

uint16_t deadline;

uint16_t get_recent_keycode(signed char);
bool was_keycode_replaced(void);
void update_bkspc_countdown(unsigned char i);

void clear_recent_keys(void);
void recent_keys_task(void);

uint16_t get_ongoing_keycode(uint16_t keycode, keyrecord_t* record);
uint16_t get_ongoing_keycode_user(uint16_t keycode, keyrecord_t* record);

void store_keycode(uint16_t keycode, keyrecord_t* record);
void process_key(uint16_t keycode, keyrecord_t* record);
void invoke_key(uint16_t keycode, keyrecord_t* record);
void replace_ongoing_key(uint16_t clever_keycode, uint16_t* ongoing_keycode, keyrecord_t* record);
void process_word(uint16_t keycodes[], uint8_t num_keycodes, keyrecord_t* record);
void finish_word(uint16_t keycodes[], uint8_t num_keycodes, uint16_t* ongoing_keycode, keyrecord_t* record);
void finish_magic(uint16_t keycodes[], uint8_t num_keycodes, uint16_t* ongoing_keycode, keyrecord_t* record);

bool enough_time_before_combo(void);

void process_clever_keys(uint16_t keycode, keyrecord_t* record);
void get_clever_keycode(uint16_t* ongoing_keycode, keyrecord_t* record);
//void end_CK(keyrecord_t* record);

//bool process_accent(uint16_t accent, uint16_t letter, keyrecord_t* record);

#ifdef __cplusplus
}
#endif
