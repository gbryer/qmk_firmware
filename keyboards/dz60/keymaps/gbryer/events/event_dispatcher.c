#include "event_dispatcher.h"


void dispatch_bool_event(Events ev, bool value) {
    dispatch_int_event(ev, value ? 1 : -1);
}
void dispatch_int_event(Events ev, int value) {
    uprintf("[%d]=%d\n", ev, value);
}