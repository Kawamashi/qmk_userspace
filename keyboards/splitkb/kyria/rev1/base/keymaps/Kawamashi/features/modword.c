#include "modword.h"

modword_state_t modword_state = idle;

static bool caps_word_active = false;

static signed char capslist_countdown = 1;
static unsigned char countdown_end = 6;

static uint16_t idle_timer = 0;


void modword_task(void) {
  if (modword_state != idle) {
    if (timer_expired(timer_read(), idle_timer)) { disable_modword(modword_state); }
  }
}

uint8_t get_modword(void) {
  return (modword_state);
}

void caps_word_on(void) {
  clear_oneshot();
/*   clear_mods();
  clear_oneshot_mods(); */

  caps_word_active = true;
}

void caps_word_off(void) {
  unregister_weak_mods(MOD_BIT(KC_LSFT));  // Make sure weak shift is off.
  caps_word_active = false;
}


void enable_modword(modword_state_t modword, uint16_t keycode) {
  if (modword_state != idle) { disable_modword(modword_state); }   // Only one modword can be active at a time
  switch (keycode) {

    case CAPSLIST:
        capslist_countdown = 1;
        countdown_end = 6;
    case CAPSWORD:
        caps_word_on();
        break;

    case CAPSLOCK:
        tap_code(KC_CAPS);
        break;

    case SEL_WORD:
        tap_code16(C(KC_RIGHT));
        tap_code16(C(KC_LEFT));
        tap_code16(RCS(KC_RIGHT));
        break;

    case SEL_LINE:
        tap_code(KC_HOME);
        tap_code16(S(KC_END));
        break;

    default:
        break;
  }
  modword_state = modword;
  idle_timer = timer_read() + CAPS_WORD_IDLE_TIMEOUT;
}
  
void disable_modword(modword_state_t modword) {
  switch (modword) {
    case capslist:
    case capsword:
        caps_word_off();
        break;

    case capslock:
        tap_code(KC_CAPS);
        break;

    case selectword:
        clear_weak_mods();
        break;

    default:
        break;
  }
  modword_state = idle;
}

bool toggle_modword(modword_state_t modword_target, uint16_t keycode, keyrecord_t* record) {
  if (record->event.pressed) {
    if (modword_state != modword_target) {
        // Activate layerword layer
        enable_modword(modword_target, keycode);
    } else {
        // Press again an layerword key to exit the layerword layer
        disable_modword(modword_target);
    }
    return false;
  }
  return true;
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


bool process_modword(uint16_t keycode, keyrecord_t* record) {

  switch (keycode) {
    case CAPSWORD:
        return toggle_modword(capsword, keycode, record);

    case CAPSLIST:
        return toggle_modword(capslist, keycode, record);

    case CAPSLOCK:
        return toggle_modword(capslock, keycode, record);

    case SEL_WORD:
        return toggle_modword(selectword, keycode, record);
    
    case SEL_LINE:
        return toggle_modword(selectword, keycode, record);
  }


  if (modword_state == idle) { return true; }

  if (modword_state == capslock) { 
    idle_timer = record->event.time + CAPS_WORD_IDLE_TIMEOUT;
    return true;
  }

  // Caps word or caps list is active //

  if (caps_word_active || modword_state == selectword) { clear_weak_mods(); }

  if (modword_state == selectword) {
    if (IS_LAYER_OFF(_SHORTNAV)) { disable_modword(selectword); }
  }
  

  // No action on keyrelease
  if (!record->event.pressed) { return true; }

  const uint8_t mods = get_mods() | get_oneshot_mods();
  if (mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_ALGR))) {
    // Avoid interfering with ctrl, alt and gui.
    disable_modword(modword_state);
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

  if (caps_word_active) { update_caps_word(keycode, record); }
  if (modword_state == capslist && !caps_word_active) { reactivate_caps_word(keycode, record); }     // Do not merge into a single 'if' block !
  if (modword_state == selectword) { word_selection_press_user(keycode); }

  return true;
}

void update_caps_word(uint16_t keycode, keyrecord_t* record) {

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
  if (modword_state == capsword) { modword_state = idle; }
}

void reactivate_caps_word(uint16_t keycode, keyrecord_t* record) {
    
  if (should_continue_caps_list(keycode, record)) {

      if (list_separator()) {
        caps_word_on();  // Reactivate Caps Word for a new word
        capslist_countdown = 1;
        return;
      }
      if (capslist_countdown < countdown_end) { return; }
  }
  disable_modword(capslist);
}