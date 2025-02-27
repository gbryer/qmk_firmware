#pragma once

#include "quantum.h"
//#include "unicode/unicode.h"

#ifdef __cplusplus
extern "C" {
#endif

bool process_alternate_text_mode(int mode_idx, uint16_t keycode, keyrecord_t* record, uint16_t sel_keycode, const char* const* alternate_chars, const char* const* alternate_numbers);
bool process_reset_alternate_text_mode(uint16_t keycode, keyrecord_t* record, uint16_t sel_keycode);

#ifdef __cplusplus
}
#endif
