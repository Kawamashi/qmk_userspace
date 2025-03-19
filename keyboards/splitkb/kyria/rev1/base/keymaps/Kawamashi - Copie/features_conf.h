#pragma once

#include "quantum.h"
#include "keymap.h"

bool isLetter(uint16_t keycode);
bool is_caps_lock_on(void);
uint16_t tap_hold_extractor(uint16_t keycode);

uint8_t get_os4a_layer(uint16_t keycode);
bool os4a_layer_changer(uint16_t keycode);
bool to_be_shifted(uint16_t keycode, keyrecord_t *record);
//bool mods_for_mouse(uint16_t keycode);