/* Copyright 2025 @Kawamashi
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

/*Custom one-shot key structure.
 * 
 * The `trigger` field is the keycode that triggers the modifier or the layer-change.
 * The `suppressor` field is the keycode that 
 * Trigger and suppressor can be the same key, but they can be different, to "bounce" between layers.
 * 
 * 
 * For one-shot mods, the `modifier` field must use the MOD_* prefix.
 * Modifiers can be combined, for ex. MOD_LCTL | MOD_LSFT
 * KC_NO must be used if no modifiers are triggered by the one-shot key.
 * The `layer` field is used for one-shot layers.
 * `0` must be used 
 */ 
typedef struct {
  uint16_t trigger;
  uint16_t suppressor;
  uint8_t modifier;
  uint8_t layer;
} oneshot_t;

// Array of one-shot on steroids keys.
// Each custom OSoS must also be declared in custom_keycodes.
extern const oneshot_t oneshot[];

// Represents the five states a one-shot key can be in
typedef enum {
    os_idle,
    os_down_unused,
    os_down_used,
    os_up_queued,
    os_up_queued_used,
} oneshot_state_t;


uint16_t get_oneshot_on_steroids_tapping_term(uint16_t keycode, keyrecord_t *record);

#if defined(OS_STEROIDS_TAPPING_TERM_PER_KEY)
#   define GET_OS_STEROIDS_TAPPING_TERM(keycode, record) get_oneshot_on_steroids_tapping_term(keycode, record)
#else
#   define GET_OS_STEROIDS_TAPPING_TERM(keycode, record) (OS_STEROIDS_TAPPING_TERM)
#endif

// Oneshot-type wrapper
#ifdef OS_STEROIDS_RELEASED_ON_OTHER_KEY
#  define OS(key1, key2, mods, layer) key1, key2, mods, layer
#else
#  define OS(key, mods, layer) key, key, mods, layer
#endif  // OS_STEROIDS_RELEASED_ON_OTHER_KEY

#ifdef OS_STEROIDS_FREE_LAYER_STACK_PER_KEY
#   define SHOULD_FREE_LAYER_STACK should_oneshot_on_steroids_deactivate_layer(oneshot[i].trigger, key_layer, record)
#else
#   define SHOULD_FREE_LAYER_STACK true
#endif


/* Idle timeout:
 * Custom one-shot mods are configured to deactivate if the keyboard is idle for some time.
 * This is useful to prevent unexpected behaviours.
 * In config.h, define `ONESHOT_TIMEOUT` with a time in milliseconds.
 */
//void oneshot_task(void);

//
bool has_mod_been_absorbed_by_osl(uint8_t mod);

// Deactivates a specific one-shot on steroids key (by index)
void deactivate_oneshot_on_steroids(int8_t index);

// Deactivates a specific one-shot on steroids key (by keycode)
void cancel_oneshot_on_steroids(uint16_t keycode);

// If the keycode is a one-shot on steroids key, returns its state
// Otherwise, returns -1
int8_t get_oneshot_on_steroids_state(uint16_t keycode);

// If the keycode is a one-shot on steroids key, returns its index in the array of OSoS
// Otherwise, returns -1
int8_t get_oneshot_on_steroids_index(uint16_t keycode);

//
uint8_t get_oneshot_layer_on_steroids(void);

// Returns whether a keycode is a custom one-shot key or not
bool is_oneshot_on_steroids(uint16_t keycode);

// Returns whether a keycode is a custom one-shot layer key or not
bool is_oneshot_layer_on_steroids(uint16_t keycode);

// Returns whether a keycode is a custom one-shot mod key or not
bool is_oneshot_mod_on_steroids(uint16_t keycode);

//
bool is_oneshot_layer_on_steroids_active(void);

// Deactivates all one-shot on steroids keys
void clear_oneshots_on_steroids(void);

// Deactivates the active one-shot layer
void reset_oneshot_layer_on_steroids(void);

// Deactivates all one-shot on steroids keys involving a specific modifier
void del_oneshot_mods_on_steroids(uint8_t mods);

// Deactivates all one-shot on steroids keys involving any modifier
void clear_oneshot_mods_on_steroids(void);



//void post_process_oneshot_on_steroids(uint16_t keycode, keyrecord_t *record);

// Custom one-shot keys implementation that doesn't rely on timers.
// If a mod is used while it is held it will be unregistered on keyup as normal.
// Otherwise it will be queued and only released after the next non-mod keyup.
//bool process_oneshot_on_steroids(uint16_t keycode, keyrecord_t *record);



bool is_oneshot_on_steroids_custom_behaviour(uint16_t keycode, keyrecord_t* record);

// To be implemented by the user. Defines keys to cancel one-shot mods and layers.
bool is_oneshot_on_steroids_cancel_key(uint16_t keycode);

// To be implemented by the user. 
// Defines keys not to release the modifier when determining whether a one-shot mod has been used.
// Setting this to layer change keys allows carrying one-shot modifiers between layers.
bool should_oneshot_on_steroids_ignore_key(uint16_t keycode, uint16_t trigger, keyrecord_t* record);

//
bool should_mod_be_held_after_tapping_term(uint8_t mod, uint16_t trigger);

//
bool should_osl_on_steroids_absorb_mods(uint16_t keycode);

//
bool should_oneshot_on_steroids_deactivate_layer(uint16_t keycode, uint8_t layer, keyrecord_t* record);

#ifdef __cplusplus
}
#endif
