#pragma once
#include "quantum.h"

#ifdef __cplusplus
extern "C" {
#endif

void pre_process_speculative_hold(uint16_t keycode, keyrecord_t* record);

bool process_record_speculative_hold(uint16_t keycode, keyrecord_t* record);

/**
 * Optional callback to define per-key where Speculative Hold applies.
 *
 * @param keycode    Keycode of the mod-tap key.
 * @param record     keyrecord_t for the event.
 * @return Whether to perform speculative holding for this key.
 */
bool get_speculative_hold(uint16_t keycode, keyrecord_t* record);

#ifdef __cplusplus
}
#endif