#include "oneshot.h"

void update_oneshot(oneshot_state *state, uint16_t mod, uint16_t trigger, uint16_t keycode, keyrecord_t *record) {

    if (keycode == trigger) {
        if (record->event.pressed) {
            // Trigger keydown
            if (*state == os_up_unqueued) {
                register_code(mod);
            }
            *state = os_down_unused;
        } else {
            // Trigger keyup
            switch (*state) {
                case os_down_unused:
                    // If we didn't use the mod while trigger was held, queue it.
                    *state = os_up_queued;
                    break;
                case os_down_used:
                    // If we did use the mod while trigger was held, unregister it.
                    *state = os_up_unqueued;
                    unregister_code(mod);
                    break;
                default:
                    break;
            }
        }
    } else if (is_oneshot_cancel_key(keycode)) {
        if (record->event.pressed && *state != os_up_unqueued) {
            // Cancel oneshot on designated cancel keydown.
            *state = os_up_unqueued;
            unregister_code(mod);
        }

    } else if (!is_oneshot_ignored_key(keycode)) {
        
        // Regular key released / roll between two regular keys
        if (*state == os_up_queued_used) {
            *state = os_up_unqueued;
            unregister_code(mod);

        } else if (record->event.pressed) {
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
                    *state = os_up_unqueued;
                    unregister_code(mod);
                    break;
                default:
                    break;
            }
        }
    }
}
