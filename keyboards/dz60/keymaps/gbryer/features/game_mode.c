#include "game_mode.h"

bool is_game_chat_set = false;
int game_layer;
uint16_t game_keycode;

void setup_game_mode(int layer, uint16_t sel_keycode) {
    game_layer = layer;
    game_keycode = sel_keycode;
}

bool process_game_mode(uint16_t keycode, keyrecord_t *record) {

    if (keycode == game_keycode && record->event.pressed) {
        layer_invert(game_layer);
        return true;
    }

    return false;
}

bool process_game_mode_chat(uint16_t keycode, keyrecord_t *record, uint16_t game_chat_keycode,
                            uint16_t normal_chat_keycode) {

    if (keycode == KC_ESC) {

        if (is_game_chat_set && record->event.pressed) {  // Checks if KC_GAME_CHAT was pressed ingame
            tap_code(KC_ESC);
            is_game_chat_set = false;
            layer_invert(game_layer);                     // Switches back to GAMING layer after chatting
        }
        return true; // Let QMK handle the rest

    } else if (keycode == normal_chat_keycode) {

        if (is_game_chat_set && record->event.pressed) {  // Checks if KC_GAME_CHAT was pressed ingame
            tap_code(normal_chat_keycode);
            is_game_chat_set = false;
            layer_invert(game_layer);                     // Switches back to GAMING layer after chatting
        }
        return true; // Let QMK handle the rest

    } else if (keycode == game_chat_keycode) {

        if (record->event.pressed) {
            tap_code(normal_chat_keycode);
            layer_invert(game_layer);
            is_game_chat_set = true;
        }
        return false;

    }


    return false;

}