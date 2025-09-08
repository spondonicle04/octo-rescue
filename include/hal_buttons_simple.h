// hal_buttons_simple.h
#ifndef HAL_BUTTONS_SIMPLE_H
#define HAL_BUTTONS_SIMPLE_H


#include <Arduino.h>
#include "event_bus.h"
#include "config.h" // expects BTN_SELECT, BTN_DOWN, BTN_UP, BTN_LIVE (and optional BTN_BACK)


// Function-key semantic codes carried in Event.a when src == SRC_FN_KEYS
enum FnKey : uint8_t {
FN_NONE = 0,
FN_SELECT = 1,
FN_DOWN = 2,
FN_UP = 3,
FN_LIVE = 4,
FN_BACK = 5
};


void hal_buttons_setup();
void hal_buttons_poll();


#endif // HAL_BUTTONS_SIMPLE_H

