#include "features_conf.h"

bool is_caps_lock_on(void) { return host_keyboard_led_state().caps_lock; }

bool isLetter(uint16_t keycode) {
  switch (keycode) {
    case KC_A ... KC_L:
    case FR_M:
    case KC_N ... KC_Z:
    case FR_AGRV:
    case FR_EACU:
    case FR_EGRV:
    case FR_CCED:
    case ALGR(FR_U):
      return true;

    default:
      return false;
  }
}

uint16_t tap_hold_extractor(uint16_t keycode) {
  switch (keycode) {
/*     case LT_REPT:
      return REPEAT;
    case LT_MGC:
      return MAGIC; */
    default:
      return keycode &= 0xff;
  }
}


// Caps Word

bool caps_word_press_user(uint16_t keycode) {

  // Keycodes that continue Caps Word, with shift applied.
  if (isLetter(keycode)) {
    add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
    return true;
  } 

  switch (keycode) {
    // Keycodes that continue Caps Word, without shifting.
    case FR_GRV:
    case FR_MOIN:
    case FR_UNDS:
    case FR_SLSH:
    case KC_KP_1 ... KC_KP_0:
    case KC_LEFT:
    case KC_RIGHT:
    case KC_BSPC:
    case KC_DEL:
    case FR_APOS:
    case FR_ACIR:
      return true;

    default:
      return false;  // Deactivate Caps Word.
    }
}


// Custom AltGr keys

const custom_altgr_key_t custom_altgr_keys[] = {
  {FR_C, FR_COPY},
  {FR_Y, FR_TM},
  //{FR_I, FR_LDAQ},
  //{FR_T, FR_RDAQ},
  {FR_J, FR_CURR},
  {FR_H, FR_HASH},
  {FR_Q, FR_SECT},
  {KC_KP_8, FR_INFN},
  {FR_F, FR_DEG}
};

uint8_t NUM_CUSTOM_ALTGR_KEYS =
    sizeof(custom_altgr_keys) / sizeof(custom_altgr_key_t);


// One-shot 4 all configuration

uint8_t get_os4a_layer(uint16_t keycode) {
  switch (keycode) {
    case L_OS4A: return _L_MODS;
    case R_OS4A: return _R_MODS;
    default: return 0;
  }
}

bool os4a_layer_changer(uint16_t keycode) {
  switch (keycode) {
    case OS_FA:
    case NUMWORD:
    case TT_FA:
      return true;
    default:
      return false;
  }
}

bool to_be_shifted(uint16_t keycode, keyrecord_t *record) {
  // Combos and encoder events.
  if (!IS_KEYEVENT(record->event)) { return true; }
  
  switch (keycode) {
    case KC_CAPS:
    case CAPSWORD:
      return false;
    default:
      return (os4a_layer == _R_MODS) == on_left_hand(record->event.key);
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
  // Alt-gr et shift s'appliquent à la touche typo, pour permettre de faire les majuscules plus facilement ainsi qu'un tréma avec.
  // Autrement, la touche typo est ignorée par les Callum mods.
  // Ça permet de transmettre les mods à la touche suivante, par ex pour faire Ctrl + K. 
  uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
  if (keycode == OS_ACC && (mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_ALGR)))) { return true;}

  switch (keycode) {
    case L_OS4A:
    case R_OS4A:
    case OS_SHFT:
    case OS_CTRL:
    case OS_RALT:
    case OS_LALT:
    case OS_WIN:
    case OS_FA:
    case NUMWORD:
    case TT_FA:
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
  }
  return true;
}

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {

  switch (keycode) {
    case C(FR_Z):
      return C(FR_Y);
    case C(FR_Y):
      return C(FR_Z);
  }

  keycode = tap_hold_extractor(keycode);
  if (isLetter(keycode)) { return MAGIC; }

  return KC_TRNS;  // Defer to default definitions.
}