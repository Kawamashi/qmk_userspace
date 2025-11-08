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
#include "keymap.h"

// Idle timout:
// Layerword can be configured to deactivate if the keyboard is idle
// for some time. This is useful to prevent unexpected behaviours.
void layerword_task(void);

// Each layerword timeout timer can be defined individually (in ms).
uint16_t layerword_exit_timeout(uint8_t layer);

// Returns the layerword number
uint8_t get_layerword_layer(void);

void enable_layerword(uint8_t layer);
void disable_layerword(uint8_t layer);
void toggle_layerword(uint16_t keycode);
bool process_layerword_keys(uint16_t keycode, keyrecord_t *record);

bool process_layerword(uint16_t keycode, keyrecord_t *record);

uint8_t layerword_layer_from_trigger(uint16_t keycode);
bool should_continue_layerword(uint8_t layer, uint16_t keycode, keyrecord_t *record);

bool should_add_shift(uint16_t keycode, keyrecord_t *record);
bool not_to_be_shifted(uint16_t keycode);
void mouse_mods_key_up(uint16_t keycode, keyrecord_t *record);