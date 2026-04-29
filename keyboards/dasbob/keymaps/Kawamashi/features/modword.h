#pragma once

#include "quantum.h"
#include "keymap.h"

// Represents the five states a one-shot key can be in
typedef enum {
    idle,
    capsword,
    capslist,
    capslock,
    selectword
} modword_state_t;


// Idle timout:
// Modword can be configured to deactivate if the keyboard is idle
// for some time. This is useful to prevent unexpected behaviours.
// To be called from `housekeeping_task_user`.
void modword_task(void);

// Returns the active Modword
uint8_t get_modword(void);

// Turns on Caps Word.
void caps_word_on(void);

// Turns off Caps Word.
void caps_word_off(void);

// Turns on Modword
void enable_modword(modword_state_t modword, uint16_t keycode);

// Turns off Modword
void disable_modword(modword_state_t modword);

// Toggles Modword
bool toggle_modword(modword_state_t modword_target, uint16_t keycode, keyrecord_t* record);

// Handler function for Modword
// To be called from `process_record_user`
bool process_modword(uint16_t keycode, keyrecord_t* record);


// Handler function for Caps Word
// Checks whether the key should continue Caps Word by calling caps_word_press_user(),
// and invert Caps Word on shift.
void update_caps_word(uint16_t keycode, keyrecord_t* record);

/* Callback function, called on each key press while Caps Word is active.
 *
 * When the key should be shifted (that is, a letter key), the callback should
 * call `add_weak_mods(MOD_BIT(KC_LSFT))` to shift the key. The callback also
 * determines whether the key should continue Caps Word. Returning true
 * continues the current "word", while returning false is "word breaking" and
 * deactivates Caps Word.
 */
bool caps_word_press_user(uint16_t keycode);


// Handler function for Caps List
// Checks whether the key should continue Caps List.
// If it recognizes a list separator, it reactivates Caps Word.
void update_caps_list(uint16_t keycode, keyrecord_t* record);

// Callback function, called on each key press while Caps List is active.
// The callback determines whether the key should continue Caps List.
// If the key should continue Caps List, the caps list counter must be updated.
bool should_continue_caps_list(uint16_t keycode, keyrecord_t* record);

// When caps list is active and caps word has been deactivated, a counter starts.
// If a character is not list-breaking, it increase the counter.
// Backspace decrease the counter.
// When the counter reach the limit, Caps List is deactivated
// This function updates the counter on each non-list-breaking keypress.
bool update_capslist_counter(signed char i);

// Caps List reactivates Caps Word after list separators (e.g. `, `, `and`, `or`).
// List separators must be defined in this function.
bool list_separator(void);

// Determines whether the last keys form a list separator.
bool word_check(uint16_t keycodes[], uint8_t num_keycodes, unsigned char new_counter_limit);


// Handler function for Word Selection
// Checks whether the key should continue Word Selection.
// Navigation keys should continue Word Selection,
// and weak mods should be added to select words and lines.
void word_selection_press_user(uint16_t keycode);

// Function to select a word, forward or backward.
// Updates the number of words selected.
void select_word(signed char nb_word);

// Function to select a line, downward or upward.
// Updates the number of lines selected.
void select_line(signed char nb_line);
