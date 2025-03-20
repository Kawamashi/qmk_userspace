#include "os4a.h"


oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_altgr_state = os_up_unqueued;
oneshot_state os_win_state = os_up_unqueued;

uint8_t os4a_layer = 0;
bool exit_os4a_layer = false;
bool pending_OSL = false;

void os4a_layer_on(uint8_t layer) {
  layer_on(layer);
  os4a_layer = layer;
}

void os4a_layer_off(uint8_t layer) {
  layer_off(layer);
  os4a_layer = 0;
  exit_os4a_layer = false;
}

void os4a_tap(uint16_t keycode) {
  if (os4a_layer == 0) {
      // Activate OS4A layer
      os4a_layer_on(get_os4a_layer(keycode));
  } else {
      // Press again an OS4A key to exit the OS4A layer and clear the OS mods.
      os4a_layer_off(os4a_layer);
  }
}

bool process_os4a_keys(uint16_t keycode, keyrecord_t *record) {
    // tap action
    if (record->event.pressed && record->tap.count) {
      os4a_tap(keycode);
      return false;
    }
    // normal processing if held
    return true;
}

/* void update_osl(uint16_t keycode) {
  //if (os4a_layer_changer(keycode)) { pending_OSL = true; }
  pending_OSL = os4a_layer_changer(keycode);
} */

bool process_os4a_layers(uint16_t keycode, keyrecord_t *record) {

    // Should keycode exit the OS4A layer ?
    if (is_oneshot_ignored_key(keycode)) { return false; }

    // Add OS Shift when no other mods are active.
    // Testing exit_os4a_layer is necessary to prevent OS shift to be added when other features create keyrecords
    // to be processed (ex: custom altgr, clever keys).
    uint8_t mods = get_mods() | get_oneshot_mods();
    if (!exit_os4a_layer && !pending_OSL && to_be_shifted(keycode, record) && mods == 0) {
      set_oneshot_mods(MOD_BIT(KC_LSFT));
    }
    return true;
}

void mouse_mods_key_up(uint16_t keycode, keyrecord_t *record) {
    
    // The OS4A layer must be exited only when ctrl or shift are registered,
    // not when the OSM are released without having being held.
    //if (get_mods() & QK_ONE_SHOT_MOD_GET_MODS(keycode)) { 

    // When ctrl or shift are released after being held, exit the OS4A layer.
    if (!record->event.pressed && !record->tap.count) {
      os4a_layer_off(os4a_layer);
    }
}

bool process_mods(uint16_t keycode, keyrecord_t *record) {

  // Handling Callum's OSM on OS4A layers
  update_oneshot(&os_shft_state, KC_LSFT, OS_SHFT, keycode, record);
  update_oneshot(&os_ctrl_state, KC_LCTL, OS_CTRL, keycode, record);
  update_oneshot(&os_alt_state, KC_LALT, OS_LALT, keycode, record);
  update_oneshot(&os_altgr_state, KC_RALT, OS_RALT, keycode, record);
  update_oneshot(&os_win_state, KC_LWIN, OS_WIN, keycode, record);
  
  // Handling OS4A keys
  if (IS_OS4A_KEY(keycode)) { return process_os4a_keys(keycode, record); }

  // Behaviour of the OS4A layers
  if (os4a_layer != 0) { exit_os4a_layer = process_os4a_layers(keycode, record); }

  // Updating OSL status on OS4A layers
  pending_OSL = os4a_layer_changer(keycode);

  // When Ctrl or Shift are released, for mouse use.
  //if (mods_for_mouse(keycode)) { mouse_mods_key_up(keycode, record); }

  return true;
}
