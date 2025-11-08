/* Copyright 2025 @Kawamashi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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

#include "clever_keys_utilities.h"

static uint16_t recent[RECENT_SIZE] = {KC_NO};
uint16_t deadline = 0;
static unsigned char bkspc_countdown = RECENT_SIZE + 1;

// Copy of the record argument for the clever key.
static keyrecord_t mod_record;
static bool processingCK = false;

static uint16_t last_keypress_timer = 0;


uint16_t get_recent_keycode(signed char i) {
  return recent[RECENT_SIZE + i];
}

bool was_keycode_replaced(void) {
  return processingCK;
}

void update_bkspc_countdown(unsigned char i) {
  bkspc_countdown = i;
}

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
uint16_t get_ongoing_keycode(uint16_t keycode, keyrecord_t* record) {

  uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();

  if (mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_ALGR))) {
    clear_recent_keys();  // Avoid interfering with ctrl, alt and gui.
    return KC_NO;
  }

  switch (keycode) {
    // Sticky keys don't type anything on their own.
    case QK_ONE_SHOT_MOD ... QK_ONE_SHOT_MOD_MAX:
    // Ignore MO, TO, TG, TT, and OSL layer switch keys.
    case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
    case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:
/*     case QK_MOMENTARY ... QK_MOMENTARY_MAX:
    case QK_TO ... QK_TO_MAX:
    case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
    case QK_TRI_LAYER_LOWER ... QK_TRI_LAYER_UPPER: */
        return KC_NO;
  }

  // Extract keycode from regular tap-hold keys.
  if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
      if (record->tap.count == 0) { return KC_NO; }
      // Get tapping keycode.
      keycode = tap_hold_extractor(keycode);
  }

  // Handles custom keycodes.
  uint16_t custom_keycode = get_ongoing_keycode_user(keycode, record);
  if (custom_keycode != KC_TRNS) { return custom_keycode; }


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


  uint8_t basic_keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);

  switch (basic_keycode) {
    case KC_A ... KC_SLASH:  // These keys type letters, digits, symbols.

      if (mods & MOD_BIT(KC_ALGR)) { return ALGR(keycode); }

      // Handle keys carrying a modifier, for ex on symbols layer
      if (basic_keycode != keycode) { return keycode; }

      if (is_letter(basic_keycode)) { return keycode; }
      
      // Handle shifted symbols (ex shift + '-' = '!')
      // Convert 8-bit mods to the 5-bit format used in keycodes. This is lossy: if
      // left and right handed mods were mixed, they all become right handed.
      mods = ((mods & 0xf0) ? /* set right hand bit */ 0x10 : 0)
            // Combine right and left hand mods.
            | (((mods >> 4) | mods) & 0xf);
      // Combine basic keycode with mods.
      keycode = (mods << 8) | basic_keycode;
      return keycode;
  }

  // Avoid acting otherwise, particularly on navigation keys.
  clear_recent_keys();
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
  process_key(keycode, record);  // tap_code doesn't work with caps word.
  bkspc_countdown = 1;
}

void replace_ongoing_key(uint16_t clever_keycode, uint16_t* ongoing_keycode, keyrecord_t* record) {
  record->keycode = clever_keycode;
  *ongoing_keycode = clever_keycode;
  set_last_keycode(clever_keycode);
  processingCK = true;
}

void process_word(uint16_t keycodes[], uint8_t num_keycodes, keyrecord_t* record) {
  for (int i = 0; i < num_keycodes; ++i) {
    process_key(keycodes[i], record);  // tap_code doesn't work with caps word.
  }
}

void finish_word(uint16_t keycodes[], uint8_t num_keycodes, uint16_t* ongoing_keycode, keyrecord_t* record) {
  process_word(keycodes, num_keycodes - 1, record);
  bkspc_countdown = num_keycodes - 1;
  replace_ongoing_key(keycodes[num_keycodes - 1], ongoing_keycode, record);
}

void finish_magic(uint16_t keycodes[], uint8_t num_keycodes, uint16_t* ongoing_keycode, keyrecord_t* record) {
  process_word(keycodes, num_keycodes - 1, record);
  replace_ongoing_key(keycodes[num_keycodes - 1], ongoing_keycode, record);
}


void process_clever_keys(uint16_t keycode, keyrecord_t* record) {

  if (record->event.pressed) {
    uint16_t ongoing_keycode = get_ongoing_keycode(keycode, record);

    if (ongoing_keycode != KC_NO) {
      get_clever_keycode(&ongoing_keycode, record);
      store_keycode(ongoing_keycode, record);

      // Global quick tap for combos.
      // IS_KEYEVENT prevents combos from updating last_keypress_timer, to allow combos to be chained.
      if (IS_KEYEVENT(record->event)) { last_keypress_timer = timer_read(); }
    }

  } else if (processingCK) {
    processingCK = false;
    record->keycode = recent[RECENT_SIZE - 1];
  }
}

bool enough_time_before_combo(void) {
  return timer_elapsed(last_keypress_timer) > TAP_INTERVAL;
}

/* void end_CK(keyrecord_t* record) {
  if (processingCK) {
    processingCK = false;
    record->event.pressed = false;
    process_record(record); 
  }
} */