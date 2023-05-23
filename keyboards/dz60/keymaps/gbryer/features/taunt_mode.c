#include "taunt_mode.h"
#include "../events/event_dispatcher.h"

bool mode_set = false;

bool is_caps_enabled(void) {
    led_t led_state = host_keyboard_led_state();
    return led_state.caps_lock;
}

bool process_taunt_mode(uint16_t keycode, keyrecord_t *record, uint16_t sel_keycode) {
    if (keycode == sel_keycode && record->event.pressed) {

        mode_set = !mode_set;

        // when it's turned on, toggle caps lock (makes first letter lowercase)
        if (mode_set) {
            if (!is_caps_enabled()) {
                register_code(KC_CAPS);
                unregister_code(KC_CAPS);
            }
        } else {
            if (is_caps_enabled()) {
                register_code(KC_CAPS);
                unregister_code(KC_CAPS);
            }
        }

        dispatch_bool_event(EVENT_TAUNT_MODE, mode_set);

        return true;
    }

    if (mode_set) {
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

    return false;
}
