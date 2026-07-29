/* Copyright 2026 @Kawamashi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#ifdef __cplusplus
extern "C" {
#endif

/*Custom one shot on steroids key structure.
 * 
 * The `trigger` field is the keycode that is pressed to initiate the one shot on steroids.
 * The `suppressor` field is the keycode that is held to continue the one shot on steroids.
 * Trigger and suppressor are usually the same key.
 * In some cases, instead of holding down the trigger key, it’s easier to use another key, hence the suppressor key.
 * 
 * For one shot mods, the `modifier` field must use the MOD_BIT() macro.
 * Modifiers can be combined, for ex. MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT)
 * `0` must be used for layer-only one shot keys.
 * The `layer` field is used for one shot layers.
 * `0` must be used for mods-only one shot keys.
 */ 
typedef struct {
  uint16_t trigger;
  uint16_t suppressor;
  uint8_t modifier;
  uint8_t layer;
} oneshot_t;

// Array of one shot on steroids keys.
// Each OSoS key must also be declared in custom_keycodes.
extern const oneshot_t oneshot[];

// Represents the five states a one shot key can be in
typedef enum {
    os_idle,
    os_down_unused,
    os_down_used,
    os_up_queued,
    os_up_queued_used,
} oneshot_state_t;

#if !defined OS_STEROIDS_TERM
#   define OS_STEROIDS_TERM TAPPING_TERM
#endif

// Function to customise the One Shot Term for each one shot on steroids.
uint16_t get_oneshot_on_steroids_term(uint16_t keycode, keyrecord_t *record);

#ifdef OS_STEROIDS_TERM_PER_KEY
#   define GET_OS_STEROIDS_TERM(keycode, record) get_oneshot_on_steroids_term(keycode, record)
#else
#   define GET_OS_STEROIDS_TERM(keycode, record) (OS_STEROIDS_TERM)
#endif

// Oneshot-type wrapper
#ifdef OS_STEROIDS_SPLIT_TRIGGER_HOLD
#  define OS(key1, key2, mods, layer) key1, key2, mods, layer
#else
#  define OS(key, mods, layer) key, key, mods, layer
#endif  // OS_STEROIDS_SPLIT_TRIGGER_HOLD

#ifdef OS_STEROIDS_FREE_LAYER_STACK_PER_KEY
#   define SHOULD_FREE_LAYER_STACK should_oneshot_on_steroids_deactivate_layer(oneshot[i].trigger, key_layer)
#else
#   define SHOULD_FREE_LAYER_STACK true
#endif


/* Idle timeout:
 * One Shot on steroids are configured to deactivate if the keyboard is idle for some time.
 * This is useful to prevent unexpected behaviors.
 * In config.h, define `ONESHOT_TIMEOUT` with a time in milliseconds.
 */
//void oneshot_task(void);

// Returns whether a modifier has been absorbed by an OSoS key
bool has_mod_been_absorbed_by_osl(uint8_t mod);

// Deactivates a specific OSoS key (by index)
void deactivate_oneshot_on_steroids(int8_t index);

// Deactivates a specific OSoS key (by keycode)
void cancel_oneshot_on_steroids(uint16_t keycode);

// If the keycode is an OSoS key, returns its state
// Otherwise, returns -1
int8_t get_oneshot_on_steroids_state(uint16_t keycode);

// If the keycode is an OSoS key, returns its index in the array of OSoS
// Otherwise, returns -1
int8_t get_oneshot_on_steroids_index(uint16_t keycode);

// If there is an active OSoS layer, returns the layer
// Otherwise, returns 0
uint8_t get_oneshot_layer_on_steroids(void);

// Returns whether a keycode is an OSoS key
bool is_oneshot_on_steroids(uint16_t keycode);

// Returns whether a keycode is an OSoS layer key
bool is_oneshot_layer_on_steroids(uint16_t keycode);

// Returns whether a keycode is a modifier-only OSoS key
bool is_oneshot_mod_on_steroids(uint16_t keycode);

// Returns whether an OSoS layer is active
bool is_oneshot_layer_on_steroids_active(void);

// Deactivates all OSoS keys
void clear_oneshots_on_steroids(void);

// Deactivates the active OSoS layer key
void reset_oneshot_layer_on_steroids(void);

// Deactivates all OSoS keys using a specific modifier
void del_oneshot_mods_on_steroids(uint8_t mods);

// Deactivates all OSoS keys using any modifier
void clear_oneshot_mods_on_steroids(void);

// Handles one shot on steroids custom behavior,
// before their standard processing
bool is_oneshot_on_steroids_custom_behavior(uint16_t keycode, keyrecord_t* record);

// Defines keys to cancel one shot mods and layers.
bool is_oneshot_on_steroids_cancel_key(uint16_t keycode);

// Defines keys to be ignored when determining whether a one shot on steroids has been used.
// Setting this to OSoS layer keys allows carrying one shot modifiers between layers.
bool should_oneshot_on_steroids_ignore_key(uint16_t keycode, uint16_t trigger, keyrecord_t* record);

// When triggering the one shot effect, define which mods should be released
// to send one shot mods instead, not to interfere with the mouse.
// By default, the function returns true if mods include shift or ctrl.
bool should_mod_be_held_after_oneshot_release(uint8_t mod, uint16_t trigger);

// Function to customise which OSoS layer keys should absorb modifiers.
bool should_osl_on_steroids_absorb_mods(uint16_t keycode);

// Function to customise which OSoS layer keys should deactivate the layer it is comming from.
bool should_oneshot_on_steroids_deactivate_layer(uint16_t keycode, uint8_t layer);

#ifdef __cplusplus
}
#endif
