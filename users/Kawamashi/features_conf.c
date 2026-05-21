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

bool on_left_hand(keypos_t pos) {
#ifdef SPLIT_KEYBOARD
  return pos.row < MATRIX_ROWS / 2;
#else
  return (MATRIX_COLS > MATRIX_ROWS) ? pos.col < MATRIX_COLS / 2
                                     : pos.row < MATRIX_ROWS / 2;
#endif
}

static bool is_numpad = false;

void set_numpad(bool target) {
  is_numpad = target;
}

bool replace_numpad(void) {
  return is_numpad;
}


uint16_t tap_hold_extractor(uint16_t keycode) {

  switch (keycode) {
    case M(C(PG_V)) :
      return C(PG_V);
    case I(C(PG_C)):
      return C(PG_C);

    default:
      return keycode &= 0xff;
  }
}

bool process_custom_tap_hold(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {    // On press
      tap_code16(keycode);
      return false;
  }
  return true;
}


bool process_macros_I(uint16_t keycode, keyrecord_t *record) {

  if (record->tap.count) {
    // Special tap-hold keys (on tap).
    switch (keycode) {
      case LT_REPT:
        const uint8_t mods = get_mods() | get_oneshot_mods();
        if (mods & MOD_MASK_SHIFT) { return toggle_modword(capsword, CAPSWORD, record); }
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
      case LT_NUMW:
        return process_custom_tap_hold(tap_hold_extractor(keycode), record); */

      case M(C(PG_V)) :
        return process_custom_tap_hold(C(PG_V), record);
      case I(C(PG_C)):
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

  // Custom oneshots don't type anything on their own.
  //if (is_custom_oneshot(keycode)) { return KC_NO; }

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
        case PG_U:
          return U_CIRC;

        default:
          if (is_letter(keycode)) { return LETTER_1DK; }
          return SYMBOL_1DK;
      }
    
    // There are no symbols on _SHORTNAV or _FUNCAPPS
    case _SHORTNAV:
    case _FUNCAPPS:
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

  // Custom oneshots don't type anything on their own. They mustn't be remembered.
  if (is_custom_oneshot(keycode)) { return false; }

  if (is_letter(tap_hold_extractor(keycode))) {
    // Forget Shift on letter keys when Shift or AltGr are the only mods.
    if ((*remembered_mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
      *remembered_mods &= ~MOD_MASK_SHIFT;
      return true;
    }
  }

  switch (keycode) {
    case KC_BSPC:
    case OS_SHFT:
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
}


// One-shot mods

const oneshot_t oneshot[] = {
  {OS_SHFT, OS_SHFT, MOD_LSFT, _BASE},
  {OS_WINM, LT_MGC, KC_NO, _FUNCAPPS},
  {OS_WNUM, LT_REPT, MOD_LGUI, _NUMROW},
  {OS_1DK, OS_1DK, KC_NO, _1DK}
};

bool is_oneshot_cancel_key(uint16_t keycode) {
  switch (keycode) {

    default:
      return false;
  }
}

bool should_oneshot_stay_pressed(uint16_t keycode) {

  switch (keycode) {
    case OS_1DK:
      // On veut que les one-shot mods soient transmis aux touches de la couche 1DK, par ex pour faire Ctrl + K.
      // Il faut donc que cette fonction appliquée à OS_1DK renvoie true pour la plupart des mods.
      // Par contre, pour faire la touche morte "~", il faut taper shift + alt-gr + OS_1DK.
      // Alt-gr doit être relâché après appui sur OS_1DK.
      // Cette fonction appliquée à OS_1DK ne doit donc renvoyer false que quand Alt-gr est utilisé.
      const uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
      if (mods & MOD_BIT(KC_ALGR)) { return false; }
      return true;

    case FUNWORD:
    case LT_NUMW:   // to combine numbers with mods
    case LT_MGC:    // custom one-shot shift must shift the character produced by the magic key
      return true;

    default:
      return false;
  }
}