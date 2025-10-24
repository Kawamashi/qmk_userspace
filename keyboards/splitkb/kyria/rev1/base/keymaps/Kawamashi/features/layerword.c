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

#include "layerword.h"

//static uint16_t layer_word_timer = 0;

static uint8_t layerword_layer = 0;
static bool continue_layerword = false;

static uint16_t idle_timer = 0;

void layerword_task(void) {
    if (layerword_layer != 0 && timer_expired(timer_read(), idle_timer)) {
        disable_layerword(layerword_layer);
    }
}

uint8_t get_layerword_layer(void) {
    return (layerword_layer);
}

void enable_layerword(uint8_t layer) {
    if (layerword_layer != 0) { layer_off(layerword_layer); }   // Only one layerword can be active at a time
    layer_on(layer);
    layerword_layer = layer;
    continue_layerword = true;
    idle_timer = timer_read() + layerword_exit_timeout(layer);
}
  
void disable_layerword(uint8_t layer) {
    layer_off(layer);
    layerword_layer = 0;
}

void toggle_layerword(uint16_t keycode) {
    uint8_t layerword_target = layerword_layer_from_trigger(keycode);

    if (layerword_layer != layerword_target) {
        // Activate layerword layer
        enable_layerword(layerword_target);
    } else {
        // Press again an layerword key to exit the layerword layer
        disable_layerword(layerword_target);
    }
}
  
bool process_layerword_keys(uint16_t keycode, keyrecord_t *record) {

    // Normal processing when hold
    if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
        if (record->tap.count == 0) { return true; }
    }
    // Tap action
    if (record->event.pressed) {
        toggle_layerword(keycode);
        return false;
    }
    // Normal processing on release
    return true;
}


bool process_layerword(uint16_t keycode, keyrecord_t *record) {
    // Handle the custom keycodes that go with this feature
    if (layerword_layer_from_trigger(keycode)) { return process_layerword_keys(keycode, record); }

    // Other than the custom keycodes, nothing else in this feature will activate
    // if the behavior is not on, so allow QMK to handle the event as usual.
    if (layerword_layer == 0) { return true; }

    // Should exit layer word on key release or on 2nd keypress of rolled keys
    if (!continue_layerword) {
        disable_layerword(layerword_layer);
        return true;
    }

    if (record->event.pressed) {

        // Get the base keycode of a mod or layer tap key
        switch (keycode) {
            case QK_MOD_TAP ... QK_MOD_TAP_MAX:
            case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
                // Earlier return if this has not been considered tapped yet
                if (record->tap.count == 0) { return true; }
                keycode = tap_hold_extractor(keycode);
                break;
        }

        if (layerword_exit_timeout(layerword_layer) > 0) {
            idle_timer = record->event.time + layerword_exit_timeout(layerword_layer);
        }
        continue_layerword = should_continue_layerword(layerword_layer, keycode, record);

    } else {  // On keyrelease
        
        if (IS_QK_LAYER_TAP(keycode) && record->tap.count == 0) {
            // Release event on a held layer-tap key when layerword is on.
            // Skip default handling so that layer stays on.
            if (QK_LAYER_TAP_GET_LAYER(keycode) == layerword_layer) { return false; }
        }
    }
    return true;
}

bool should_add_shift(uint16_t keycode, keyrecord_t *record) {

  // Shift shouldn't be added if other mods are active
  if (get_mods() | get_oneshot_mods()) { return false; }

  // Combos and encoder events.
  if (!IS_KEYEVENT(record->event)) { return true; }

  // Specific exceptions
  if (not_to_be_shifted(keycode)) { return false; }

  // Otherwise, add shift if the key is on the other side of the keyboard.
  return (layerword_layer == _R_MODS) == on_left_hand(record->event.key);
}


void mouse_mods_key_up(uint16_t keycode, keyrecord_t *record) {
    
    // The OS4A layer must be exited only when ctrl or shift are registered,
    // not when the OSM are released without having being held.
    //if (get_mods() & QK_ONE_SHOT_MOD_GET_MODS(keycode)) { 

    // When ctrl or shift are released after being held, exit the OS4A layer.
    if (!record->tap.count) {
      disable_layerword(layerword_layer);
    }
}
