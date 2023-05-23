#include "alternate_text_mode.h"

// bool mode_set = false;
long long mode_set_flags = 0;

void send_unicode_char(const char * unicode_char) {
//    unicode_input_start();
//    send_unicode_string(unicode_char);
//    unicode_input_finish();
}

bool process_alternate_text_mode(int mode_idx, uint16_t keycode, keyrecord_t* record, uint16_t sel_keycode, const char* const alternate_chars, const char* const alternate_numbers) {
    if (keycode == sel_keycode && record->event.pressed) {
        //        mode_set = !taunt_mode_set;
        mode_set_flags ^= (1 << mode_idx);

        //        if (mode_set) {
        //        if (mode_set_flags & (1 << mode_idx)) {
        //
        //
        //        }

        return true;
    }

    if (mode_set_flags & (1 << mode_idx)) {
        if (record->event.pressed) {
            if (alternate_chars != NULL && keycode >= KC_A && keycode <= KC_Z) {
                const char* c = &alternate_chars[keycode - KC_A];
                send_unicode_char(c);
                return true;
            } else if (alternate_numbers != NULL && keycode >= KC_0 && keycode <= KC_9) {
//                send_unicode_char(alternate_numbers[keycode - KC_0]);
                return true;
            }
        }
    }

    return false;
}
