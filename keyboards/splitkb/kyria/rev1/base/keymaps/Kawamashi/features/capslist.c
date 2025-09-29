#include "capslist.h"

static bool caps_list_active = false;
//static unsigned short int capslist_countdown = 0;
unsigned short int capslist_countdown = 0;
//static unsigned short int countdown_end = 5;
unsigned short int countdown_end = 5;

bool is_caps_list_on(void) { return caps_list_active; }

void enable_caps_list(void) {
    if (is_caps_lock_on()) { tap_code(KC_CAPS); }
    caps_word_on();
    caps_list_active = true;
    capslist_countdown = 0;
    countdown_end = 5;
}

void disable_caps_list(void) {
    caps_word_off();
    caps_list_active = false;
}

void toggle_caps_list(void) {
    if (caps_list_active) {
      disable_caps_list();
    } else {
      enable_caps_list();
    }
}

bool process_caps_list(uint16_t keycode, keyrecord_t *record) {
    // Handle the custom keycodes that go with this feature
    if (keycode == CAPSLIST) {
        if (record->event.pressed) {
          toggle_caps_list();
        }
        return false;
    }

    // Other than the custom keycodes, nothing else in this feature will activate
    // if the behavior is not on, so allow QMK to handle the event as usual.
    if (!caps_list_active) { return true; }
    // No action on keyrelease
    if (!record->event.pressed) { return true; }
    // If Caps Word is on, Caps List stays on as well.
    if (is_caps_word_on()) { return true; }

    // Get the base keycode of a mod or layer tap key
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
    
    if (should_continue_caps_list(keycode)) {
        if (caps_word_reactivation()) {
            caps_word_on();  // Reactivate Caps Word for a new word
            capslist_countdown = 0;
        }
        if (capslist_countdown < countdown_end) { return true; }
    }
    disable_caps_list();
    return true;
}