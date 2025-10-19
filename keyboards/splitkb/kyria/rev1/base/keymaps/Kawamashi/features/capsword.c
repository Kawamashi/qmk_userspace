#include "capsword.h"

static bool caps_word_active = false;
static bool caps_list_active = false;

static signed char capslist_countdown = 1;
static unsigned char countdown_end = 6;

static uint16_t idle_timer = 0;


void caps_word_task(void) {
  if (timer_expired(timer_read(), idle_timer)) {
    if (caps_list_active) {
        caps_list_off();
    } else if (caps_word_active) {
        caps_word_off();
    }
  }
}

bool is_caps_lock_on(void) { return host_keyboard_led_state().caps_lock; }
bool is_caps_word_on(void) { return caps_word_active; }
bool is_caps_list_on(void) { return caps_list_active; }

void caps_word_on(void) {
  if (caps_word_active) { return; }

  clear_mods();
  clear_oneshot_mods();

  idle_timer = timer_read() + CAPS_WORD_IDLE_TIMEOUT;
  caps_word_active = true;
}

void caps_list_on(void) {
  if (caps_list_active) { return; }

  idle_timer = timer_read() + CAPS_WORD_IDLE_TIMEOUT;
  caps_list_active = true;
  capslist_countdown = 1;
  countdown_end = 6;
}

void caps_word_off(void) {
  if (!caps_word_active) { return; }

  unregister_weak_mods(MOD_BIT(KC_LSFT));  // Make sure weak shift is off.
  caps_word_active = false;
}

void caps_list_off(void) {
  if (!caps_list_active) { return; }
    
  caps_word_off();
  caps_list_active = false;
}

void caps_lock_off(void) {
  if (is_caps_lock_on()) { tap_code(KC_CAPS); }
}

void caps_word_toggle(void) {
  if (caps_list_active) {
      caps_list_active = false;
      caps_word_on();

  } else if (caps_word_active) {
      caps_word_off();

  } else {
      // Deactivating Caps Lock and Caps List when Caps Word activates.
      caps_lock_off();
      caps_word_on();
  }
}

void caps_list_toggle(void) {
  if (caps_list_active) {
      caps_list_off();
  } else {
      caps_lock_off();
      caps_list_on();
      caps_word_on();
  }
}

void caps_lock_toggle(void) {
  if (!is_caps_lock_on()) {
      caps_word_off();
      caps_list_off();
  }
  tap_code(KC_CAPS);
}

bool update_capslist_countdown(signed char i) {
  capslist_countdown = capslist_countdown + i;
  return true;
}

bool word_check(uint16_t keycodes[], uint8_t num_keycodes, unsigned char new_countdown_end) {
  for (int i = 0; i < num_keycodes; ++i) {
    if (get_recent_keycode(- 2 - i) != keycodes[num_keycodes - 1 - i]) { return false; }
  }
  countdown_end = new_countdown_end;
  return true;
}


bool process_caps_word(uint16_t keycode, keyrecord_t* record) {

  switch (keycode) {
    case CAPSWORD:
        if (record->event.pressed) {
            caps_word_toggle();
        }
        return false;
    
    case CAPSLIST:
        if (record->event.pressed) {
            caps_list_toggle();
        }
        return false;

    case KC_CAPS:
        if (record->event.pressed) {
            caps_lock_toggle();
        }
        return true;
  }


  if (!caps_word_active && !caps_list_active) { return true; }

  // Caps word or caps list is active //

  if (caps_word_active) {clear_weak_mods();}

  // No action on keyrelease
  if (!record->event.pressed) { return true; }

  const uint8_t mods = get_mods() | get_oneshot_mods();
  if (mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_ALGR))) {
    // Avoid interfering with ctrl, alt and gui.
    caps_word_off();
    return true;
  }
  
  if (was_keycode_replaced()) {
    keycode = get_recent_keycode(-1);
  } else {
    switch (keycode) {
        case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:
          return true;
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
          // Earlier return if this has not been considered tapped yet
          if (record->tap.count == 0) { return true; }
          keycode = tap_hold_extractor(keycode);    // Get tapping keycode.
          break;
    }
  }

  idle_timer = record->event.time + CAPS_WORD_IDLE_TIMEOUT;

  if (caps_word_active) { update_caps_word(keycode); }
  if (caps_list_active && !caps_word_active) { reactivate_caps_word(keycode); }     // Do not merge into a single 'if' block !

  return true;
}

void update_caps_word(uint16_t keycode) {

  if (caps_word_press_user(keycode)) {
      // Invert on shift
      if (get_oneshot_mods() & MOD_MASK_SHIFT) {
        set_weak_mods(get_weak_mods() ^ MOD_BIT(KC_LSFT));
        del_oneshot_mods(MOD_MASK_SHIFT);
      }
      send_keyboard_report();
      return;
  }

  caps_word_off();
}

void reactivate_caps_word(uint16_t keycode) {
    
  if (should_continue_caps_list(keycode)) {

      if (list_separator()) {
        caps_word_on();  // Reactivate Caps Word for a new word
        capslist_countdown = 1;
        return;
      }
      if (capslist_countdown < countdown_end) { return; }
  }
  caps_list_off();
}