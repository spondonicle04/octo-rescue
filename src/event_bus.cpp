// event_bus.cpp
#include "event_bus.h"
#include <Arduino.h>

static const uint8_t Q_LEN = 32;            // must be power of two
static volatile uint8_t headIdx = 0;
static volatile uint8_t tailIdx = 0;
static Event q[Q_LEN];

static inline uint8_t advance(uint8_t i){ return (uint8_t)((i + 1) & (Q_LEN - 1)); }

bool eb_pushFromISR(const Event& e){
  uint8_t n = advance(headIdx);
  if (n == tailIdx) return false;           // full
  q[headIdx] = e;                            // write element
  headIdx = n;                               // publish
  return true;
}

bool eb_push(const Event& e){
  // main-loop producer: prevent race with ISR producer
  noInterrupts();
  bool ok = eb_pushFromISR(e);
  interrupts();
  return ok;
}

bool eb_pop(Event& e){
  if (tailIdx == headIdx) return false;     // empty
  e = q[tailIdx];
  tailIdx = advance(tailIdx);
  return true;
}
