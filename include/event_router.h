// event_router.h
#ifndef EVENT_ROUTER_H
#define EVENT_ROUTER_H


#include "event_bus.h"
#include "input_codes.h"


// Pump all pending events and deliver to the current context
// (or perform global actions like snap-to-live)
void route_events();


#endif // EVENT_ROUTER_H

