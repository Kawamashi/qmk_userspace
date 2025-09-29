#pragma once

#include "quantum.h"
#include "keymap.h"

extern unsigned short int capslist_countdown;
extern unsigned short int countdown_end;

bool is_caps_list_on(void);

void enable_caps_list(void);
void disable_caps_list(void);
void toggle_caps_list(void);

bool process_caps_list(uint16_t keycode, keyrecord_t *record);

bool should_continue_caps_list(uint16_t keycode);
bool caps_word_reactivation(void);