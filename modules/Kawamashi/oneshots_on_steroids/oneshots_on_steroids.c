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
#error "Oneshots on Steroids can't work when NO_ACTION_ONESHOT is defined. Please undefined NO_ACTION_ONESHOT."
#endif  // NO_ACTION_ONESHOT

static oneshot_state_t oneshot_state[OS_STEROIDS_COUNT] = { [0 ... OS_STEROIDS_COUNT - 1] = os_idle };

static uint16_t idle_timer = 0;

static uint16_t oneshot_tap_time[OS_STEROIDS_COUNT] = { [0 ... OS_STEROIDS_COUNT - 1] = 0 };

#if defined OS_STEROIDS_RELAY_MODS || defined OS_STEROIDS_RELAY_MODS_PER_KEY
static uint8_t oneshot_pressed_mods[OS_STEROIDS_COUNT] = { [0 ... OS_STEROIDS_COUNT - 1] = 0 };
static uint8_t oneshot_added_mods[OS_STEROIDS_COUNT] = { [0 ... OS_STEROIDS_COUNT - 1] = 0 };
#endif  // OS_STEROIDS_RELAY_MODS

#ifdef OS_STEROIDS_RELAY_MODS_PER_KEY
#   define SHOULD_RELAY_MODS should_oneshot_on_steroids_relay_mods(oneshot[i].trigger)
#else
#   define SHOULD_RELAY_MODS true
#endif

//static uint8_t oneshot_origin_layer[OS_STEROIDS_COUNT] = { [0 ... OS_STEROIDS_COUNT - 1] = 0 };

#ifdef ONESHOT_TIMEOUT
void housekeeping_task_oneshots_on_steroids(void) {
    if (idle_timer && timer_expired(timer_read(), idle_timer)) {
        clear_all_oneshots_on_steroids();
        idle_timer = 0;
    }
}
#endif  // ONESHOT_TIMEOUT

void clear_oneshot_on_steroids(uint8_t index) {
    oneshot_state[index] = os_idle;
    if (oneshot[index].modifier != 0) { 
        if (should_mod_be_held(oneshot[index].modifier, oneshot[index].trigger)) {
#               ifdef DUMMY_MOD_NEUTRALIZER_KEYCODE
            //neutralize_flashing_modifiers(MOD_BIT(KC_LEFT_GUI));
            neutralize_flashing_modifiers(oneshot[index].modifier);
#               endif // DUMMY_MOD_NEUTRALIZER_KEYCODE
            unregister_mods(oneshot[index].modifier);
        } else {
            del_oneshot_mods(oneshot[index].modifier);
        }
    }

#       ifdef OS_STEROIDS_RELAY_MODS
    if (oneshot_added_mods[index] != 0) {
#           ifdef DUMMY_MOD_NEUTRALIZER_KEYCODE
        //neutralize_flashing_modifiers(MOD_BIT(KC_LEFT_GUI));
        neutralize_flashing_modifiers(oneshot_added_mods[index]);
#           endif // DUMMY_MOD_NEUTRALIZER_KEYCODE
        unregister_mods(oneshot_added_mods[index]);
        oneshot_added_mods[index] = 0;
    }
#       endif  // OS_STEROIDS_RELAY_MODS

    if (oneshot[index].layer != 0) { layer_off(oneshot[index].layer); }
}

void clear_all_oneshots_on_steroids(void) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (oneshot_state[i] == os_up_queued) { clear_oneshot_on_steroids(i); }
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
#                   ifdef DUMMY_MOD_NEUTRALIZER_KEYCODE
                //neutralize_flashing_modifiers(MOD_BIT(KC_LEFT_GUI));
                neutralize_flashing_modifiers(oneshot_added_mods[i]);
#                   endif // DUMMY_MOD_NEUTRALIZER_KEYCODE
                unregister_mods(oneshot_added_mods[i]);
                oneshot_added_mods[i] = 0;
#               endif  // OS_STEROIDS_RELAY_MODS
            }
        }
    }
}

bool is_oneshot_on_steroids(uint16_t keycode) {
    for (uint8_t i = 0; i < OS_STEROIDS_COUNT; i++) {
        if (keycode == oneshot[i].trigger) { return true; }
    }
    return false;
}

#ifdef OS_STEROIDS_RELAY_MODS
bool handle_mods(uint8_t index, uint8_t mod) {
    if (mod & oneshot_pressed_mods[index]) {

        switch (oneshot_state[index]) {
            case os_down_unused:
            case os_up_queued:
                //oneshot_added_mods[index] = mod;
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


bool process_record_oneshots_on_steroids(uint16_t keycode, keyrecord_t *record){
    
    bool should_continue_processing = true;

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
                        uint8_t oneshot_mod = get_oneshot_mods() & ~oneshot[i].modifier;
                        if (oneshot_mod) {
                            oneshot_added_mods[i] |= oneshot_mod;
                            register_mods(oneshot_mod);
                        }
                    }
#                       endif  // OS_STEROIDS_RELAY_MODS
                    if (oneshot[i].modifier != 0) { register_mods(oneshot[i].modifier); }
                
                    if (oneshot[i].layer != 0) { 
/*                         uint8_t key_layer = read_source_layers_cache(record->event.key);
                        uint8_t default_layer = get_highest_layer(default_layer_state);
                        if (key_layer != default_layer) {
                            oneshot_origin_layer[i] = key_layer;
                            layer_off(key_layer);
                        } else {
                            // It's important to reinitialize oneshot_origin_layer
                            // ex: same OSLs, one on base layer, one on a secondary layer
                            oneshot_origin_layer[i] = 0;
                        } */
                        layer_on(oneshot[i].layer);
                    }
                    oneshot_tap_time[i] = timer_read();
                    oneshot_state[i] = os_down_unused;
                } else {
                    clear_oneshot_on_steroids(i);
                    //if (oneshot_origin_layer[i] != 0) { layer_on(oneshot_origin_layer[i]); }
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
/*                 if (oneshot[i].modifier != 0) {
                    unregister_mods(oneshot[i].modifier);
                } */
/* #                   ifdef OS_STEROIDS_RELAY_MODS
                if (oneshot_added_mods[i] != 0) {
                    unregister_mods(oneshot_added_mods[i]);
                }
#                   endif  // OS_STEROIDS_RELAY_MODS */

                if (oneshot_state[i] == os_down_unused && timer_elapsed(oneshot_tap_time[i]) < TAPPING_TERM) {

                    oneshot_state[i] = os_up_queued;
                    if (oneshot[i].modifier != 0) {
                        if (!should_mod_be_held(oneshot[i].modifier, oneshot[i].trigger)) {
                            unregister_mods(oneshot[i].modifier);
                            add_oneshot_mods(oneshot[i].modifier);
                        }
                    }
/*                     if (oneshot[i].modifier != 0) { add_oneshot_mods(oneshot[i].modifier); }
#                       ifdef OS_STEROIDS_RELAY_MODS
                    if (oneshot_added_mods[i] != 0) { add_oneshot_mods(oneshot_added_mods[i]); }
#                       endif  // OS_STEROIDS_RELAY_MODS */
#                       ifdef ONESHOT_TIMEOUT
                    idle_timer = (record->event.time + ONESHOT_TIMEOUT) | 1;
#                       endif  // ONESHOT_TIMEOUT

                } else {

#                       ifdef OS_STEROIDS_RELAY_MODS
                    if (oneshot_added_mods[i] != 0) {
#                           ifdef DUMMY_MOD_NEUTRALIZER_KEYCODE
                        //neutralize_flashing_modifiers(MOD_BIT(KC_LEFT_GUI));
                        neutralize_flashing_modifiers(oneshot_added_mods[i]);
#                           endif // DUMMY_MOD_NEUTRALIZER_KEYCODE
                        unregister_mods(oneshot_added_mods[i]);
                        oneshot_added_mods[i] = 0;
                    }
#                       endif  // OS_STEROIDS_RELAY_MODS

                    oneshot_state[i] = os_idle;
                    if (oneshot[i].modifier != 0) {
#                           ifdef DUMMY_MOD_NEUTRALIZER_KEYCODE
                        //neutralize_flashing_modifiers(MOD_BIT(KC_LEFT_GUI));
                        neutralize_flashing_modifiers(oneshot[i].modifier);
#                           endif // DUMMY_MOD_NEUTRALIZER_KEYCODE
                        unregister_mods(oneshot[i].modifier);
                    }
                    if (oneshot[i].layer != 0) { layer_off(oneshot[i].layer); }
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

            if (should_oneshot_on_steroids_stay_pressed(keycode, oneshot[i].trigger)) {
#                   ifdef ONESHOT_TIMEOUT
                idle_timer = (record->event.time + ONESHOT_TIMEOUT) | 1;
#                   endif  // ONESHOT_TIMEOUT
                continue;
            }

            if (IS_QK_MOD_TAP(keycode)) {
                // Étoffer ces cas.
                // Il faut qu’ils décallent le chrono comme le bloc au-dessus
                if (!record->tap.count) { return true; }
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
                    if (!handle_mods(i, MOD_BIT(keycode))) { return false; }
                } else if (IS_QK_MOD_TAP(keycode) && !record->tap.count) {
                    //uint8_t mod_tap_mods = QK_MOD_TAP_GET_MODS(keycode)
                    if (!handle_mods(i, GET_MOD_BITS(QK_MOD_TAP_GET_MODS(keycode)))) { return false; }
                }
            }
#               endif  // OS_STEROIDS_RELAY_MODS
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
            oneshot_state[i] = os_idle;

#               ifdef OS_STEROIDS_RELAY_MODS
            if (oneshot_added_mods[i] != 0) {
/* #                   ifdef DUMMY_MOD_NEUTRALIZER_KEYCODE
                //neutralize_flashing_modifiers(MOD_BIT(KC_LEFT_GUI));
                neutralize_flashing_modifiers(oneshot_added_mods[i]);
#                   endif // DUMMY_MOD_NEUTRALIZER_KEYCODE */
                unregister_mods(oneshot_added_mods[i]);
                oneshot_added_mods[i] = 0;
            }
#               endif  // OS_STEROIDS_RELAY_MODS

            if (oneshot[i].modifier != 0) {
                if (should_mod_be_held(oneshot[i].modifier, oneshot[i].trigger)) {
/* #                       ifdef DUMMY_MOD_NEUTRALIZER_KEYCODE
                    //neutralize_flashing_modifiers(MOD_BIT(KC_LEFT_GUI));
                    neutralize_flashing_modifiers(mod);
#                       endif // DUMMY_MOD_NEUTRALIZER_KEYCODE */
                    unregister_mods(oneshot[i].modifier);
                }
            }

            if (oneshot[i].layer != 0) {
                layer_off(oneshot[i].layer);
                if (oneshot[i].suppressor == oneshot[i].trigger) {
                    //if (oneshot_origin_layer[i] != 0) { layer_on(oneshot_origin_layer[i]); }
                }
            }
            continue;
        }
    }
}

__attribute__((weak)) bool is_oneshot_on_steroids_cancel_key(uint16_t keycode) {
    switch (keycode) {

        default:
            return false;
    }
}

__attribute__((weak)) bool should_oneshot_on_steroids_stay_pressed(uint16_t keycode, uint16_t trigger) {
    switch (keycode) {
        case MOD_BIT(KC_LCTL):
        
        default:
            return false;
    }
}

__attribute__((weak)) bool should_mod_be_held(uint8_t mod, uint16_t trigger) {
    // shift and ctrl shouldn't be held,
    // not to interfere with the mouse
    return (mod & (MOD_MASK_CTRL | MOD_MASK_SHIFT)) == 0;
/*     switch (mod) {
        case MOD_BIT(KC_LCTL):
        case MOD_BIT(KC_LSFT):
            return false;
        default:
            return true;
    } */
}

#ifdef OS_STEROIDS_RELAY_MODS_PER_KEY
__attribute__((weak)) bool should_oneshot_on_steroids_relay_mods(uint16_t keycode) {
    switch (keycode) {

        default:
            return false;
    }
}
#endif  // OS_STEROIDS_RELAY_MODS

