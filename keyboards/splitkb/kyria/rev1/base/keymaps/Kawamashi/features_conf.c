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

  // This function extracts the base keycode of MT and LT,
  // even if the tap/hold key is a custom one, with non-basic tap keycode.
  switch (keycode) {
    case LT_NBSPC:
      return NNB_SPC;
    default:
      return keycode &= 0xff;
  }
}


bool process_macros(uint16_t keycode, keyrecord_t *record) {

  if (record->tap.count) {
    // Special tap-hold keys (on tap).
    switch (keycode) {
      case SFT_T(FEN_G):
        return process_custom_tap_hold(LWIN(KC_LEFT), record);

      case RCTL_T(FEN_B):
        return process_custom_tap_hold(LWIN(KC_DOWN), record);

      case SFT_T(COPY):
        enable_layerword(_SHORTNAV);
        return process_custom_tap_hold(C(PG_C), record);

      case LT_NBSPC:
        return process_custom_tap_hold(NNB_SPC, record);

      case LT_REPT:
        repeat_key_invoke(&record->event);
        return false;

      case LT_MGC:
        alt_repeat_key_invoke(&record->event);
        return false;
    }
  }

  if (record->event.pressed) {
    // Other macros (on press).
    switch (keycode) {
      case TG_APOS:
        is_apos_dr = !is_apos_dr;
        return false;

      case PG_DEG:
        tap_code(PG_ODK);
        tap_code(KC_9);
        return false;
    }
  }
  return true; // Process all other keycodes normally
}


// Clever keys configuration

uint16_t get_ongoing_keycode_user(uint16_t keycode, keyrecord_t* record) {
  // Handles custom keycodes to be processed for Clever Keys

  if (is_send_string_macro(keycode)) { return keycode; }
    switch (get_highest_layer(layer_state)) {

      case _ODK:
        switch (keycode) {
          case PG_K:
          case PG_B:
          case PG_H:
          //case KC_SPC:  // When space is added by clever keys, for ex. in order to uppercase K after '?' for ex.
            return keycode;

          case PG_POIN:
            return PG_3PTS;
          case PG_E:
            return E_GRV;
          case PG_O:
            return E_CIRC;

          default:
            if (is_letter(keycode)) { return L_ODK; }
            return ODK;
        }
      
      // There are no symbols on _SHORTNAV or _FUNCAPPS
      case _SHORTNAV:
      case _FUNCAPPS:
        clear_recent_keys();
        return KC_NO;
    }

  if (keycode == PG_E) { return PG_E; }   // because PG_E is not a basic keycode

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


// One-shot mods

const oneshot_key_t oneshot_keys[] = {
  {OS_SHFT, KC_LSFT},
  {OS_CTRL, KC_LCTL},
  {OS_LALT, KC_LALT},
  {OS_WIN, KC_LWIN},
};

bool is_oneshot_cancel_key(uint16_t keycode) {
  switch (keycode) {
    case L_OS4A:
    case R_OS4A:
      return true;
    default:
      return false;
  }
}

bool should_oneshot_stay_pressed(uint16_t keycode) {

  switch (keycode) {
    case OS_ODK:
      // On veut que les one-shot mods soient transmis aux touches de la couche ODK, par ex pour faire Ctrl + K.
      // Il faut donc que cette fonction appliquée à OS_ODK renvoie true pour la plupart des mods.
      // Par contre, pour faire la touche morte "~", il faut taper shift + alt-gr + OS_ODK.
      // Alt-gr doit être relâché après appui sur OS_ODK.
      // Cette fonction appliquée à OS_ODK ne doit donc renvoyer false que quand Alt-gr est utilisé.
      const uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
      if (mods & MOD_BIT(KC_ALGR)) { return false; }
      return true;

    case OS_FA:       // to be combined with Alt
    case FUNWORD:
    case NUMWORD:     // to combine numbers with mods
    //case NUM_ODK:   // NUM_ODK sends PG_ODK when pressed. When shifted, PG_ODK sends one-shot shift.
      return true;

    default:
      return false;
  }
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

  switch (keycode) {
    case C(PG_Z):
      return C(PG_Y);
    case C(PG_Y):
      return C(PG_Z);
  }

  if (get_recent_keycode(-1) != KC_NO) { return MAGIC; }
  if (get_last_keycode() == KC_NO) { return MAGIC; }
  
  return KC_TRNS;  // Defer to default definitions.
}