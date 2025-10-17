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


bool is_caps_lock_on(void) { return host_keyboard_led_state().caps_lock; }


// This function extracts the base keycode of MT and LT,
// even if the tap/hold key is a custom one, with non-basic tap keycode.
uint16_t tap_hold_extractor(uint16_t keycode) {
  switch (keycode) {
    case LT_NBSPC:
      return NNB_SPC;
    default:
      return keycode &= 0xff;   //QK_MOD_TAP_GET_TAP_KEYCODE(keycode)
  }
}

/* bool process_custom_tap_hold(uint16_t keycode, keyrecord_t *record) {

  if (record->tap.count) {    // Handling of special tap-hold keys (on tap).
    switch (keycode) {

        case RCTL_T(FEN_B):
          return process_tap_hold(LWIN(KC_DOWN), record);

        case SFT_T(COPY):
          return process_tap_hold(C(PG_C), record);

        case LT_NBSPC:
          return process_tap_hold(NNB_SPC, record);

        case LT_REPT:
          repeat_key_invoke(&record->event);
          return false;

        case LT_MGC:
          alt_repeat_key_invoke(&record->event);
          return false;
    }
  } else if (!record->event.pressed) {
    if (keycode == LT_MGC && is_select_word()) {
        end_select_word();
    }
  }
  return true; // Process all other keycodes normally
} */

bool process_macros(uint16_t keycode, keyrecord_t *record) {

  if (record->tap.count) {    // Handling of special tap-hold keys (on tap).
    switch (keycode) {

        case RCTL_T(FEN_B):
          return process_custom_tap_hold(LWIN(KC_DOWN), record);

        case SFT_T(COPY):
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

  if (record->event.pressed) {    // Handling of other macros (on press).
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

  switch (get_highest_layer(layer_state|default_layer_state)) {

    case _ODK:
      switch (keycode) {
        case PG_K:
        case PG_B:
        case KC_SPC:  // When space is added by clever keys, for ex. in order to uppercase K after '?' for ex.
          return keycode;

        case PG_POIN:
          return PG_3PTS;

        default:
          clear_recent_keys();
          return KC_NO;
      }
      
    case _SHORTNAV:
      switch (keycode) {
        case KC_SPC:
          return keycode;
        
        default:
          clear_recent_keys();
          return KC_NO;
      }
  }

  if (keycode == PG_E) { return PG_E; }   // because PG_E is not a basic keycode

/*   if (!IS_KEYEVENT(record->event)) {
    switch (keycode) {
      case KC_BSPC:
        break;

      default:
        clear_recent_keys();
        return KC_NO;
    }
  } */

  return KC_TRNS;
}


// One-shot 4 all configuration

uint8_t os4a_layer_from_trigger(uint16_t keycode) {
  switch (keycode) {
    case L_OS4A: return _L_MODS;
    case R_OS4A: return _R_MODS;
    default: return 0;
  }
}

bool should_stay_os4a_layer(uint16_t keycode) {
  // keycodes that stay on os4a layers w/o being shifted
  switch (keycode) {
    case OS_SHFT:
    case OS_CTRL:
    case OS_LALT:
    case OS_WIN:
      return true;

    default:
      return false;
  }
}

bool not_to_be_shifted(uint16_t keycode) {
  // keycodes that exit os4a layers w/o being shifted
  switch (keycode) {
      case KC_CAPS:
      case CAPSWORD:
      case CAPSLIST:
        return true;

      default:
        return false;
  }
}


// Callum mods

bool is_oneshot_cancel_key(uint16_t keycode) {
  switch (keycode) {
    case L_OS4A:
    case R_OS4A:
        return true;
    default:
        return false;
  }
}

bool is_oneshot_ignored_key(uint16_t keycode) {

  const uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
  //if (keycode == OS_ODK && (mods & ~MOD_BIT(KC_ALGR))) { return true; }

  switch (keycode) {
    case OS_ODK:
      // On veut que la touche typo soit ignorée par tous les Callum mods sauf Alt-gr.
      // Ça permet de transmettre les mods à la touche suivante, par ex pour faire Ctrl + K. 
      // Alt-gr doit pouvoir s’appliquer à la touche typo, pour permettre de faire la touche morte "~" avec.
      // OS_ODK ne doit être ignored_key que lorsqu’elle est employée avec Alt-gr
      // sous peine de ne pas pouvoir faire shift + typo + touche de l'autre côté
      if (mods & ~MOD_BIT(KC_ALGR)) { return true; }
      break;
      
    case OS_SHFT:
    case OS_CTRL:
    case OS_LALT:
    case OS_WIN:
    case OS_FA:       // to be combined with Alt
    case TG_FA:
    case NUMWORD:     // to combine numbers with mods
    //case NUM_ODK:   // NUM_ODK sends PG_ODK when pressed. When shifted, PG_ODK sends one-shot shift.
      return true;
  }
  return false;
}


// Repeat and Magic keys

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record, uint8_t* remembered_mods) {
  switch (keycode) {
    case KC_BSPC:
    case LT_REPT:
    case LT_MGC:
      return false;
  }
  return true;
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