#pragma once

#include "quantum.h"
#include "keymap.h"
//#include "features_conf.h"

//#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

#define RECENT_SIZE 8    // Number of keys in `recent` buffer.

extern uint16_t recent[RECENT_SIZE];
uint16_t deadline;
extern unsigned short int bkspc_countdown;

void clear_recent_keys(void);
void recent_keys_task(void);
uint16_t get_next_keycode(uint16_t keycode, keyrecord_t* record);

void store_keycode(uint16_t keycode, keyrecord_t* record);
void process_key(uint16_t keycode, keyrecord_t* record);
void invoke_key(uint16_t keycode, keyrecord_t* record);
bool replace_next_key(uint16_t keycode, keyrecord_t* record);
void process_word(uint16_t keycodes[], uint8_t num_keycodes, keyrecord_t* record);
bool finish_word(uint16_t keycodes[], uint8_t num_keycodes, keyrecord_t* record);
bool finish_magic(uint16_t keycodes[], uint8_t num_keycodes, keyrecord_t* record);

bool process_clever_keys(uint16_t keycode, keyrecord_t* record);

bool process_accent(uint16_t accent, uint16_t letter, keyrecord_t* record);

#ifdef __cplusplus
}
#endif
