#pragma once

#include <Arduino.h>
#include "config.h" // expects PIN_BL_PWM and PIN_BRIGHT_POT

// Set to 1 if your backlight driver inverts PWM (hardware dependent)
#ifndef BL_INVERT
#define BL_INVERT 0
#endif

// Poll/update cadence in milliseconds
#ifndef BL_UPDATE_MS
#define BL_UPDATE_MS 20
#endif

// IIR smoothing coefficients: new = (old*NUM + raw)/DEN
// e.g., 7/8 ≈ gentle smoothing; 15/16 is stronger
#ifndef BL_SMOOTH_NUM
#define BL_SMOOTH_NUM 7
#endif
#ifndef BL_SMOOTH_DEN
#define BL_SMOOTH_DEN 8
#endif

// Deadband in PWM counts to ignore tiny changes (0..255)
#ifndef BL_DEADBAND_PWM
#define BL_DEADBAND_PWM 2
#endif

// Optional minimum PWM to avoid near-off flicker (0 disables)
#ifndef BL_MIN_PWM
#define BL_MIN_PWM 0
#endif

// Raise Timer1 PWM frequency (pins 11/12 on Mega2560) by setting prescaler.
// 1 => ~31 kHz (phase-correct), 8/64/etc for lower. Set to 0 to skip.
#ifndef BL_SET_TIMER1_PRESCALER
#define BL_SET_TIMER1_PRESCALER 1
#endif
#ifndef BL_TIMER1_PRESCALER
#define BL_TIMER1_PRESCALER 1   // valid: 1, 8, 64, 256, 1024
#endif

// Initialize backlight PWM pin and any defaults
void hal_backlight_setup();

// Periodically read the brightness pot and update PWM
void hal_backlight_poll();

// Runtime configuration hooks
void bl_set_max_percent(uint8_t percent_0_100);
void bl_set_invert(bool invert);
uint8_t bl_get_max_percent();
bool bl_get_invert();
