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


/* #include "keymap_french_propergol.h"
#include "features_conf.h"
#include "word_conf.h"
#include "features/tap_hold_utilities.h"
#include "features/clever_keys_utilities.h"
#include "features/layerword.h"
#include "features/modword.h"
#include "features/speculative_hold.h"
#include "features/prefixing_layers.h"
#include "features/oneshot.h" */

#include "krypton.h"


bool get_speculative_hold(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {  // Enable speculative holding for these keys.
    case M(PG_I):
    case I(PG_N):
      return true;
  }
  return false;  // Disable otherwise.
}

// Housekeeping

void housekeeping_task_user(void) {
  housekeeping_task_tap_flow();
  recent_keys_task();
  modword_task();
  layerword_task();
  oneshot_task();
}


// Key processing

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {

  process_flow_tap(keycode, record);
  pre_process_speculative_hold(keycode, record);

  return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // Speculative Hold
  if (!process_record_speculative_hold(keycode, record)) { return false; }

  // LT Repeat and Magic keys
  if (!process_macros_I(keycode, record)) { return false; }

  // 
  if (!process_custom_oneshot(keycode, record)) { return false; }

  // Layer word
  if (!process_layerword(keycode, record)) { return false; }

  // Prefixed layers
  if (!process_prefixing_layers(keycode, record)) { return false; }

  // Clever keys
  process_clever_keys(keycode, record);

  // Caps Word
  if (!process_modword(keycode, record)) {return false; }

  // Other macros
  if (!process_macros_II(keycode, record)) { return false; }

  // Process all other keycodes normally
  return true;
}