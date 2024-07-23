#pragma once

/* Tapping this number of times holds the key until tapped once again. */
#define ONESHOT_TAP_TOGGLE 2
/* Time (in ms) before the one shot key is released */
#define ONESHOT_TIMEOUT 1000

//#define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS
//#define QUICK_TAP_TERM 150
//#define RETRO_TAPPING

#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY

#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX

// Lower the firmware size:
#define NO_DEBUG
#define NO_ACTION_ONESHOT