#pragma once

#include "quantum.h"
#include "keymap.h"

#ifdef __cplusplus
extern "C" {
#endif


bool process_macros(uint16_t keycode, keyrecord_t *record);

#ifdef __cplusplus
}
#endif