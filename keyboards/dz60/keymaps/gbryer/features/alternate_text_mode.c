#include "alternate_text_mode.h"
#include "../events/event_dispatcher.h"

long long mode_set_flags = 0;
int mode = -1;

bool process_reset_alternate_text_mode(uint16_t keycode, keyrecord_t* record, uint16_t sel_keycode) {
    if (keycode == sel_keycode && record->event.pressed) {
        if (mode != -1) {
            mode = -1;
            dispatch_int_event(EVENT_ALTERNATE_TEXT_MODE, mode);
            return true;
        }
    }
    return false;
}

bool process_alternate_text_mode(int mode_idx, uint16_t keycode, keyrecord_t* record, uint16_t sel_keycode, const char* const* alternate_chars, const char* const* alternate_numbers) {

    if (keycode == sel_keycode && record->event.pressed) {

        if (mode == mode_idx) {
            mode = -1;
        } else {
            mode = mode_idx;
        }

        dispatch_int_event(EVENT_ALTERNATE_TEXT_MODE, mode);

        return true;
    }

    if (mode == mode_idx) {
        if (record->event.pressed && get_mods() == 0 ) {
            if (alternate_chars != NULL && keycode >= KC_A && keycode <= KC_Z) {
                send_unicode_string(alternate_chars[keycode - KC_A]);
                wait_ms(10);
                return true;
            } else if (alternate_numbers != NULL && keycode >= KC_0 && keycode <= KC_9) {
                send_unicode_string(alternate_numbers[keycode - KC_0]);
                wait_ms(10);
                return true;
            }
        }
    }

    return false;
}
