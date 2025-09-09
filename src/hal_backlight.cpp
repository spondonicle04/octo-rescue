// hal_backlight.cpp
#include <Arduino.h>
#include "hal_backlight.h"
#include <avr/io.h>

// Simple smoothing and rate limit to avoid flicker/jitter
static uint16_t smoothAdc = 0;           // smoothed 0..1023
static uint8_t lastDuty = 255;           // last PWM duty 0..255
static unsigned long lastUpdateMs = 0;   // update cadence control
static uint8_t s_maxPercent = 100;       // 0..100 runtime cap
// s_potInvert: user setting to flip knob direction (not PWM polarity)
static bool    s_potInvert = false;

// Map 10-bit ADC to 8-bit PWM with basic scaling
static inline uint8_t adcToPwm(uint16_t v) {
  // Optionally invert POT direction
  if (s_potInvert) v = (uint16_t)(1023 - v);
  // Scale ~0..1023 -> 0..255 (integer divide by ~4)
  uint8_t level = (uint8_t)((v + 2) >> 2); // brightness level
  // Apply max-percent cap
  level = (uint8_t)((uint16_t)level * (uint16_t)s_maxPercent / 100u);
  // Map to PWM considering fixed hardware polarity
  uint8_t pwm = level;
#if BL_INVERT
  pwm = (uint8_t)(255 - pwm);
#endif
  // Optional: enforce a small minimum to avoid near-off flicker
#if BL_MIN_PWM > 0
  if (pwm < BL_MIN_PWM) pwm = BL_MIN_PWM;
#endif
  return pwm;
}

void hal_backlight_setup() {
  pinMode(PIN_BL_PWM, OUTPUT);
  // Read initial pot to seed smoothing
  uint16_t a0 = analogRead(PIN_BRIGHT_POT);
  smoothAdc = a0;
  lastDuty = adcToPwm(smoothAdc);
  analogWrite(PIN_BL_PWM, lastDuty);

#if BL_SET_TIMER1_PRESCALER
  // Bump Timer1 PWM frequency for pins 11/12 to reduce visible flicker/whine.
  // Arduino core uses Timer1 for PWM on pins 11/12. Adjust prescaler safely.
  // Mapping: 1->0x01, 8->0x02, 64->0x03, 256->0x04, 1024->0x05
  uint8_t csBits = 0x03; // default 64 if unrecognized
  #if BL_TIMER1_PRESCALER == 1
    csBits = 0x01;
  #elif BL_TIMER1_PRESCALER == 8
    csBits = 0x02;
  #elif BL_TIMER1_PRESCALER == 64
    csBits = 0x03;
  #elif BL_TIMER1_PRESCALER == 256
    csBits = 0x04;
  #elif BL_TIMER1_PRESCALER == 1024
    csBits = 0x05;
  #endif
  // Only touch if Timer1 exists on this MCU
  #ifdef TCCR1B
    TCCR1B = (TCCR1B & 0b11111000) | csBits;
  #endif
#endif
}

// Runtime config
void bl_set_max_percent(uint8_t percent_0_100) {
  if (percent_0_100 > 100) percent_0_100 = 100;
  s_maxPercent = percent_0_100;
}
void bl_set_invert(bool invert) { s_potInvert = invert; }
uint8_t bl_get_max_percent() { return s_maxPercent; }
bool bl_get_invert() { return s_potInvert; }

void hal_backlight_poll() {
  const unsigned long now = millis();
  // Update rate limit
  if ((now - lastUpdateMs) < BL_UPDATE_MS) return;
  lastUpdateMs = now;

  // Read and smooth (simple IIR: 7/8 old + 1/8 new)
  uint16_t raw = analogRead(PIN_BRIGHT_POT);
  smoothAdc = (uint16_t)(((uint32_t)smoothAdc * BL_SMOOTH_NUM + raw) / BL_SMOOTH_DEN);

  uint8_t duty = adcToPwm(smoothAdc);
  // Ignore tiny changes within deadband to prevent visible shimmer
  if ((duty > lastDuty ? (duty - lastDuty) : (lastDuty - duty)) > BL_DEADBAND_PWM) {
    lastDuty = duty;
    analogWrite(PIN_BL_PWM, duty);
  }
}
