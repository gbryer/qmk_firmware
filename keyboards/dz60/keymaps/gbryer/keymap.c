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
#include "events/event_dispatcher.h"

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
    KC_ALT_ENTER,
    KC_NULL_W,
    KC_NULL_A,
    KC_NULL_S,
    KC_NULL_D,

};

//tap_dance_action_t tap_dance_actions[] = {
//};


void keyboard_post_init_user(void) {
    // ...
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_MAIN] = LAYOUT(
            KC_GRAVE/*KC_GESC*/, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_DEL,
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC,
            KC_ESC/*KC_F14*/, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
            /*TD(TD_SHIFT)*/SC_LSPO, SC_LSPO, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SC_RSPC, KC_HOME,
            KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,  KC_FUNCTION(KC_F15),  KC_SPC,           KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT,   KC_END
    ),

    [_GAMING] = LAYOUT(
            KC_ESC/*KC_GESC*/, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_DEL,
            KC_TAB,  KC_Q,    KC_NULL_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC,
            KC_GRAVE/*KC_F14*/, KC_NULL_A,    KC_NULL_S,    KC_NULL_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
            KC_LSFT, KC_LSFT,KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_HOME,
            KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,  KC_FUNCTION(KC_F15),  KC_SPC,           KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT,   KC_END
    ),

    [_FUNCTION] = LAYOUT(
            _______,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_F13, _______,
            KC_LIBVIRT_INPUT_GRAB, _______, _______, _______, _______, KC_TAUNT_MODE, KC_CIRCLE_TEXT_MODE, KC_UPSIDE_DOWN_TEXT_MODE, KC_SCRIPT_TEXT_MODE, KC_RESET_TEXT_MODE, KC_PSCR, _______, _______, QK_BOOT,
            KC_CAPS, _______, _______, _______, _______, KC_GAMING, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, _______, _______, KC_ALT_ENTER,
            _______, _______, _______, _______, _______, _______, _______, _______, KC_SELECT_WORD, _______, _______, _______, _______, _______,
            _______, _______, _______,                   _______, _______, _______,          KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY
    ),

};


layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);

    switch (layer) {
        case _GAMING:
            autocorrect_disable();
            break;
        case _MAIN:
            autocorrect_enable();
            break;
        default:
            return state;
    }

    dispatch_int_event(EVENT_LAYER, layer);

    return state;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
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

bool nmforward = false;
bool nmback = false;
bool nmleft = false;
bool nmright = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

//#ifdef CONSOLE_ENABLE
//    uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
//#endif

    // https://github.com/chilliams/qmk_firmware/commit/6f7e229d42a9e388aae92163bb3fde39afc2571f
    // http://wiki.quakeworld.nu/Strafescript
    switch(keycode) {
        case KC_NULL_W: // Handle forward movement
            if (record->event.pressed) {
                register_code(KC_W);
                nmforward = true;
                if (nmback) {
                    unregister_code(KC_S);
                }
            } else {
                unregister_code(KC_W);
                nmforward = false;
                if (nmback) {
                    register_code(KC_S);
                }
            }
            return false;

        case KC_NULL_A: // Handle left movement
            if (record->event.pressed) {
                register_code(KC_A);
                nmleft = true;
                if (nmright) {
                    unregister_code(KC_D);
                }
            } else {
                unregister_code(KC_A);
                nmleft = false;
                if (nmright) {
                    register_code(KC_D);
                }
            }
            return false;

        case KC_NULL_S: // Handle backward movement
            if (record->event.pressed) {
                register_code(KC_S);
                nmback = true;
                if (nmforward) {
                    unregister_code(KC_W);
                }
            } else {
                unregister_code(KC_S);
                nmback = false;
                if (nmforward) {
                    register_code(KC_W);
                }
            }
            return false;

        case KC_NULL_D: // Handle right movement
            if (record->event.pressed) {
                register_code(KC_D);
                nmright = true;
                if (nmleft) {
                    unregister_code(KC_A);
                }
            } else {
                unregister_code(KC_D);
                nmright = false;
                if (nmleft) {
                    register_code(KC_A);
                }
            }
            return false;
    }

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

    // Toggle GAMING layer and turn on caps lock if enabled
    if (keycode == KC_GAMING && record->event.pressed) {
        layer_invert(_GAMING);
        if ((host_keyboard_led_state().caps_lock)) {
            tap_code16(KC_CAPS);
        }
        return false;
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

    return true;
}



