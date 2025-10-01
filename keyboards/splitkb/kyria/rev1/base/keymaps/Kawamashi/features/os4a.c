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

#include "os4a.h"


oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_altgr_state = os_up_unqueued;
oneshot_state os_win_state = os_up_unqueued;

static uint8_t os4a_layer = 0;
static bool exit_os4a_layer = false;

uint8_t get_os4a_layer(void) {
  return (os4a_layer);
}

void os4a_layer_on(uint8_t layer) {
  layer_on(layer);
  os4a_layer = layer;
}

void os4a_layer_off(uint8_t layer) {
  layer_off(layer);
  os4a_layer = 0;
  exit_os4a_layer = false;
}

void os4a_tap(uint16_t keycode) {
  if (os4a_layer == 0) {
      // Activate OS4A layer
      os4a_layer_on(os4a_layer_from_trigger(keycode));
  } else {
      // Press again an OS4A key to exit the OS4A layer and clear the OS mods.
      os4a_layer_off(os4a_layer);
  }
}

bool process_os4a_keys(uint16_t keycode, keyrecord_t *record) {
    // tap action
    if (record->event.pressed && record->tap.count) {
      os4a_tap(keycode);
      return false;
    }
    // normal processing if held
    return true;
}


bool add_shift(uint16_t keycode, keyrecord_t *record) {

  // Testing exit_os4a_layer is necessary to prevent OS shift to be added in case of rolled keys
  // or when other features invoke keycodes to be processed (ex: custom altgr, clever keys).
  if (exit_os4a_layer) { return false; }

  // Shift shouldn't be added if other mods are active
  if ((get_mods() | get_oneshot_mods()) != 0) { return false; }

  // Combos and encoder events.
  if (!IS_KEYEVENT(record->event)) { return true; }

  // Specific exceptions
  if (not_to_be_shifted(keycode)) { return false; }

  // Otherwise, add shift if the key is on the other side of the keyboard.
  return (os4a_layer == _R_MODS) == on_left_hand(record->event.key);
}


void mouse_mods_key_up(uint16_t keycode, keyrecord_t *record) {
    
    // The OS4A layer must be exited only when ctrl or shift are registered,
    // not when the OSM are released without having being held.
    //if (get_mods() & QK_ONE_SHOT_MOD_GET_MODS(keycode)) { 

    // When ctrl or shift are released after being held, exit the OS4A layer.
    if (!record->tap.count) {
      os4a_layer_off(os4a_layer);
    }
}

bool process_mods(uint16_t keycode, keyrecord_t *record) {

  // Handling Callum's OSM on OS4A layers
  update_oneshot(&os_shft_state, KC_LSFT, OS_SHFT, keycode, record);
  update_oneshot(&os_ctrl_state, KC_LCTL, OS_CTRL, keycode, record);
  update_oneshot(&os_alt_state, KC_LALT, OS_LALT, keycode, record);
  update_oneshot(&os_win_state, KC_LWIN, OS_WIN, keycode, record);
  //update_oneshot(&os_altgr_state, KC_RALT, OS_RALT, keycode, record);
  
  // Handling OS4A keys
  if (IS_OS4A_KEY(keycode)) { return process_os4a_keys(keycode, record); }

  // Behaviour of the OS4A layers
  if (os4a_layer != 0) {
    
    if (record->event.pressed) {

        if (should_stay_os4a_layer(keycode)) {
            exit_os4a_layer = false;
        } else {
            if (add_shift(keycode, record)) { set_oneshot_mods(MOD_BIT(KC_LSFT)); }
            exit_os4a_layer = true;
        }
        
    } else {
      // When Ctrl or Shift are released, for mouse use.
      //if (mods_for_mouse(keycode)) { mouse_mods_key_up(keycode, record); }
      if (exit_os4a_layer) { os4a_layer_off(os4a_layer); }
    }
  }
  return true;
}

void os4a_layer_exit_check(void) {
  if (os4a_layer != 0 && exit_os4a_layer) { os4a_layer_off(os4a_layer); }
}
