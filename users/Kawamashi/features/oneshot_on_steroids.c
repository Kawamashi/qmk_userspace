#include "oneshot_on_steroids.h"

oneshot_state_t oneshot_state[OS_COUNT] = { [0 ... OS_COUNT - 1] = os_idle };
//oneshot_state_t oneshot_layer_state[OS_COUNT] = { [0 ... OS_COUNT - 1] = os_idle };

static uint16_t idle_timer = 0;
uint16_t oneshot_holding_time[OS_COUNT] = { [0 ... OS_COUNT - 1] = 0 };

void oneshot_task(void) {
  if (idle_timer && timer_expired(timer_read(), idle_timer)) {
    clear_oneshot();
    idle_timer = 0;
  }
}

void clear_oneshot(void) {
  for (uint8_t i = 0; i < OS_COUNT; i++) {
    if (oneshot_state[i] == os_up_queued) {
      oneshot_state[i] = os_idle;
      unregister_code(oneshot[i].modifier);
    }
  }
}

bool process_oneshot_on_steroids(uint16_t keycode, keyrecord_t *record){

  //bool pass = true;

  for (uint8_t i = 0; i < OS_COUNT; i++) {

    //if (record->event.pressed) {

    if (keycode == oneshot[i].trigger) {
      if (record->event.pressed) {
        // Trigger keydown
        if (oneshot_state[i] == os_idle) {
            register_code(oneshot[i].modifier);
            oneshot_holding_time[i] = timer_read();
            oneshot_state[i] = os_down_unused;
        } else {
            oneshot_state[i] = os_idle;
            unregister_code(oneshot[i].modifier);
        }
        return false;
      //}

      } else {

      //if (keycode == oneshot[i].suppressor){
        switch (oneshot_state[i]) {
            case os_down_unused:
                if (timer_elapsed(oneshot_holding_time[i]) > TAPPING_TERM) {
                    // The key has been held longer than the tapping term.
                    // It’s not considered as one-shot key.
                    oneshot_state[i] = os_idle;
                    unregister_code(oneshot[i].modifier);
                    break;
                } else {
                    // If we didn't use the mod while trigger was held, queue it.
                    oneshot_state[i] = os_up_queued;
                    idle_timer = (record->event.time + ONESHOT_TIMEOUT) | 1;
                    break;
                }
            case os_down_used:
                // If we did use the mod while trigger was held, unregister it.
                oneshot_state[i] = os_idle;
                unregister_code(oneshot[i].modifier);
                break;
            case os_idle:
                // If the oneshot hasn’t been triggered and the suppressor is not the trigger,
                // process the suppressor normally.
                if (keycode != oneshot[i].trigger) { return true; }
            default:
                break;
          }
      }
        return false;
/*       }
      if (keycode == oneshot[i].trigger) { return false; }
      continue; */
    }
  }

  for (uint8_t i = 0; i < OS_COUNT; i++) {
    if (oneshot_state[i] == os_idle) { continue; }

    if (is_oneshot_cancel_key(keycode)) {
        if (record->event.pressed) {
            // Cancel oneshot on press of specific keys.
            oneshot_state[i] = os_idle;
            unregister_code(oneshot[i].modifier);
        }
        continue;
    }

    if (should_oneshot_stay_pressed(keycode)) {
        if (record->event.pressed) {
            idle_timer = (record->event.time + ONESHOT_TIMEOUT) | 1;
        }
        continue;
    }

    // Regular key released / roll between two regular keys
    if (oneshot_state[i] == os_up_queued_used) {
        oneshot_state[i] = os_idle;
        unregister_code(oneshot[i].modifier);
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
                unregister_code(oneshot[i].modifier);
                break;
            default:
                break;
        }
    }
  }
  return true;
}
