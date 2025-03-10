#include "macros.h"

//bool is_alt_tab_active = false;

/* static bool process_swapper(uint16_t keycode) {
    if (!is_alt_tab_active) {
        is_alt_tab_active = true;
        register_code(KC_LALT);
    }
    tap_code16(keycode);
    return false;
} */

/* void swapper_task(void) {
  if (is_alt_tab_active) {
    if (IS_LAYER_OFF(_SHORTNAV)){
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
} */

bool process_macros(uint16_t keycode, keyrecord_t *record) {
    //const uint8_t mods = get_mods();

    if (record->event.pressed) {    // Handling of other macros (on press).
        switch (keycode) {
/*             case ALT_TAB:
                return process_swapper(KC_TAB);
            case REV_TAB:
                return process_swapper(S(KC_TAB)); */

            case AIDE_MEM:                    
                switch(get_highest_layer(layer_state|default_layer_state)) {
                    case _BASE:
                        tap_code(KC_F13);
                        return false;
                    case _SYMNUM:
                        tap_code(KC_F14);
                        return false;
                    case _SHORTNAV:
                        tap_code(KC_F15);
                        return false;
                    case _FUNCAPPS:
                        tap_code(KC_F16);
                        return false;
                }

            case RAZ:
                //led_t led_usb_state = host_keyboard_led_state();
                if (is_caps_lock_on()) { tap_code(KC_CAPS); }
                if (!host_keyboard_led_state().num_lock) { tap_code(KC_NUM_LOCK); }

                layer_clear();
                clear_oneshot_mods();
                caps_word_off();
                disable_num_word();
                clear_recent_keys();
                break;

            case KC_CAPS:
                caps_word_off();
                return true;

            case CAPSWORD:
                // I can't use CW_TOGG because QMK dosn't reach process_record_user when processing it.
                // Deactivating Caps Lock when Caps Word activates.
                if (is_caps_lock_on()) { tap_code(KC_CAPS); }
                caps_word_toggle();
                return false;
        }
    }
    return true; // Process all other keycodes normally
}
