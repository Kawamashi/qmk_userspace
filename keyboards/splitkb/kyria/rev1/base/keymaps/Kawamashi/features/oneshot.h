#pragma once

#include QMK_KEYBOARD_H
#include "keymap.h"

// Represents the five states a oneshot key can be in
typedef enum {
    os_idle,
    os_up_queued,
    os_up_queued_used,
    os_down_unused,
    os_down_used,
} oneshot_state;

// Idle timout:
// Custom oneshot mods are configured to deactivate if the keyboard is idle
// for some time. This is useful to prevent unexpected behaviours.
// In config.h, define `ONESHOT_TIMEOUT` with a time in milliseconds.
void oneshot_task(void);

bool process_oneshot(uint16_t keycode, keyrecord_t *record);

// Custom oneshot mod implementation that doesn't rely on timers. If a mod is
// used while it is held it will be unregistered on keyup as normal, otherwise
// it will be queued and only released after the next non-mod keyup.
bool update_oneshot(
    oneshot_state *state,
    uint16_t mod,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
);

// To be implemented by the user. Defines keys to cancel oneshot mods.
bool is_oneshot_cancel_key(uint16_t keycode);

// To be implemented by the user. Defines keys to ignore when determining
// whether a oneshot mod has been used. Setting this to modifiers and layer
// change keys allows stacking multiple oneshot modifiers, and carrying them
// between layers.
bool is_oneshot_ignored_key(uint16_t keycode);
