#include QMK_KEYBOARD_H
#include "features/select_word.h"
#include "features/open_brackets.h"
#include "features/taunt_mode.h"
#include "features/game_mode.h"
#include "features/alternate_text_mode.h"
#include <stdio.h>
#include "quantum.h"
#include "send_string.h"
#include "print.h"

/// HOW TO FLASH
/// qmk flash -kb dz60 -km gbryer -e AVR_CFLAGS="-Wno-array-bounds"

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
    KC_TAUNT_MODE, // https://github.com/daniel5151/qmk_firmware/blob/discipline/keyboards/coseyfannitutti/discipline/keymaps/prilik/keymap.c#L145
    KC_LIBVIRT_INPUT_GRAB, // https://libvirt.org/formatdomain.html#input-devices
    KC_UPSIDE_DOWN_TEXT_MODE,
    KC_CIRCLE_TEXT_MODE,
    KC_SCRIPT_TEXT_MODE,
    KC_RESET_TEXT_MODE,
    KC_ALT_ENTER
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


void shift_start(tap_dance_state_t *state, void *user_data);
void shift_end (tap_dance_state_t *state, void *user_data);
void shift_reset (tap_dance_state_t *state, void *user_data);

void super_start(tap_dance_state_t *state, void *user_data);
void super_end (tap_dance_state_t *state, void *user_data);
void super_reset (tap_dance_state_t *state, void *user_data);

//bool taunt_mode_set = false;

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
//        [TD_DOT]  = ACTION_TAP_DANCE_FN_ADVANCED(sentence_end, sentence_end_finished, NULL),
        [TD_SHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(shift_start, shift_end, shift_reset),
        [TD_SUPER] = ACTION_TAP_DANCE_FN_ADVANCED(super_start, super_end, super_reset)
};


void keyboard_post_init_user(void) {
//    setup_game_mode(KC_GAMING, _GAMING);
//    debug_enable=true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_MAIN] = LAYOUT(
            KC_GRAVE/*KC_GESC*/, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_DEL,
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC,
            KC_ESC/*KC_F14*/, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
            TD(TD_SHIFT),TD(TD_SHIFT),KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_HOME,
            KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,  KC_FUNCTION(KC_F15),  KC_SPC,           KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT,   KC_END
    ),

    [_GAMING] = LAYOUT(
            KC_GRAVE/*KC_GESC*/, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_DEL,
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC,
            KC_ESC/*KC_F14*/, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
            KC_LSFT, KC_LSFT,KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_HOME,
            KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,  KC_FUNCTION(KC_F15),  KC_SPC,           KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT,   KC_END
    ),

    [_FUNCTION] = LAYOUT(
            KC_GAMING,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_F13, _______,
            KC_LIBVIRT_INPUT_GRAB, _______, _______, _______, _______, KC_TAUNT_MODE, KC_CIRCLE_TEXT_MODE, KC_UPSIDE_DOWN_TEXT_MODE, KC_SCRIPT_TEXT_MODE, KC_RESET_TEXT_MODE, KC_PSCR, _______, _______, QK_BOOT,
            QK_LOCK, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, _______, _______, KC_ALT_ENTER,
            _______, _______, _______, _______, _______, _______, _______, _______, KC_SELECT_WORD, _______, _______, _______, _______, _______,
            _______, _______, _______,                   _______, _______, _______,          KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY
    ),

};

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    }

    if (state->count == 2) return TD_DOUBLE_SINGLE_TAP;
    else return TD_UNKNOWN; // Any number higher than the maximum state value you return above
}

void shift_start(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_LSFT));
            break;
        default:
            break;
    }
};

void shift_end (tap_dance_state_t *state, void *user_data) {

    switch (td_state) {
        case TD_DOUBLE_SINGLE_TAP:
            register_code16(KC_CAPS);
            break;
        default:
            break;
    }
}

void shift_reset (tap_dance_state_t *state, void *user_data) {

    switch (td_state) {
        case TD_DOUBLE_SINGLE_TAP:
            unregister_code16(KC_CAPS);
            break;
        default:
    }

    unregister_mods(MOD_BIT(KC_LSFT));
}



void super_start(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_LGUI));
            break;
        default:
            break;
    }
};

void super_end (tap_dance_state_t *state, void *user_data) {

    switch (td_state) {
        case TD_DOUBLE_SINGLE_TAP:
            register_code16(KC_CAPS);
            break;
        default:
            break;
    }
}

void super_reset (tap_dance_state_t *state, void *user_data) {

    switch (td_state) {
        case TD_DOUBLE_SINGLE_TAP:
            unregister_code16(KC_CAPS);
            break;
        default:
    }

    unregister_mods(MOD_BIT(KC_LGUI));
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(TD_SHIFT):
            return TAPPING_TERM;
        case KC_FUNCTION(KC_F15):
            return 150;
        default:
            return TAPPING_TERM;
    }
}



const char* const upside_down_chars[26] = {
    "ɐ", // a
    "q", // b
    "ɔ", // c
    "p", // d
    "ǝ", // e
    "ɟ", // f
    "ƃ", // g
    "ɥ", // h
    "ᴉ", // i
    "ɾ", // j
    "ʞ", // k
    "l", // l
    "ɯ", // m
    "u", // n
    "o", // o
    "d", // p
    "b", // q
    "ɹ", // r
    "s", // s
    "ʇ", // t
    "n", // u
    "ʌ", // v
    "ʍ", // w
    "x", // x
    "ʎ", // y
    "z"  // z
};

const char* const circled_chars[26] = {
    "ⓐ", // a
    "ⓑ", // b
    "ⓒ", // c
    "ⓓ", // d
    "ⓔ", // e
    "ⓕ", // f
    "ⓖ", // g
    "ⓗ", // h
    "ⓘ", // i
    "ⓙ", // j
    "ⓚ", // k
    "ⓛ", // l
    "ⓜ", // m
    "ⓝ", // n
    "ⓞ", // o
    "ⓟ", // p
    "ⓠ", // q
    "ⓡ", // r
    "ⓢ", // s
    "ⓣ", // t
    "ⓤ", // u
    "ⓥ", // v
    "ⓦ", // w
    "ⓧ", // x
    "ⓨ", // y
    "ⓩ"  // z
};

const char* const circled_number_chars[10] = {
    "⓪", // 0
    "①", // 1
    "②", // 2
    "③", // 3
    "④", // 4
    "⑤", // 5
    "⑥", // 6
    "⑦", // 7
    "⑧", // 8
    "⑨"  // 9
};

const char* const script_chars[26] = {
    "𝓪", // a
    "𝓫", // b
    "𝓬", // c
    "𝓭", // d
    "𝓮", // e
    "𝓯", // f
    "𝓰", // g
    "𝓱", // h
    "𝓲", // i
    "𝓳", // j
    "𝓴", // k
    "𝓵", // l
    "𝓶", // m
    "𝓷", // n
    "𝓸", // o
    "𝓹", // p
    "𝓺", // q
    "𝓻", // r
    "𝓼", // s
    "𝓽", // t
    "𝓾", // u
    "𝓿", // v
    "𝔀", // w
    "𝔁", // x
    "𝔂", // y
    "𝔃"  // z
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

//#ifdef CONSOLE_ENABLE
//    uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
//#endif

    if (!process_select_word(keycode, record, KC_SELECT_WORD)) {
        return false;
    }

    if (process_taunt_mode(keycode, record, KC_TAUNT_MODE)) {
        return false;
    }

    if (process_reset_alternate_text_mode(keycode, record, KC_RESET_TEXT_MODE)) {
        return false;
    }

    process_reset_alternate_text_mode(keycode, record, KC_ENT);
    process_reset_alternate_text_mode(keycode, record, KC_ESC);

    if (process_alternate_text_mode(0, keycode, record, KC_UPSIDE_DOWN_TEXT_MODE, upside_down_chars, NULL)) {
        return false;
    }

    if (process_alternate_text_mode(1, keycode, record, KC_CIRCLE_TEXT_MODE, circled_chars, circled_number_chars)) {
        return false;
    }

    if (process_alternate_text_mode(2, keycode, record, KC_SCRIPT_TEXT_MODE, script_chars, NULL)) {
        return false;
    }

    if (process_game_mode(keycode, record, _GAMING, KC_GAMING))
    {
        if (layer_state_is(_GAMING)) {
            if ((host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK))) {
                tap_code16(KC_CAPS);
            }
//            uprintf("[Game Mode] 1\n");
        } else {
//            uprintf("[Game Mode] 0\n");
        }
        return true;
    }

    if (keycode == KC_LIBVIRT_INPUT_GRAB && record->event.pressed) {

        register_code(KC_LCTL);
        register_code(KC_RCTL);
        unregister_code(KC_LCTL);
        unregister_code(KC_RCTL);

        return false;
    }

    if (keycode == KC_ALT_ENTER && record->event.pressed) {

            register_code(KC_LALT);
            register_code(KC_ENT);
            unregister_code(KC_LALT);
            unregister_code(KC_ENT);

            return false;
    }

//    if (process_open_brackets(keycode, record)) {
//        return false;
//    }

//    if (process_game_mode_chat(keycode, record, _GAMING, KC_GAMING, KC_GAME_CHAT, KC_ENT))
//    {
//        return true;
//    }

    return true;
}



