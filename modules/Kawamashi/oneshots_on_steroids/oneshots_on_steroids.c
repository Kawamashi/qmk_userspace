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

/* #if defined OSL_STEROIDS_ABSORB_MODS_PER_KEY && !defined OSL_STEROIDS_ABSORB_MODS
#error "OSL_STEROIDS_ABSORB_MODS_PER_KEY needs OSL_STEROIDS_ABSORB_MODS to work. Please define OSL_STEROIDS_ABSORB_MODS."
#endif */

#if defined OS_STEROIDS_FREE_LAYER_STACK_PER_KEY && !defined OS_STEROIDS_FREE_LAYER_STACK
#error "OS_STEROIDS_FREE_LAYER_STACK_PER_KEY needs OS_STEROIDS_FREE_LAYER_STACK to work. Please define OS_STEROIDS_FREE_LAYER_STACK."
#endif

static oneshot_state_t oneshot_state[OS_STEROIDS_COUNT] = { [0 ... OS_STEROIDS_COUNT - 1] = os_idle };
static uint16_t oneshot_tap_time[OS_STEROIDS_COUNT] = { [0 ... OS_STEROIDS_COUNT - 1] = 0 };
static int8_t active_osl_index = -1;


#   if defined OSL_STEROIDS_ABSORB_MODS
static uint8_t oneshot_pressed_mods = 0;
static uint8_t oneshot_added_mods = 0;

bool should_mod_be_released(uint8_t index, uint8_t mod) {
    if (mod & oneshot_pressed_mods) {
        oneshot_pressed_mods &= ~mod;
        if (should_osl_on_steroids_absorb_mods(oneshot[index].trigger)) {
            switch (oneshot_state[index]) {
                case os_down_unused:
                case os_up_queued:
                    oneshot_added_mods |= mod;
                    return false;
                default:
                    return true;
            }
        }
    }
    return true;
}

bool has_mod_been_absorbed_by_osl(uint8_t mod) {
    return (oneshot_added_mods) & mod;
};
#   endif  // OSL_STEROIDS_ABSORB_MODS


#   ifdef OS_STEROIDS_FREE_LAYER_STACK
static uint8_t oneshot_origin_layer = 0;

// Handles `LT`, `MO` and `TT` keys.
bool layer_switching_keys_handling(uint8_t i, uint8_t key_layer, keyrecord_t *record) {
    if (SHOULD_FREE_LAYER_STACK && key_layer == oneshot_origin_layer) {
        oneshot_origin_layer = 0;
        return false;  // Skip default handling.
    }
    return true;
}
#   endif  // OS_STEROIDS_FREE_LAYER_STACK


#   ifdef OS_STEROIDS_TIMEOUT
static uint16_t idle_timer = 0;
static uint8_t active_os_index = -1;

void housekeeping_task_oneshots_on_steroids(void) {
    if (idle_timer && timer_expired(timer_read(), idle_timer)) {
        clear_oneshots_on_steroids();
    }
}
#   endif  // OS_STEROIDS_TIMEOUT


void unregister_mods_on_steroids(uint8_t mods) {
#       ifdef DUMMY_MOD_NEUTRALIZER_KEYCODE
     neutralize_flashing_modifiers(mods);
#       endif // DUMMY_MOD_NEUTRALIZER_KEYCODE
    unregister_mods(mods);
}

void deactivate_oneshot_on_steroids(int8_t index) {
    if (oneshot_state[index] != os_idle) {

        if (oneshot[index].modifier != 0) {
#               ifdef OSL_STEROIDS_ABSORB_MODS
            bool should_unregister_mod = true;
            if (active_osl_index != -1 && index != active_osl_index) {
                should_unregister_mod = should_mod_be_released(active_osl_index, oneshot[index].modifier);
            }
            if (should_unregister_mod) {
#           endif  // OSL_STEROIDS_ABSORB_MODS

                switch (oneshot_state[index]) {
                    case os_down_unused:
                        unregister_mods_on_steroids(oneshot[index].modifier);
                        break;
                    case os_down_used:
                        unregister_mods(oneshot[index].modifier);
                        break;
                    case os_up_queued:
                        if (should_mod_be_held_after_tapping_term(oneshot[index].modifier, oneshot[index].trigger)) {
                            unregister_mods_on_steroids(oneshot[index].modifier);
                        } else {
                            del_oneshot_mods(oneshot[index].modifier);
                        }
                        break;
                    case os_up_queued_used:
                        if (should_mod_be_held_after_tapping_term(oneshot[index].modifier, oneshot[index].trigger)) {
                            unregister_mods(oneshot[index].modifier);
                        }
                        break;
                    default:
                        break;
                }
                
#               ifdef OSL_STEROIDS_ABSORB_MODS
            }
#               endif  // OSL_STEROIDS_ABSORB_MODS
        }

        if (oneshot[index].layer != 0) {
            layer_off(oneshot[index].layer);

            if (index == active_osl_index) {
                active_osl_index = -1;
                // If there is no other OSL active,
                // restore oneshot_origin_layer
                // and unregister the mods absorbed by the OSL.
#                   ifdef OS_STEROIDS_FREE_LAYER_STACK
                if (oneshot_origin_layer != 0) {
                    layer_on(oneshot_origin_layer);
                    oneshot_origin_layer = 0;
                }
#                   endif  // OS_STEROIDS_FREE_LAYER_STACK
#                   ifdef OSL_STEROIDS_ABSORB_MODS
                if (oneshot_added_mods != 0) {
                    unregister_mods_on_steroids(oneshot_added_mods);
                    oneshot_pressed_mods = 0;
                    oneshot_added_mods = 0;
                }
#                   endif  // OSL_STEROIDS_ABSORB_MODS
            }
        }
        oneshot_state[index] = os_idle;
#           ifdef OS_STEROIDS_TIMEOUT
        if (index == active_os_index) {
            idle_timer = 0;
            active_os_index = -1;
        }
#           endif  // OS_STEROIDS_TIMEOUT
    }
}

void cancel_oneshot_on_steroids(uint16_t keycode) {

    int8_t index = get_oneshot_on_steroids_index(keycode);
    if (index != -1) {
        deactivate_oneshot_on_steroids(index);
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

uint8_t get_oneshot_layer_on_steroids(void) {
    if (active_osl_index != -1) {
        if (oneshot_state[active_osl_index] > os_down_used) { return oneshot[active_osl_index].layer; }
    }
    return 0;
}

bool is_oneshot_on_steroids(uint16_t keycode) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (keycode == oneshot[i].trigger) { return true; }
    }
    return false;
}

bool is_oneshot_layer_on_steroids(uint16_t keycode) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (keycode == oneshot[i].trigger && oneshot[i].layer != 0) { return true; }
    }
    return false;
}

bool is_oneshot_mod_on_steroids(uint16_t keycode) {
    return is_oneshot_on_steroids(keycode) && !is_oneshot_layer_on_steroids(keycode);
}

bool is_oneshot_layer_on_steroids_active(void) {
    if (active_osl_index != -1) {
        return oneshot_state[active_osl_index] > os_down_used;
    }
    return false;
}

void clear_oneshots_on_steroids(void) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        deactivate_oneshot_on_steroids(i);
    }
}

void reset_oneshot_layer_on_steroids(void) {
    if (active_osl_index != -1) {
        if (oneshot_state[active_osl_index] > os_down_used) {
            deactivate_oneshot_on_steroids(active_osl_index);
        }
    }
}

void del_oneshot_mods_on_steroids(uint8_t mods) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (oneshot_state[i] != os_idle){
            if ((oneshot[i].modifier & mods) != 0) {
                deactivate_oneshot_on_steroids(i);
#               ifdef OSL_STEROIDS_ABSORB_MODS
            } else if ((oneshot_added_mods & mods) != 0) {
                // Case of OSL carrying modifiers
                // In this case, we must remove modifiers w/o cancelling the OSL.
                unregister_mods_on_steroids(mods);
                oneshot_added_mods &= ~mods;
#               endif  // OSL_STEROIDS_ABSORB_MODS
            }
        }
    }
}

void clear_oneshot_mods_on_steroids(void) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (oneshot_state[i] != os_idle){
            if (oneshot[i].modifier != 0) {
                deactivate_oneshot_on_steroids(i);
#               ifdef OSL_STEROIDS_ABSORB_MODS
            } else if (oneshot_added_mods != 0) {
                // Case of OSL carrying modifiers
                // In this case, we must remove modifiers w/o cancelling the OSL.
                unregister_mods_on_steroids(oneshot_added_mods);
                oneshot_added_mods = 0;
#               endif  // OSL_STEROIDS_ABSORB_MODS
            }
        }
    }
}


bool process_record_oneshots_on_steroids(uint16_t keycode, keyrecord_t *record){
    
    bool should_continue_processing = true;

    if (!is_oneshot_on_steroids_custom_behaviour(keycode, record)) { return false; }

    // Processing triggers and suppressors
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {

        if (record->event.pressed) {    // On press

            if (keycode == oneshot[i].trigger) {
                // Trigger keydown
                if (oneshot_state[i] == os_idle) {

                    if (oneshot[i].modifier != 0) { register_mods(oneshot[i].modifier); }
                
                    if (oneshot[i].layer != 0) {
#                           ifdef OS_STEROIDS_FREE_LAYER_STACK
                        if (!is_oneshot_layer_active() && !is_oneshot_layer_on_steroids_active()) {
                        // OSL on steroids can deactivate another layer only if there is no ongoing oneshot layer,
                        // not to mess up with the layer stack.
                            uint8_t key_layer = read_source_layers_cache(record->event.key);
                            //uint8_t default_layer = get_highest_layer(default_layer_state);
                            if (SHOULD_FREE_LAYER_STACK && key_layer > oneshot[i].layer) {  // && key_layer != default_layer
                                oneshot_origin_layer = key_layer;
                                layer_off(key_layer);
                            }
                        }
#                           endif  // OS_STEROIDS_FREE_LAYER_STACK

#                           ifdef OSL_STEROIDS_ABSORB_MODS
                        if (should_osl_on_steroids_absorb_mods(oneshot[i].trigger)) {
#                               ifdef OS_STEROIDS_TIMEOUT
                            if (active_os_index != -1) {
                                if ((get_mods() | get_oneshot_mods()) & oneshot[active_os_index].modifier) {
                                    // preventing OSM tapped before from idling
                                    idle_timer = 0;
                                    active_os_index = -1;
                                }
                            }
#                               endif  // OS_STEROIDS_TIMEOUT
                            // removing the oneshot mod of mods
                            uint8_t mods = get_mods() & ~oneshot[i].modifier;
                            if (mods) {
                                oneshot_pressed_mods |= mods;
                            }
                            uint8_t oneshot_mods = get_oneshot_mods() & ~oneshot[i].modifier;
                            if (oneshot_mods) {
                                del_oneshot_mods(oneshot_mods);
                                oneshot_added_mods |= oneshot_mods;
                                register_mods(oneshot_mods);
                            }
                        }
#                           endif  // OSL_STEROIDS_ABSORB_MODS

                        layer_on(oneshot[i].layer);
                        active_osl_index = i;
                    }
                    oneshot_tap_time[i] = timer_read();
                    oneshot_state[i] = os_down_unused;
                } else {
                    // The oneshot key is being tapped twice, without another key having being tapped:
                    // cancel the oneshot.
                    deactivate_oneshot_on_steroids(i);
                }
                should_continue_processing = false;
                continue;
            }

        } else {    // On release

            if (oneshot_state[i] == os_idle) {
                // Case of canceled oneshot (oneshot tapped twice)
                if (keycode == oneshot[i].trigger) { should_continue_processing = false; }
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
                    active_os_index = i;
#                       endif  // OS_STEROIDS_TIMEOUT
                } else {
                    // The oneshot key has been released after the tapping term
                    // or a key was tapped when the oneshot key was held:
                    // cancel the oneshot.
                    deactivate_oneshot_on_steroids(i);
                }
            }

            // Trigger released
            if (keycode == oneshot[i].trigger) { should_continue_processing = false; }
        }
    }

    // Processing other keys
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {

        if (oneshot_state[i] == os_idle) { continue; }
        if (keycode == oneshot[i].trigger) { continue; }

        if (record->event.pressed) {    // On press

            if (is_oneshot_on_steroids_cancel_key(keycode)) {
                // Cancel oneshot on press of specific keys.
                deactivate_oneshot_on_steroids(i);
                continue;
            }

            if (should_oneshot_on_steroids_stay_pressed(keycode, oneshot[i].trigger, record)) {
#                   ifdef OS_STEROIDS_TIMEOUT
                if (idle_timer) {idle_timer = (record->event.time + OS_STEROIDS_TIMEOUT) | 1;}
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
                    if (is_oneshot_on_steroids(keycode)) {
                        // because process_record_oneshots_on_steroids() will return false,
                        // halting all further processing,
                        // post_process_record_oneshots_on_steroids will never be reached.
                        deactivate_oneshot_on_steroids(i);
                    } else {
                        oneshot_state[i] = os_up_queued_used;
                    }
                    break;
                default:
                    break;
            }

        } else {    // On release
            if (i == active_osl_index) {

#                   ifdef OSL_STEROIDS_ABSORB_MODS
                uint8_t mod_being_released = 0;
                if (IS_MODIFIER_KEYCODE(keycode)) {
                    mod_being_released = MOD_BIT(keycode);

                } else if (IS_QK_MOD_TAP(keycode) && !record->tap.count) {
                    mod_being_released = QK_MOD_TAP_GET_MODS(keycode);
                    if ((mod_being_released & 0x10) != 0) { mod_being_released <<= 4; }

                } else if (IS_QK_ONE_SHOT_MOD(keycode) && !record->tap.count) {
                    mod_being_released = QK_ONE_SHOT_MOD_GET_MODS(keycode);
                    if ((mod_being_released & 0x10) != 0) { mod_being_released <<= 4; }
                }

                if (mod_being_released) {
                    if (!should_mod_be_released(i, mod_being_released)) {
                        should_continue_processing = false;
                        continue;
                    }
                }
#                   endif  // OSL_STEROIDS_ABSORB_MODS

#                   ifdef OS_STEROIDS_FREE_LAYER_STACK
                if (IS_QK_MOMENTARY(keycode)) {  // `MO` keys
                    if (!layer_switching_keys_handling(i, QK_MOMENTARY_GET_LAYER(keycode), record)) {
                        should_continue_processing = false;
                        continue;
                    }
                }
                if (IS_QK_LAYER_TAP_TOGGLE(keycode)) {  // `TT` keys
                    if (!layer_switching_keys_handling(i, QK_LAYER_TAP_TOGGLE_GET_LAYER(keycode), record)) {
                        should_continue_processing = false;
                        continue;
                    }
                }
                if (IS_QK_LAYER_TAP(keycode) && !record->tap.count) {
                    if (!layer_switching_keys_handling(i, QK_LAYER_TAP_GET_LAYER(keycode), record)) {
                        should_continue_processing = false;
                        continue;
                    }
                }
#                   endif  // OS_STEROIDS_FREE_LAYER_STACK
            }
        }
    }
    return should_continue_processing;
}

void post_process_record_oneshots_on_steroids(uint16_t keycode, keyrecord_t *record){

    // In post_process_record instead of process_record because of a bug
    // when rolling two keys, the first one affected by a custom oneshot
    // and the second one being a mod-tap on base layer.

    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (oneshot_state[i] == os_up_queued_used) {
            // The oneshot has been used.
            deactivate_oneshot_on_steroids(i);
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

__attribute__((weak)) bool should_oneshot_on_steroids_stay_pressed(uint16_t keycode, uint16_t oneshot, keyrecord_t* record) {

    // Oneshot on steroids applied one after another
    if (is_oneshot_on_steroids(keycode)) {
        if (is_oneshot_layer_on_steroids(oneshot)) {
            // Two OSL can't be active at the same time:
            // if another OSL is active, it must be reset.
            if (is_oneshot_layer_on_steroids(keycode)) { return false; }
            // keycode is not a OSL, it’s a OSM.
#               ifdef OSM_SHOULD_STAY_ON_OSL_LAYER
            return true;
#               else
            // When using OSM as Callum mods, an OSL tapped before must be reset.
            return false;
#               endif  // OSM_SHOULD_STAY_ON_OSL_LAYER
        } else {
            // oneshot is OSM on steroids
#               ifdef OSL_STEROIDS_ABSORB_MODS
            if (is_oneshot_layer_on_steroids(keycode)) {
                if (should_osl_on_steroids_absorb_mods(keycode)) { return false; }
            }
#               endif  // OSL_STEROIDS_ABSORB_MODS
            // OSM on steroids should stay pressed
            // whether keycode is OSM or OSL on steroids.
            return true;
        }
    }

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

#ifdef OSL_STEROIDS_ABSORB_MODS
__attribute__((weak)) bool should_osl_on_steroids_absorb_mods(uint16_t keycode) {
    switch (keycode) {

        default:
            return true;
    }
}
#endif  // OSL_STEROIDS_ABSORB_MODS

#ifdef OS_STEROIDS_FREE_LAYER_STACK
__attribute__((weak)) bool should_oneshot_on_steroids_deactivate_layer(uint16_t keycode, uint8_t layer, keyrecord_t* record) {
    switch (layer) {

        default:
            return true;
    }
}
#endif  // OS_STEROIDS_FREE_LAYER_STACK

