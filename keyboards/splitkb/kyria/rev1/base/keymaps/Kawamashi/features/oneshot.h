#pragma once

#include QMK_KEYBOARD_H
#include "keymap.h"

// Custom one-shot key entry. The `keycode` field is the keycode as it appears in
// your keymap. The `modifier` field must use the KC_* prefix.
typedef struct {
  uint16_t keycode;
  uint16_t modifier;
} oneshot_key_t;

// Table of custom one-shot modifiers.
// Each custom OSM must also be declared in custom_keycodes.
extern const oneshot_key_t oneshot_keys[];

// Represents the five states a oneshot key can be in
typedef enum {
    os_idle,
    os_up_queued,
    os_up_queued_used,
    os_down_unused,
    os_down_used,
} oneshot_state_t;

// Idle timout:
// Custom oneshot mods are configured to deactivate if the keyboard is idle
// for some time. This is useful to prevent unexpected behaviours.
// In config.h, define `ONESHOT_TIMEOUT` with a time in milliseconds.
void oneshot_task(void);

// Custom oneshot mod implementation that doesn't rely on timers. If a mod is
// used while it is held it will be unregistered on keyup as normal, otherwise
// it will be queued and only released after the next non-mod keyup.
bool process_oneshot(uint16_t keycode, keyrecord_t *record);

// To be implemented by the user. Defines keys to cancel oneshot mods.
bool is_oneshot_cancel_key(uint16_t keycode);

// To be implemented by the user. 
// Defines keys not to release the modifier when determining
// whether a oneshot mod has been used
// Setting this to layer change keys allows carrying oneshot modifiers between layers.
bool should_oneshot_stay_pressed(uint16_t keycode);
