#include QMK_KEYBOARD_H
#include "features/select_word.h"
#include <stdio.h>
#include "quantum.h"
#include "send_string.h"

uint16_t last_keycode;

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
        TD_DOT = 0,
        TD_SHIFT
    };


bool game_chat_set;
void game_chat_enable(void);
void game_chat_disable(void);

static void sentence_end(qk_tap_dance_state_t *state, void *user_data);
void sentence_end_finished (qk_tap_dance_state_t *state, void *user_data);


bool taunt_mode_set = false;

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
        [TD_DOT]  = ACTION_TAP_DANCE_FN_ADVANCED(sentence_end, sentence_end_finished, NULL),
        [TD_SHIFT] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS)
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_MAIN] = LAYOUT(
            KC_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_DEL,
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC,
            KC_F14, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
            TD(TD_SHIFT),TD(TD_SHIFT),KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, TD(TD_DOT),  KC_SLSH, OSM(MOD_RSFT), KC_HOME,
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




static void sentence_end(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {

        // Double tapping TD_DOT produces
        // ". <one-shot-shift>" i.e. dot, space and capitalize next letter.
        // This helps to quickly end a sentence and begin another one
        // without having to hit shift.
        case 2:
            /* Check that Shift is inactive */
            if (!(get_mods() & MOD_MASK_SHIFT)) {
                tap_code(KC_SPC);
                /* Internal code of OSM(MOD_LSFT) */
                add_oneshot_mods(MOD_BIT(KC_LSHIFT));

            } else {
                // send ">" (KC_DOT + shift → ">")
                tap_code(KC_DOT);
            }
            break;

            // Since `sentence_end` is called on each tap
            // and not at the end of the tapping term,
            // the third tap needs to cancel the effects
            // of the double tap in order to get the expected
            // three dots ellipsis.
        case 3:
            // remove the added space of the double tap case
            tap_code(KC_BSPC);
            // replace the space with a second dot
            tap_code(KC_DOT);
            // tap the third dot
            tap_code(KC_DOT);
            break;

            // send KC_DOT on every normal tap of TD_DOT
        default:
            tap_code(KC_DOT);
    }
};

void sentence_end_finished (qk_tap_dance_state_t *state, void *user_data) {
    last_keycode = KC_DOT;
}


static void add_braces(const char* str, const uint8_t mods, const uint8_t oneshot_mods) {
    clear_mods();  // Temporarily disable mods.
    clear_oneshot_mods();
//    SEND_STRING(str);
    send_string_P(str);
    tap_code(KC_LEFT);  // Move cursor between braces.
    set_mods(mods);  // Restore mods.
    set_oneshot_mods(oneshot_mods);
}

// KC_GAME_CHAT: A key that taps enter to enable chat in-game, and temporarily toggles typing layer
// When you're done chatting, pressing enter to send will automatically swap back to game layer, no extra buttons needed
void game_chat_enable(void) {
    game_chat_set = true;
}

void game_chat_disable(void) {
    game_chat_set = false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (!process_select_word(keycode, record, KC_SELECT_WORD)) {
        return false;
    }

    // Get current mod and one-shot mod states.
    const uint8_t mods = get_mods();
    const uint8_t oneshot_mods = get_oneshot_mods();
    const bool pressed = record->event.pressed;

    if (taunt_mode_set) {
        if (pressed) {
            if (keycode != KC_SPC) {
                int r = rand() % 4;
                if (r != 1) {
                    register_code(KC_CAPS);
                    unregister_code(KC_CAPS);
                }
            }
        }
    }

    switch (keycode) {

//        case KC_:
//            if (pressed) {
//                set_single_persistent_default_layer(_COLEMAKDH);
//            }
//            return false;

        case KC_GAMING:
            if (pressed) {
                layer_invert(_GAMING);
            }
            return false;


//        case KC_SWITCH:
//            if (pressed) {
//                layer_on(_SWITCH);
//            } else {
//                layer_off(_SWITCH);
//            }
//            return false;

        case KC_ESC:                                       // Cancels chat mode in most games
            if (game_chat_set && pressed) {  // Checks if KC_GAME_CHAT was pressed ingame
                tap_code(KC_ESC);
                game_chat_set = false;
                layer_invert(_GAMING);                     // Switches back to GAMING layer after chatting
            }
            return true; // Let QMK handle the rest

        case KC_ENT:
            if (game_chat_set && pressed) {  // Checks if KC_GAME_CHAT was pressed ingame
                tap_code(KC_ENT);
                game_chat_set = false;
                layer_invert(_GAMING);                     // Switches back to GAMING layer after chatting
            }
            return true; // Let QMK handle the rest

        case KC_GAME_CHAT:
            // Toggle `game_chat_set`
            if (pressed) {
                tap_code(KC_ENT);
                layer_invert(_GAMING);
                game_chat_enable();
            }
            return false;

        case KC_LBRC:
            if (pressed) {
                if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                    add_braces(PSTR("{}"), mods, oneshot_mods);
                } else {
                    add_braces(PSTR("[]"), mods, oneshot_mods);
                }
            }
            return false;

        case KC_9:
            if (pressed) {
                if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                    add_braces(PSTR("()"), mods, oneshot_mods);
                } else {
                    tap_code(KC_9);
                }
            }
            return false;

        case KC_COMM:
            if (pressed) {
                if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                    add_braces(PSTR("<>"), mods, oneshot_mods);
                } else {
                    tap_code(KC_COMM);
                }
            }
            return false;

        case KC_TAUNT_MODE:
            if (pressed) {
                taunt_mode_set = !taunt_mode_set;
                // when it's turned on, toggle caps lock (makes first letter lowercase)
                if (taunt_mode_set) {
                    register_code(KC_CAPS);
                    unregister_code(KC_CAPS);
                }
            }
            return false;

    }
    return true;
}


// https://docs.qmk.fm/#/one_shot_keys
void oneshot_locked_mods_changed_user(uint8_t mods) {
    if (mods & MOD_MASK_SHIFT) {
        backlight_enable();
    }
    if (mods & MOD_MASK_GUI) {
        backlight_enable();
    }
    if (!mods) {
        backlight_disable();
    }
}