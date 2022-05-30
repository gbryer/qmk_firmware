#pragma once

#include "quantum.h"
#include "print.h"

#ifdef __cplusplus
extern "C" {
#endif

//void setup_game_mode(int layer, uint16_t sel_keycode);
bool process_game_mode(uint16_t keycode, keyrecord_t *record, int game_layer, uint16_t game_keycode);
bool process_game_mode_chat(uint16_t keycode, keyrecord_t *record, int game_layer, uint16_t game_keycode, uint16_t game_chat_keycode,
                            uint16_t normal_chat_keycode);

#ifdef __cplusplus
}
#endif
