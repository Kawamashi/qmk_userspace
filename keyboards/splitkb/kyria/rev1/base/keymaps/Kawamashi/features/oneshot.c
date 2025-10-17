#include "oneshot.h"

oneshot_state os_shft_state = os_idle;
oneshot_state os_ctrl_state = os_idle;
oneshot_state os_alt_state = os_idle;
oneshot_state os_win_state = os_idle;


bool process_oneshot(uint16_t keycode, keyrecord_t *record){

    // Handling Callum's OSM on OS4A layers
  if (!update_oneshot(&os_shft_state, KC_LSFT, OS_SHFT, keycode, record)) { return false; }
  if (!update_oneshot(&os_ctrl_state, KC_LCTL, OS_CTRL, keycode, record)) { return false; }
  if (!update_oneshot(&os_alt_state, KC_LALT, OS_LALT, keycode, record)) { return false; }
  if (!update_oneshot(&os_win_state, KC_LWIN, OS_WIN, keycode, record)) { return false; }
  
  return true;
}

bool update_oneshot(oneshot_state *state, uint16_t mod, uint16_t trigger, uint16_t keycode, keyrecord_t *record) {

    if (keycode == trigger) {
        if (record->event.pressed) {
            // Trigger keydown
            if (*state == os_idle) {
                register_code(mod);
                *state = os_down_unused;
            } else {
                *state = os_idle;
                unregister_code(mod);
            }
            
        } else {
            // Trigger keyup
            switch (*state) {
                case os_down_unused:
                    // If we didn't use the mod while trigger was held, queue it.
                    *state = os_up_queued;
                    break;
                case os_down_used:
                    // If we did use the mod while trigger was held, unregister it.
                    *state = os_idle;
                    unregister_code(mod);
                    break;
                default:
                    break;
            }
        }
        return false;
    }

    if (*state == os_idle) { return true; }

    if (is_oneshot_cancel_key(keycode)) {
        if (record->event.pressed) {
            // Cancel oneshot on designated cancel keydown.
            *state = os_idle;
            unregister_code(mod);
        }
        return true;
    }

    if (!is_oneshot_ignored_key(keycode)) {
        
        // Regular key released / roll between two regular keys
        if (*state == os_up_queued_used) {
            *state = os_idle;
            unregister_code(mod);
            return true;
        }

        if (record->event.pressed) {
            // Regular key pressed
            if (*state == os_up_queued) {
                *state = os_up_queued_used;
            }

        } else {
            // Regular key release
            switch (*state) {
                // When the mod key is still pressed
                case os_down_unused:
                    *state = os_down_used;
                    break;
                // Roll between a mod key and a regular key
                case os_up_queued:
                    *state = os_idle;
                    unregister_code(mod);
                    break;
                default:
                    break;
            }
        }
    }
    return true;
}
