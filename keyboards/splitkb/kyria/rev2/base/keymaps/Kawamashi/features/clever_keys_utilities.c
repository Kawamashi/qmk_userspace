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

uint16_t recent[RECENT_SIZE] = {KC_NO};
uint16_t deadline = 0;
//static unsigned short int bkspc_countdown = RECENT_SIZE + 1;
unsigned short int bkspc_countdown = RECENT_SIZE + 1;

// Copy of the record argument for the clever key.
static keyrecord_t mod_record;
static bool processingCK = false;

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
  if (isSendStringMacro(keycode)) { return keycode; }
  //if (keycode == PG_CCED) { return PG_CCED; }
  if (IS_LAYER_ON(_ODK)) {
    switch (keycode) {
      case PG_K:
      case PG_J:
      case PG_AROB:
      case PG_3PTS:
      case KC_SPC:  // In order to uppercase J after '?' for ex.
        return keycode;
      case PG_VIRG:
        return PG_3PTS;
/*       case PG_T:
        return PG_MOIN; */
      default:
        clear_recent_keys();
        return KC_NO;
    }
  }

  uint8_t basic_keycode = keycode;
  // Handle keys carrying a modifier, for ex on layers(! and ?).
  if (IS_QK_MODS(keycode)) { basic_keycode = QK_MODS_GET_BASIC_KEYCODE(keycode); }

  switch (basic_keycode) {
    case KC_A ... KC_SLASH:  // These keys type letters, digits, symbols.
    case PG_E:

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
  //record->keycode = keycode;
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
    process_key(keycodes[i], record);   // Better solution, if there is enought space in the chip.
    //tap_code(keycodes[i]);
  }
  bkspc_countdown = num_keycodes;
}

void finish_word(uint16_t keycodes[], uint8_t num_keycodes, uint16_t* ongoing_keycode, keyrecord_t* record) {
  process_word(keycodes, num_keycodes - 1, record);
  replace_ongoing_key(keycodes[num_keycodes - 1], ongoing_keycode, record);
}


bool process_clever_keys(uint16_t keycode, keyrecord_t* record) {

  if (record->event.pressed) {
    uint16_t ongoing_keycode = get_ongoing_keycode(keycode, record);

    if (ongoing_keycode != KC_NO) {
      get_clever_keycode(&ongoing_keycode, record);
      store_keycode(ongoing_keycode, record);
    }
    //return true; // If no clever key was found, process keycode normally.
  } 
  return true;
}

void end_CK(keyrecord_t* record) {
  if (processingCK) {
    processingCK = false;
    record->event.pressed = false;
    process_record(record); 
  }
}