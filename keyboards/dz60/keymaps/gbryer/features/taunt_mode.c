#include "taunt_mode.h"


bool taunt_mode_set = false;

bool process_taunt_mode(uint16_t keycode, keyrecord_t *record, uint16_t sel_keycode) {

    if (taunt_mode_set) {
        if (record->event.pressed) {
            if (keycode != KC_SPC) {
                int r = rand() % 4;
                if (r != 1) {
                    register_code(KC_CAPS);
                    unregister_code(KC_CAPS);
                }
            }
        }
    }

    if (keycode == sel_keycode && record->event.pressed) {
        taunt_mode_set = !taunt_mode_set;
        // when it's turned on, toggle caps lock (makes first letter lowercase)
        if (taunt_mode_set) {
            register_code(KC_CAPS);
            unregister_code(KC_CAPS);
        }
        return true;
    }

    return false;
}
