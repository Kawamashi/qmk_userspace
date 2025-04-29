#include "tap_hold_utilities.h"



void tap_converter(uint16_t keycode, keyrecord_t *record) {

  if (IS_OS4A_KEY(keycode)) {
    // Instant OS4A processing
    os4a_tap(keycode);

/*   } else if (keycode == LT_REPT) {    // Je pense que dans les faits, cette ligne ne sert à rien.
    repeat_key_invoke(&record->event); */

  } else {
    if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
      // Tranform the record to send the tap event
      record->keycode = tap_hold_extractor(keycode);
    }
    process_record(record);
  }

  // Send the base keycode key up event
  record->event.pressed = false;
  process_record(record);
}

// Returns true if `pos` on the left hand of the keyboard, false if right.
bool on_left_hand(keypos_t pos) {
#ifdef SPLIT_KEYBOARD
  return pos.row < MATRIX_ROWS / 2;
#else
  return (MATRIX_COLS > MATRIX_ROWS) ? pos.col < MATRIX_COLS / 2
                                     : pos.row < MATRIX_ROWS / 2;
#endif
}

bool same_side_combination(const keyrecord_t* tap_hold_record, const keyrecord_t* other_record) {
  return on_left_hand(tap_hold_record->event.key) == on_left_hand(other_record->event.key);
}

// By default, use the BILATERAL_COMBINATIONS rule to consider the tap-hold key
// "held" only when it and the other key are on opposite hands.
__attribute__((weak)) bool forbidden_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                                           uint16_t other_keycode, keyrecord_t* other_record) {
  return same_side_combination(tap_hold_record, other_record);
}


static bool process_tap_hold(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {    // On press
      tap_code16(keycode);
      return false;
  }
  return true;
}

bool process_custom_tap_hold(uint16_t keycode, keyrecord_t *record) {

  if (record->tap.count) {    // Handling of special tap-hold keys (on tap).
    switch (keycode) {

        case ALGR_T(FR_LACL):
            return process_tap_hold(FR_LACL, record);

        case RCTL_T(FEN_B):
            return process_tap_hold(LWIN(KC_DOWN), record);

        case SFT_T(COPY):
            return process_tap_hold(C(FR_C), record);

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