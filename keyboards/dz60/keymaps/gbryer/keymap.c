#include QMK_KEYBOARD_H
#include "features/select_word.h"
#include "features/open_brackets.h"
#include "features/taunt_mode.h"
#include "features/game_mode.h"
#include <stdio.h>
#include "quantum.h"
#include "send_string.h"


#define KC_FUNCTION(KEY) LT(_FUNCTION, KEY)

enum layers {
    _DEFAULT = 0,
    _MAIN = 0,
    _GAMING,
    _FUNCTION
};

enum custom_keycodes {
    KC_GAMING = SAFE_RANGE,
    KC_GAME_CHAT,
    KC_SELECT_WORD,
    KC_TAUNT_MODE // https://github.com/daniel5151/qmk_firmware/blob/discipline/keyboards/coseyfannitutti/discipline/keymaps/prilik/keymap.c#L145
};

    enum {
        TD_SHIFT = 0,
        TD_SUPER
    };

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_SINGLE_TAP
} td_state_t;

static td_state_t td_state;

//bool game_chat_set;
//void game_chat_enable(void);
//void game_chat_disable(void);


void shift_start(qk_tap_dance_state_t *state, void *user_data);
void shift_end (qk_tap_dance_state_t *state, void *user_data);
void shift_reset (qk_tap_dance_state_t *state, void *user_data);

void super_start(qk_tap_dance_state_t *state, void *user_data);
void super_end (qk_tap_dance_state_t *state, void *user_data);
void super_reset (qk_tap_dance_state_t *state, void *user_data);


//bool taunt_mode_set = false;

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
//        [TD_DOT]  = ACTION_TAP_DANCE_FN_ADVANCED(sentence_end, sentence_end_finished, NULL),
        [TD_SHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(shift_start, shift_end, shift_reset),
        [TD_SUPER] = ACTION_TAP_DANCE_FN_ADVANCED(super_start, super_end, super_reset)
};


void keyboard_pre_init_user(void) {
    setup_game_mode(KC_GAMING, _GAMING);
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_MAIN] = LAYOUT(
            KC_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_DEL,
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC,
            KC_F14, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
            TD(TD_SHIFT),TD(TD_SHIFT),KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_HOME,
            KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,  KC_FUNCTION(KC_SPC),  KC_SPC,           KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT,   KC_END
    ),

    [_GAMING] = LAYOUT(
            KC_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_DEL,
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC,
            KC_F14, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_GAME_CHAT,
            KC_LSFT, KC_LSFT,KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_HOME,
            KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,  KC_FUNCTION(KC_F15),  KC_SPC,           KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT,   KC_END
    ),

    [_FUNCTION] = LAYOUT(
            KC_GAMING,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_F13, _______,
            _______, _______, _______, _______, _______, KC_TAUNT_MODE, _______, _______, _______, _______, KC_PSCR, _______, _______, RESET,
            _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, _______, _______,          KC_LOCK,
            _______, _______, _______, _______, _______, _______, _______, _______, KC_SELECT_WORD, BL_DEC, BL_INC, _______, _______, _______,
            _______, _______, _______,                   _______, _______, _______,          KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY
    ),

};

td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    }

    if (state->count == 2) return TD_DOUBLE_SINGLE_TAP;
    else return TD_UNKNOWN; // Any number higher than the maximum state value you return above
}

void shift_start(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_LSFT));
            break;
        default:
            break;
    }
};

void shift_end (qk_tap_dance_state_t *state, void *user_data) {

    switch (td_state) {
        case TD_DOUBLE_SINGLE_TAP:
            register_code16(KC_CAPS);
            break;
        default:
            break;
    }
}

void shift_reset (qk_tap_dance_state_t *state, void *user_data) {

    switch (td_state) {
        case TD_DOUBLE_SINGLE_TAP:
            unregister_code16(KC_CAPS);
            break;
        default:
    }

    unregister_mods(MOD_BIT(KC_LSFT));
}



void super_start(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_LGUI));
            break;
        default:
            break;
    }
};

void super_end (qk_tap_dance_state_t *state, void *user_data) {

    switch (td_state) {
        case TD_DOUBLE_SINGLE_TAP:
            register_code16(KC_CAPS);
            break;
        default:
            break;
    }
}

void super_reset (qk_tap_dance_state_t *state, void *user_data) {

    switch (td_state) {
        case TD_DOUBLE_SINGLE_TAP:
            unregister_code16(KC_CAPS);
            break;
        default:
    }

    unregister_mods(MOD_BIT(KC_LGUI));
}



bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (!process_select_word(keycode, record, KC_SELECT_WORD)) {
        return false;
    }

    if (process_taunt_mode(keycode, record, KC_TAUNT_MODE)) {
        return false;
    }

    if (process_game_mode(keycode, record))
    {
        return true;
    }

    if (process_open_brackets(keycode, record)) {
        return false;
    }

    if (process_game_mode_chat(keycode, record, KC_GAME_CHAT, KC_ENT))
    {
        return true;
    }

    return true;
}
