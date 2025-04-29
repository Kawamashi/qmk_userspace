#pragma once

#include "quantum.h"
#include "keymap.h"
//#include <string.h>
//#include "keymap_french_frgo.h"

#ifdef __cplusplus
extern "C" {
#endif

bool clever_key_finder(uint16_t next_keycode, keyrecord_t* record);


#ifdef __cplusplus
}
#endif
