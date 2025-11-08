#include "oneshot.h"

oneshot_state_t oneshot_state[OS_COUNT] = { [0 ... OS_COUNT - 1] = os_idle };

static uint16_t idle_timer = 0;

void oneshot_task(void) {
  if (timer_expired(timer_read(), idle_timer)) {
    clear_oneshot();
  }
}

void clear_oneshot(void) {
  for (uint8_t i = 0; i < OS_COUNT; i++) {
    if (oneshot_state[i] == os_up_queued) {
      oneshot_state[i] = os_idle;
      unregister_code(oneshot_keys[i].modifier);
    }
  }
}

bool process_oneshot(uint16_t keycode, keyrecord_t *record){

  for (uint8_t i = 0; i < OS_COUNT; i++) {
    if (keycode == oneshot_keys[i].keycode) {
      if (record->event.pressed) {
        // Trigger keydown
        if (oneshot_state[i] == os_idle) {
            register_code(oneshot_keys[i].modifier);
            oneshot_state[i] = os_down_unused;
        } else {
            oneshot_state[i] = os_idle;
            unregister_code(oneshot_keys[i].modifier);
        }
        
      } else {
        // Trigger keyup
        switch (oneshot_state[i]) {
            case os_down_unused:
                // If we didn't use the mod while trigger was held, queue it.
                oneshot_state[i] = os_up_queued;
                idle_timer = record->event.time + ONESHOT_TIMEOUT;
                break;
            case os_down_used:
                // If we did use the mod while trigger was held, unregister it.
                oneshot_state[i] = os_idle;
                unregister_code(oneshot_keys[i].modifier);
                break;
            default:
                break;
        }
      }
      return false;
    }
  }

  for (uint8_t i = 0; i < OS_COUNT; i++) {
    if (oneshot_state[i] == os_idle) { continue; }

    if (is_oneshot_cancel_key(keycode)) {
        if (record->event.pressed) {
            // Cancel oneshot on press of specific keys.
            oneshot_state[i] = os_idle;
            unregister_code(oneshot_keys[i].modifier);
        }
        continue;
    }

    if (should_oneshot_stay_pressed(keycode)) {
        if (record->event.pressed) {
            idle_timer = record->event.time + ONESHOT_TIMEOUT;
        }
        continue;
    }

    // Regular key released / roll between two regular keys
    if (oneshot_state[i] == os_up_queued_used) {
        oneshot_state[i] = os_idle;
        unregister_code(oneshot_keys[i].modifier);
        continue;
    }

    if (record->event.pressed) {
        // Regular key pressed
        if (oneshot_state[i] == os_up_queued) {
            oneshot_state[i] = os_up_queued_used;
        }

    } else {
        // Regular key release
        switch (oneshot_state[i]) {
            // When the mod key is still pressed
            case os_down_unused:
                oneshot_state[i] = os_down_used;
                break;
            // Roll between a mod key and a regular key
            case os_up_queued:
                oneshot_state[i] = os_idle;
                unregister_code(oneshot_keys[i].modifier);
                break;
            default:
                break;
        }
    }
  }
  return true;
}
