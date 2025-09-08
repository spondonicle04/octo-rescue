// events.h
#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

enum EventKind : uint8_t { EVT_INPUT = 0, EVT_ACTION = 1, EVT_TICK = 2, EVT_IO = 3 };

struct Event {
  EventKind kind;
  uint16_t code;   // e.g., BTN codes, ACTION codes
  int16_t  value;  // e.g., encoder delta, bool flags
};

#endif // EVENTS_H
