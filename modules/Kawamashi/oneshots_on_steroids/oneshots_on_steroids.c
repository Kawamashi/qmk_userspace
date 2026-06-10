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

#include "oneshots_on_steroids.h"

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 0, 0);

#ifdef NO_ACTION_ONESHOT
#error "One Shots on Steroids need vanilla One Shot Keys to work. Please undefine NO_ACTION_ONESHOT."
#endif  // NO_ACTION_ONESHOT

#if defined OS_STEROIDS_RELAY_MODS_PER_KEY && !defined OS_STEROIDS_RELAY_MODS
#error "OS_STEROIDS_RELAY_MODS_PER_KEY needs OS_STEROIDS_RELAY_MODS to work. Please define OS_STEROIDS_RELAY_MODS."
#endif

#if defined OS_STEROIDS_FREE_LAYER_STACK_PER_KEY && !defined OS_STEROIDS_FREE_LAYER_STACK
#error "OS_STEROIDS_FREE_LAYER_STACK_PER_KEY needs OS_STEROIDS_FREE_LAYER_STACK to work. Please define OS_STEROIDS_FREE_LAYER_STACK."
#endif

static oneshot_state_t oneshot_state[OS_STEROIDS_COUNT] = { [0 ... OS_STEROIDS_COUNT - 1] = os_idle };

static uint16_t idle_timer = 0;

static uint16_t oneshot_tap_time[OS_STEROIDS_COUNT] = { [0 ... OS_STEROIDS_COUNT - 1] = 0 };

static uint8_t oneshot_mods_on_steroids = 0;

#if defined OS_STEROIDS_RELAY_MODS
static uint8_t oneshot_pressed_mods[OS_STEROIDS_COUNT] = { [0 ... OS_STEROIDS_COUNT - 1] = 0 };
static uint8_t oneshot_added_mods[OS_STEROIDS_COUNT] = { [0 ... OS_STEROIDS_COUNT - 1] = 0 };
#endif  // OS_STEROIDS_RELAY_MODS

#ifdef OS_STEROIDS_RELAY_MODS_PER_KEY
#   define SHOULD_RELAY_MODS should_oneshot_on_steroids_relay_mods(oneshot[i].trigger, record)
#else
#   define SHOULD_RELAY_MODS true
#endif

#ifdef OS_STEROIDS_FREE_LAYER_STACK
static uint8_t oneshot_origin_layer[OS_STEROIDS_COUNT] = { [0 ... OS_STEROIDS_COUNT - 1] = 0 };
#endif  // OS_STEROIDS_FREE_LAYER_STACK

#ifdef OS_STEROIDS_FREE_LAYER_STACK_PER_KEY
#   define SHOULD_FREE_LAYER_STACK should_oneshot_on_steroids_deactivate_layer(oneshot[i].trigger, key_layer, record)
#else
#   define SHOULD_FREE_LAYER_STACK true
#endif

#ifdef OS_STEROIDS_TIMEOUT
void housekeeping_task_oneshots_on_steroids(void) {
    if (idle_timer && timer_expired(timer_read(), idle_timer)) {
        clear_all_oneshots_on_steroids();
        idle_timer = 0;
    }
}
#endif  // OS_STEROIDS_TIMEOUT

void unregister_mods_on_steroids(uint8_t mods) {
#       ifdef DUMMY_MOD_NEUTRALIZER_KEYCODE
     neutralize_flashing_modifiers(mods);
#       endif // DUMMY_MOD_NEUTRALIZER_KEYCODE
    unregister_mods(mods);
}

void clear_oneshot_on_steroids(int8_t index) {
    if (oneshot_state[index] != os_idle) {

        if (oneshot[index].modifier != 0) {
            if (!should_mod_be_held_after_tapping_term(oneshot[index].modifier, oneshot[index].trigger) && oneshot_state[index] == os_up_queued) {
                // OSoS mods involving Ctrl and/or Shift
                del_oneshot_mods(oneshot[index].modifier);
            } else {
                unregister_mods_on_steroids(oneshot[index].modifier);
            }
            oneshot_mods_on_steroids &= ~oneshot[index].modifier;
        }
    #       ifdef OS_STEROIDS_RELAY_MODS
        if (oneshot_added_mods[index] != 0) {
            unregister_mods_on_steroids(oneshot_added_mods[index]);
            oneshot_added_mods[index] = 0;
        }
    #       endif  // OS_STEROIDS_RELAY_MODS

        if (oneshot[index].layer != 0) {
            layer_off(oneshot[index].layer);
    #           ifdef OS_STEROIDS_FREE_LAYER_STACK
            if (oneshot_origin_layer[index] != 0) { 
                layer_on(oneshot_origin_layer[index]);
                oneshot_origin_layer[index] = 0;
            }
    #           endif  // OS_STEROIDS_FREE_LAYER_STACK
        }
        oneshot_state[index] = os_idle;
    }
}

void finish_oneshot_on_steroids(uint8_t index) {
    if (oneshot_state[index] != os_idle) {

        if (oneshot[index].modifier != 0) { 
            if (should_mod_be_held_after_tapping_term(oneshot[index].modifier, oneshot[index].trigger) || oneshot_state[index] != os_up_queued) {
                unregister_mods(oneshot[index].modifier);
            }
            oneshot_mods_on_steroids &= ~oneshot[index].modifier;
        }
    #       ifdef OS_STEROIDS_RELAY_MODS
        if (oneshot_added_mods[index] != 0) {
            unregister_mods(oneshot_added_mods[index]);
            oneshot_added_mods[index] = 0;
        }
    #       endif  // OS_STEROIDS_RELAY_MODS

        if (oneshot[index].layer != 0) {
            layer_off(oneshot[index].layer);
    #           ifdef OS_STEROIDS_FREE_LAYER_STACK
            if (oneshot_origin_layer[index] != 0) { 
                layer_on(oneshot_origin_layer[index]);
                oneshot_origin_layer[index] = 0;
            }
    #           endif  // OS_STEROIDS_FREE_LAYER_STACK
        }
        oneshot_state[index] = os_idle;
    }
}

int8_t get_oneshot_on_steroids_state(uint16_t keycode) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (keycode == oneshot[i].trigger) { return oneshot_state[i]; }
    }
    return -1;
}

int8_t get_oneshot_on_steroids_index(uint16_t keycode) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (keycode == oneshot[i].trigger) { return i; }
    }
    return -1;
}

uint8_t get_oneshot_mods_on_steroids(void) {
    return oneshot_mods_on_steroids;
}

bool is_oneshot_on_steroids(uint16_t keycode) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (keycode == oneshot[i].trigger) { return true; }
    }
    return false;
}

void clear_all_oneshots_on_steroids(void) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (oneshot_state[i] != os_idle) { clear_oneshot_on_steroids(i); }
    }
}

void clear_oneshot_layer_on_steroids(uint8_t layer) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (oneshot[i].layer == layer && oneshot_state[i] != os_idle) {
            clear_oneshot_on_steroids(i);
        }
    }
}

void clear_all_oneshot_mod_on_steroids(void) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (oneshot_state[i] != os_idle){
            if (oneshot[i].modifier != 0) {
                clear_oneshot_on_steroids(i);
#               ifdef OS_STEROIDS_RELAY_MODS
            } else if (oneshot_added_mods[i] != 0) {
                // Case of OSL carrying modifiers
                // In this case, we must remove modifiers w/o cancelling the OSL.
                unregister_mods_on_steroids(oneshot_added_mods[i]);
                oneshot_added_mods[i] = 0;
#               endif  // OS_STEROIDS_RELAY_MODS
            }
        }
    }
}

bool is_oneshot_layer_on_steroids_active(void) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (oneshot[i].layer != 0 && oneshot_state[i] != os_idle) { return true; }
    }
    return false;
}


#ifdef OS_STEROIDS_RELAY_MODS
bool modifiers_handling(uint8_t index, uint8_t mod) {
    if (mod & oneshot_pressed_mods[index]) {

        switch (oneshot_state[index]) {
            case os_down_unused:
            case os_up_queued:
                oneshot_added_mods[index] |= mod;
                oneshot_pressed_mods[index] &= ~mod;
                return false;

            default:
                return true;
        }
    }
    return true;
}
#endif  // OS_STEROIDS_RELAY_MODS

#ifdef OS_STEROIDS_FREE_LAYER_STACK
// Handles `LT`, `MO` and `TT` keys.
bool layer_switching_keys_handling(uint8_t i, uint8_t key_layer, keyrecord_t *record) {
    if (SHOULD_FREE_LAYER_STACK && key_layer == oneshot_origin_layer[i]) {
        oneshot_origin_layer[i] = 0;
        return false;  // Skip default handling.
    }
    return true;
}
#endif  // OS_STEROIDS_FREE_LAYER_STACK


bool process_record_oneshots_on_steroids(uint16_t keycode, keyrecord_t *record){
    
    bool should_continue_processing = true;

    if (!is_oneshot_on_steroids_custom_behaviour(keycode, record)) { return false; }

    // Processing triggers and suppressors
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {

        if (record->event.pressed) {    // On press

            if (keycode == oneshot[i].trigger) {
                // Trigger keydown
                if (oneshot_state[i] == os_idle) {

#                       ifdef OS_STEROIDS_RELAY_MODS
                    if (SHOULD_RELAY_MODS) {
                        // removing the oneshot mod of mods
                        uint8_t mods = get_mods() & ~oneshot[i].modifier;
                        if (mods) {
                            oneshot_pressed_mods[i] = mods;
                        }
                        uint8_t oneshot_mods = get_oneshot_mods() & ~oneshot[i].modifier;
                        if (oneshot_mods) {
                            oneshot_added_mods[i] |= oneshot_mods;
                            register_mods(oneshot_mods);
                        }
                    }
#                       endif  // OS_STEROIDS_RELAY_MODS
                    if (oneshot[i].modifier != 0) {
                        register_mods(oneshot[i].modifier);
                        oneshot_mods_on_steroids |= oneshot[i].modifier;
                    }
                
                    if (oneshot[i].layer != 0) {
#                           ifdef OS_STEROIDS_FREE_LAYER_STACK
                        if (!is_oneshot_layer_active() && !is_oneshot_layer_on_steroids_active()) {
                        // Oneshots can deactivate another layer only when no oneshot layer is ongoing,
                        // not to mess up with the layer stack.
                            uint8_t key_layer = read_source_layers_cache(record->event.key);
                            //uint8_t default_layer = get_highest_layer(default_layer_state);
                            if (SHOULD_FREE_LAYER_STACK && key_layer > oneshot[i].layer) {  // && key_layer != default_layer
                                oneshot_origin_layer[i] = key_layer;
                                layer_off(key_layer);
                            }
                        }
#                           endif  // OS_STEROIDS_FREE_LAYER_STACK
                        layer_on(oneshot[i].layer);
                    }
                    oneshot_tap_time[i] = timer_read();
                    oneshot_state[i] = os_down_unused;
                } else {
                    // The oneshot key is being tapped twice, without another key having being tapped:
                    // cancel the oneshot.
                    clear_oneshot_on_steroids(i);
                }
                return false;
            }

        } else {    // On release

            if (oneshot_state[i] == os_idle) {
                // Case of canceled oneshot (oneshot tapped twice)
                if (keycode == oneshot[i].trigger) { return false; }
                continue;
            }

            if (keycode == oneshot[i].suppressor) {

                if (oneshot_state[i] == os_down_unused && timer_elapsed(oneshot_tap_time[i]) < TAPPING_TERM) {
                    // The oneshot key has been released earlier than the tapping term,
                    // without any other key being pressed in-between:
                    // triggering the oneshot behaviour.
                    oneshot_state[i] = os_up_queued;
                    if (oneshot[i].modifier != 0) {
                        if (!should_mod_be_held_after_tapping_term(oneshot[i].modifier, oneshot[i].trigger)) {
                            unregister_mods(oneshot[i].modifier);
                            add_oneshot_mods(oneshot[i].modifier);
                        }
                    }
#                       ifdef OS_STEROIDS_TIMEOUT
                    idle_timer = (record->event.time + OS_STEROIDS_TIMEOUT) | 1;
#                       endif  // OS_STEROIDS_TIMEOUT
                } else if (oneshot_state[i] == os_down_used) {
                    // A key was tapped when the oneshot key was held:
                    // the oneshot has been used.
                    finish_oneshot_on_steroids(i);
                } else {
                    // The oneshot key has been released after than the tapping term:
                    // cancel the oneshot.
                    clear_oneshot_on_steroids(i);
                }
                if (keycode == oneshot[i].trigger) { should_continue_processing = false; }
                continue;
            }

            // Case of trigger released, with trigger ≠ suppressor
            if (keycode == oneshot[i].trigger) { return false; }
        }
    }
    if (!should_continue_processing) { return false; }

    // Processing other keys
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {

        if (oneshot_state[i] == os_idle) { continue; }

        if (record->event.pressed) {    // On press

            if (is_oneshot_on_steroids_cancel_key(keycode)) {
                // Cancel oneshot on press of specific keys.
                clear_oneshot_on_steroids(i);
                continue;
            }

            if (should_oneshot_on_steroids_stay_pressed(keycode, oneshot[i].trigger, record)) {
#                   ifdef OS_STEROIDS_TIMEOUT
                idle_timer = (record->event.time + OS_STEROIDS_TIMEOUT) | 1;
#                   endif  // OS_STEROIDS_TIMEOUT
                continue;
            }
            
            // Regular key pressed
            switch (oneshot_state[i]) {
                case os_down_unused:
                    // When the mod key is still pressed
                    oneshot_state[i] = os_down_used;
                    break;
                case os_up_queued:
                    oneshot_state[i] = os_up_queued_used;
                    break;
                default:
                    break;
            }

        } else {    // On release

#               ifdef OS_STEROIDS_RELAY_MODS
            if (SHOULD_RELAY_MODS) {
                if (IS_MODIFIER_KEYCODE(keycode)) {
                    if (!modifiers_handling(i, MOD_BIT(keycode))) { return false; }
                }
                if (IS_QK_MOD_TAP(keycode) && !record->tap.count) {
                    uint8_t mod_tap_mods = QK_MOD_TAP_GET_MODS(keycode);
                    if ((mod_tap_mods & 0x10) != 0) { mod_tap_mods <<= 4; }
                    if (!modifiers_handling(i, mod_tap_mods)) { return false; }
                    //if (!modifiers_handling(i, GET_MOD_BITS(QK_MOD_TAP_GET_MODS(keycode)))) { return false; }
                }
            }
#               endif  // OS_STEROIDS_RELAY_MODS

#               ifdef OS_STEROIDS_FREE_LAYER_STACK
            if (IS_QK_MOMENTARY(keycode)) {  // `MO` keys
                if (!layer_switching_keys_handling(i, QK_MOMENTARY_GET_LAYER(keycode), record)) { return false; }
            }
            if (IS_QK_LAYER_TAP_TOGGLE(keycode)) {  // `TT` keys
                if (!layer_switching_keys_handling(i, QK_LAYER_TAP_TOGGLE_GET_LAYER(keycode), record)) { return false; }
            }
            if (IS_QK_LAYER_TAP(keycode) && !record->tap.count) {
                if (!layer_switching_keys_handling(i, QK_LAYER_TAP_GET_LAYER(keycode), record)) { return false; }
            }
#               endif  // OS_STEROIDS_FREE_LAYER_STACK
        }
    }
    return true;
}

void post_process_record_oneshots_on_steroids(uint16_t keycode, keyrecord_t *record){

    // In post_process_record instead of process_record because of a bug
    // when rolling two keys, the first one affected by a custom oneshot
    // and the second one being a mod-tap on base layer.

    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (oneshot_state[i] == os_up_queued_used) {
            // The oneshot has been used.
            finish_oneshot_on_steroids(i);
            continue;
        }
    }
}

__attribute__((weak)) bool is_oneshot_on_steroids_custom_behaviour(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {

        default:
            return true;
    }
}

__attribute__((weak)) bool is_oneshot_on_steroids_cancel_key(uint16_t keycode) {
    switch (keycode) {

        default:
            return false;
    }
}

__attribute__((weak)) bool should_oneshot_on_steroids_stay_pressed(uint16_t keycode, uint16_t trigger, keyrecord_t* record) {

    switch (keycode) {
        // Ignore mod keys.
        case KC_LCTL ... KC_RGUI:
        case KC_HYPR:
        case KC_MEH:
        case QK_ONE_SHOT_MOD ... QK_ONE_SHOT_MOD_MAX:
        // Ignore MO, TO, TG, TT, and OSL layer switch keys.
        case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
        case QK_MOMENTARY ... QK_MOMENTARY_MAX:
        case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:
        case QK_TO ... QK_TO_MAX:
        case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
        case QK_TRI_LAYER_LOWER ... QK_TRI_LAYER_UPPER:
            return true;
    }
    // Ignore oneshot on steroids
    if (is_oneshot_on_steroids(keycode)) { return true; }

    // Ignore tap-hold keys when held
    if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
        if (!record->tap.count) { return true; }
    }

    return false;
}

__attribute__((weak)) bool should_mod_be_held_after_tapping_term(uint8_t mod, uint16_t trigger) {
    // shift and ctrl shouldn't be held after the tapping term,
    // using `add_oneshot_mods()` instead, not to interfere with the mouse
    if (mod & (MOD_MASK_CTRL | MOD_MASK_SHIFT)) { return false; }
    return true;
}

#ifdef OS_STEROIDS_RELAY_MODS_PER_KEY
__attribute__((weak)) bool should_oneshot_on_steroids_relay_mods(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {

        default:
            return true;
    }
}
#endif  // OS_STEROIDS_RELAY_MODS

#ifdef OS_STEROIDS_FREE_LAYER_STACK_PER_KEY
__attribute__((weak)) bool should_oneshot_on_steroids_deactivate_layer(uint16_t keycode, uint8_t layer, keyrecord_t* record) {
    switch (layer) {

        default:
            return true;
    }
}
#endif  // OS_STEROIDS_FREE_LAYER_STACK

