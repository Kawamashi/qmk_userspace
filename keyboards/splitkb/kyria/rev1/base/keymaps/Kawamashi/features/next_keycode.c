// Remembering recent keys

#include "next_keycode.h"

uint16_t recent[RECENT_SIZE] = {KC_NO};
uint16_t deadline = 0;
unsigned short int bkspc_countdown = RECENT_SIZE + 1;

// Copy of the record argument for the clever key.
static keyrecord_t mod_record;

void clear_recent_keys(void) {
  memset(recent, 0, sizeof(recent));  // Set all zeros (KC_NO).
  bkspc_countdown = RECENT_SIZE + 1;
}

void recent_keys_task(void) {
  if (recent[RECENT_SIZE - 1] && timer_expired(timer_read(), deadline)) {
    clear_recent_keys();  // Timed out; clear the buffer.
  }
}

// Handles one event. Returns false if the key was appended to `recent`.
uint16_t get_next_keycode(uint16_t keycode, keyrecord_t* record) {
  if (!record->event.pressed) { return KC_NO; }

  uint8_t mods = get_mods() | get_oneshot_mods();

  if (mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_ALGR))) {
    clear_recent_keys();  // Avoid interfering with ctrl, left alt and gui.
    return KC_NO;
  }

  // Sticky keys don't type anything on their own.
  if (IS_QK_ONE_SHOT_MOD(keycode) || IS_QK_ONE_SHOT_LAYER(keycode)) { return KC_NO; }

    // Handle backspace.
  if (keycode == KC_BSPC) {
      bkspc_countdown--;
      if (bkspc_countdown == 0) {
        // Clear the key buffers.
        clear_recent_keys();
      } else {
        // Rewind the key buffers.
        memmove(recent + 1, recent, (RECENT_SIZE - 1) * sizeof(uint16_t));
        recent[0] = KC_NO;
        // Setting the key to be repeated to match the key buffer.
        set_last_keycode(recent[RECENT_SIZE - 1]);
      }
      return KC_NO;
  }

  // Extract keycode from regular tap-hold keys.
  if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
      if (record->tap.count == 0) { return KC_NO; }
      // Get tapping keycode.
      keycode = tap_hold_extractor(keycode);
  }

  // Handles custom keycodes.
  if IS_QK_USER(keycode) { return keycode; }

  uint8_t basic_keycode = keycode;
  // Handle keys carrying a modifier, for ex on layers(! and ?).
  if (IS_QK_MODS(keycode)) { basic_keycode = QK_MODS_GET_BASIC_KEYCODE(keycode); }

  switch (basic_keycode) {
    case KC_A ... KC_SLASH:  // These keys type letters, digits, symbols.

      if (isLetter(basic_keycode) && (mods & ~MOD_BIT(KC_ALGR))) {
          // Shift doesn't matter for letters.
          return basic_keycode;

      } else if (basic_keycode != keycode) {
          // For keys carrying a modifier, for ex on layers.
          return keycode;

      } else {
          // Convert 8-bit mods to the 5-bit format used in keycodes. This is lossy: if
          // left and right handed mods were mixed, they all become right handed.
          mods = ((mods & 0xf0) ? /* set right hand bit */ 0x10 : 0)
                // Combine right and left hand mods.
                | (((mods >> 4) | mods) & 0xf);
          // Combine basic keycode with mods.
          keycode = (mods << 8) | basic_keycode;
          return keycode;
      }

    default:  // Avoid acting otherwise, particularly on navigation keys.
      clear_recent_keys();
      return KC_NO;
  }

  return KC_NO;
}

void store_keycode(uint16_t keycode, keyrecord_t* record) {
  // Slide the buffer left by one element.
  memmove(recent, recent + 1, (RECENT_SIZE - 1) * sizeof(*recent));
  recent[RECENT_SIZE - 1] = keycode;
  bkspc_countdown++;
  deadline = record->event.time + RECENT_KEYS_TIMEOUT;
}

void process_key(uint16_t keycode, keyrecord_t* record) {
  mod_record = *record;
  mod_record.keycode = keycode;
  // Send the next keycode key down event
  process_record(&mod_record);
  // Send the next keycode key up event
  mod_record.event.pressed = false;
  process_record(&mod_record);
}

void invoke_key(uint16_t keycode, keyrecord_t* record) {
  process_key(keycode, record);
  bkspc_countdown = 1;
}

bool replace_next_key(uint16_t keycode, keyrecord_t* record) {
  invoke_key(keycode, record);
  return true;
}

void process_word(uint16_t keycodes[], uint8_t num_keycodes, keyrecord_t* record) {
  for (int i = 0; i < num_keycodes; ++i) {
    invoke_key(keycodes[i], record);
  }
  bkspc_countdown = num_keycodes;
}

bool finish_word(uint16_t keycodes[], uint8_t num_keycodes, keyrecord_t* record) {
  process_word(keycodes, num_keycodes, record);
  return true;
}

bool finish_magic(uint16_t keycodes[], uint8_t num_keycodes, keyrecord_t* record) {
  // Setting the key to be repeated to match the key buffer.
  set_last_keycode(keycodes[num_keycodes - 1]);
  return finish_word(keycodes, num_keycodes, record);
}

bool process_clever_keys(uint16_t keycode, keyrecord_t* record) {

  uint16_t next_keycode = get_next_keycode(keycode, record);  

  if (next_keycode != KC_NO) {

    if (clever_key_finder(next_keycode, record)) { return false; }
    store_keycode(next_keycode, record);
  }
  return true; // If no clever key was found, process keycode normally.
}

bool process_accent(uint16_t accent, uint16_t letter, keyrecord_t* record) {
  const bool is_shifted = (get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
  if (is_shifted) {
      del_weak_mods(MOD_MASK_SHIFT);
      del_oneshot_mods(MOD_MASK_SHIFT);
      unregister_mods(MOD_MASK_SHIFT);
  }
  tap_code16(accent);
  //set_mods(mods);
  if (is_shifted) { set_oneshot_mods(MOD_BIT(KC_LSFT)); }
  invoke_key(letter, record);
  return true;
}