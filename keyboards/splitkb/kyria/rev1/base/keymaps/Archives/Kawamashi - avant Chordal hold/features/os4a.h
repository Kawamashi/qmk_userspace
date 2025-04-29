#pragma once

#include "quantum.h"
#include "keymap.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t os4a_layer;
extern bool exit_os4a_layer;
extern bool pending_OSL;

void os4a_layer_off(uint8_t layer);
void os4a_layer_on(uint8_t layer);

void os4a_tap(uint16_t keycode);

bool process_os4a_keys(uint16_t keycode, keyrecord_t *record);

void update_osl(uint16_t keycode);

bool process_os4a_layers(uint16_t keycode, keyrecord_t *record);

void mouse_mods_key_up(uint16_t keycode, keyrecord_t *record);

bool process_mods(uint16_t keycode, keyrecord_t *record);

#ifdef __cplusplus
}
#endif
