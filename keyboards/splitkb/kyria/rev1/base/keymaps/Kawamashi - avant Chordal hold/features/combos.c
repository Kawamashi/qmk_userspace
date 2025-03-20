#include QMK_KEYBOARD_H

//#include "combos.h"
#include "keymap.h"

enum combos {
  R_BKSPC, 
  DELETE, 
  BK_WORD,
  DEL_WORD, 
  L_BKSPC, 
  HOME, 
  END, 
  ENTER,
  TAB,
  ESC, 
  HELP, 
  PANIC,
  NUMWRD,
  ALTTAB,
  CTRLALT
};

const uint16_t PROGMEM del_combo_d[] = {FR_S, FR_N, COMBO_END};
const uint16_t PROGMEM bkspc_combo_d[] = {FR_N, FR_R, COMBO_END};
const uint16_t PROGMEM del_word_combo[] = {FR_M, FR_L, COMBO_END};
const uint16_t PROGMEM bk_word_combo[] = {FR_L, FR_APOS, COMBO_END};
const uint16_t PROGMEM enter_combo[] = {FR_P, FR_EACU, COMBO_END};
const uint16_t PROGMEM tab_combo[] = {FR_T, FR_I, COMBO_END};
const uint16_t PROGMEM esc_combo[] = {FR_T, FR_A, COMBO_END};
const uint16_t PROGMEM bkspc_combo_g[] = {FR_A, FR_I, COMBO_END};
const uint16_t PROGMEM home_combo[] = {FR_Z, FR_Y, COMBO_END};
const uint16_t PROGMEM end_combo[] = {FR_EACU, FR_VIRG, COMBO_END};
const uint16_t PROGMEM help_combo[] = {FR_VIRG, FR_APOS, COMBO_END};
const uint16_t PROGMEM panic_combo[] = {FR_EACU, FR_L, COMBO_END};
const uint16_t PROGMEM numword_combo[] = {FR_S, FR_R, COMBO_END};
const uint16_t PROGMEM alttab_combo[] = {LT_D, FR_Y, COMBO_END};
const uint16_t PROGMEM ctrlalt_combo[] = {FR_A, FR_I, FR_T, COMBO_END};

combo_t key_combos[] = {
    [R_BKSPC] = COMBO(bkspc_combo_d, KC_BSPC),
    [DELETE] = COMBO(del_combo_d, KC_DEL),
    [BK_WORD] = COMBO(bk_word_combo, LCTL(KC_BSPC)), 
    [DEL_WORD] = COMBO(del_word_combo, LCTL(KC_DEL)), 
    [L_BKSPC] = COMBO(bkspc_combo_g, KC_BSPC),
    [HOME] = COMBO(home_combo, KC_HOME),
    [END] = COMBO(end_combo, KC_END),
    [ENTER] = COMBO(enter_combo, KC_ENT),
    [TAB] = COMBO(tab_combo, KC_TAB),
    [ESC] = COMBO(esc_combo, KC_ESC),
    [HELP] = COMBO(help_combo, AIDE_MEM), 
    [PANIC] = COMBO(panic_combo, RAZ),
    [NUMWRD] = COMBO(numword_combo, NUMWORD),
    [ALTTAB] = COMBO(alttab_combo, KC_NO),
    [CTRLALT] = COMBO(ctrlalt_combo, RCTL(RALT(KC_DEL)))
    };

/* uint16_t get_combo_term(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case HOME:
            return COMBO_TERM - 10;
        default:
            return COMBO_TERM;
    }
} */

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    // Chorded mods shouldn't be considered as combos.
    if (os4a_layer != 0) {
      return (os4a_layer == _R_MODS) == on_left_hand(record->event.key);
    }
    // Some combos shouldn't be affected by global_quick_tap_timer.
    switch (combo_index) {
        case R_BKSPC:
        case BK_WORD:
        case ENTER:
          return true;

        default:
          //return timer_elapsed(global_quick_tap_timer) > TAP_INTERVAL;
          if (timer_elapsed(global_quick_tap_timer) < TAP_INTERVAL) {
            return false;
          }
    }
    return true;
}


void process_combo_event(uint16_t combo_index, bool pressed) {
  switch (combo_index) {
      case ALTTAB:
          if (pressed) {
              register_mods(MOD_LALT);
              tap_code(KC_TAB);
          } else {
              unregister_mods(MOD_LALT);
          }
          break;
  }
}

bool process_combo_key_repress(uint16_t combo_index, combo_t *combo, uint8_t key_index, uint16_t keycode) {
  switch (combo_index) {
      case ALTTAB:
          switch (keycode) {
              case FR_Y:
                  tap_code16(S(KC_TAB));
                  return true;
              case LT_D:
                  tap_code(KC_TAB);
                  return true;
          }
  }
  return false;
}