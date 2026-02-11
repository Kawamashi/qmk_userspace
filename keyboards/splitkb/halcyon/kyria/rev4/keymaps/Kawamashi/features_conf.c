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

#include "features_conf.h"

static bool is_apos_dr = false;

bool replace_apos(void) {
  return is_apos_dr;
}


uint16_t tap_hold_extractor(uint16_t keycode) {

  switch (keycode) {
    case SFT_T(COPY):
      return G(PG_C);
/*     case LT_REPT:
      return get_last_keycode(); */

    default:
      return keycode &= 0xff;
  }
}


bool process_macros_I(uint16_t keycode, keyrecord_t *record) {

  if (record->tap.count) {
    // Special tap-hold keys (on tap).
    switch (keycode) {
      case LT_REPT:
        repeat_key_invoke(&record->event);
        return false;

      case LT_MGC:
        alt_repeat_key_invoke(&record->event);
        return false;
    }
  }
  return true; // Process all other keycodes normally
}


bool process_macros_II(uint16_t keycode, keyrecord_t *record) {

  if (record->tap.count) {
    // Special tap-hold keys (on tap).
    switch (keycode) {
/*       case SFT_T(FEN_G):
      case RCTL_T(FEN_B):
      case SFT_T(COPY):
      case LT_NBSPC:
        return process_custom_tap_hold(tap_hold_extractor(keycode), record); */

      case SFT_T(COPY):
        return process_custom_tap_hold(C(PG_C), record);

      case OS_1DK:
        // Custom behaviour when alt-gr
        const uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
        if (mods & MOD_BIT(KC_ALGR)) {
            tap_code16(ALGR(PG_1DK));
            return false;
        }
        return true;
    }
  }

  if (record->event.pressed) {
    // Other macros (on press).
    switch (keycode) {
      case TG_APOS:
        is_apos_dr = !is_apos_dr;
        return false;

      case PG_DEG:
        tap_code(PG_1DK);
        tap_code(KC_0);
        return false;
    }
  }
  return true; // Process all other keycodes normally
}


// Clever keys configuration

uint16_t get_ongoing_keycode_user(uint16_t keycode, keyrecord_t* record) {
  // Handles custom keycodes to be processed for Clever Keys

  if (is_send_string_macro(keycode)) { return keycode; }

  if (keycode == KC_BSPC) {
    // Setting the key to be repeated to match the key buffer.
    if (is_followed_by_apos(get_recent_keycode(-1), get_recent_keycode(-2))) {
        set_last_keycode(PG_APOS);
    } else {
        set_last_keycode(get_recent_keycode(-1));
    }
    return KC_NO;
  }

  switch (get_highest_layer(layer_state)) {

    case _1DK:
      switch (keycode) {
        case PG_K:
        case PG_B:
        case PG_H:
        case PG_Z:
        case PG_ECIR:
        //case KC_SPC:  // When space is added by clever keys, for ex. in order to uppercase K after '?' for ex.
          return keycode;

        case PG_POIN:
          return PG_3PTS;

        default:
          if (is_letter(keycode)) { return LETTER_1DK; }
          return SYMBOL_1DK;
      }
    
    // There are no symbols on _SHORTNAV or _FUNCTIONS
    case _SHORTNAV:
    case _FUNCTIONS:
      clear_recent_keys();
      return KC_NO;
  }

  switch (keycode) {
    case PG_E:    // because PG_E is not a basic keycode
      return keycode;
  }

  // Handles combos
  if (!IS_KEYEVENT(record->event)) {
    switch (keycode) {
      case KC_BSPC:
        return KC_TRNS;

      default:
        clear_recent_keys();
        return KC_NO;
    }
  }
  return KC_TRNS;
}


// Repeat and Magic keys

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record, uint8_t* remembered_mods) {
  switch (keycode) {
    case KC_BSPC:
    case LT_REPT:
    case LT_MGC:
      return false;
    
    default:
      return true;
  }
}

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {

  if (get_last_keycode() == KC_NO) { return MAGIC; }

  switch (keycode) {
    case C(PG_Z):
      return C(PG_Y);
    case C(PG_Y):
      return C(PG_Z);
  }

  if (mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_ALGR))) {
    return KC_TRNS;
  }

  keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);
  switch (keycode) {
    case KC_LEFT:
    case KC_RIGHT:
    case KC_DOWN:
    case KC_UP:
    case KC_PGUP:
    case KC_PGDN:
    case KC_HOME:
    case KC_END:
        return KC_TRNS;
  }
  return MAGIC;

/*   if (get_recent_keycode(-1) != KC_NO) { return MAGIC; }
  if (get_last_keycode() == KC_NO) { return MAGIC; }
  
  return KC_TRNS;  // Defer to default definitions. */
}