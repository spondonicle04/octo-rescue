#pragma once
#include "config_pins.h"
#include "config_features.h"
#include "board_profile_MEGA2560.h"
#include "config_checks.h"
#include "input_codes.h"   // ensures IN_BACK, IN_SELECT, IN_DOWN, IN_UP exist

// // ---- Generic key codes Called in menu object/object_classes
// #ifndef KEY_BACK
// #define KEY_BACK   0
// #endif
// #ifndef KEY_SELECT
// #define KEY_SELECT 1
// #endif
// #ifndef KEY_DOWN
// #define KEY_DOWN   2
// #endif
// #ifndef KEY_UP
// #define KEY_UP     3
// #
#include "input_codes.h"  // IN_BACK, IN_SELECT, IN_DOWN, IN_UP

#define KEY_BACK   IN_BACK
#define KEY_SELECT IN_SELECT
#define KEY_DOWN   IN_DOWN
#define KEY_UP     IN_UP

#define DEBUG_SERIAL 1   // set to 0 to strip all debug I/O
#define DEBUG_BAUD   115200

