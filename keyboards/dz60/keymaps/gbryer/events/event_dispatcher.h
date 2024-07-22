#pragma once

#include "quantum.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    EVENT_TAUNT_MODE,
    EVENT_ALTERNATE_TEXT_MODE,
    EVENT_LAYER

} Events;

void dispatch_bool_event(Events ev, bool value);
void dispatch_int_event(Events ev, int value);

#ifdef __cplusplus
}
#endif
