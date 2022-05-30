#include "open_brackets.h"

bool is_in_brackets = false;

static void add_braces(const char* str, const uint8_t mods, const uint8_t oneshot_mods) {
    clear_mods();  // Temporarily disable mods.
    clear_oneshot_mods();
    send_string_P(str);
    tap_code(KC_LEFT);  // Move cursor between braces.
    set_mods(mods);  // Restore mods.
    set_oneshot_mods(oneshot_mods);
    is_in_brackets = true;
}

bool process_open_brackets(uint16_t keycode, keyrecord_t* record) {

    const uint8_t mods = get_mods();
    const uint8_t oneshot_mods = get_oneshot_mods();
    const bool pressed = record->event.pressed;

    if (is_in_brackets && pressed) {
        is_in_brackets = false;
        if (keycode == KC_SPC)
        {
            clear_mods();
            clear_oneshot_mods();
            tap_code(KC_DEL);
            set_mods(mods);
            set_oneshot_mods(oneshot_mods);
            return false;
        }
    }

    switch (keycode) {

        case KC_LBRC:
            if (pressed) {
                if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                    add_braces(PSTR("{}"), mods, oneshot_mods);
                } else {
                    add_braces(PSTR("[]"), mods, oneshot_mods);
                }
            }
            return true;

        case KC_9:
            if (pressed) {
                if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                    add_braces(PSTR("()"), mods, oneshot_mods);
                } else {
                    tap_code(KC_9);
                }
            }
            return true;

        case KC_COMM:
            if (pressed) {
                if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                    add_braces(PSTR("<>"), mods, oneshot_mods);
                } else {
                    tap_code(KC_COMM);
                }
            }
            return true;
    }

    return false;
}
