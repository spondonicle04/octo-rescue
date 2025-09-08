#pragma once
#include <Arduino.h>
#include "config.h"   // may define DEBUG_SERIAL and DEBUG_BAUD

#ifndef DEBUG_BAUD
#define DEBUG_BAUD 115200
#endif
#ifndef DEBUG_SERIAL
#define DEBUG_SERIAL 1
#endif

#if DEBUG_SERIAL

// --- Init helper ---
inline void DEBUG_BEGIN() { Serial.begin(DEBUG_BAUD); }

// --- PROGMEM literal prints ---
inline void DPRINT(const __FlashStringHelper* s)  { Serial.print(s); }
inline void DPRINTLN(const __FlashStringHelper* s){ Serial.println(s); }

// --- Generic values (overloads) ---
template<typename T> inline void DPRINT(const T& v)            { Serial.print(v); }
template<typename T> inline void DPRINTLN(const T& v)          { Serial.println(v); }
template<typename T> inline void DPRINT(const T& v, int fmt)   { Serial.print(v, fmt); }
template<typename T> inline void DPRINTLN(const T& v, int fmt) { Serial.println(v, fmt); }

// --- Convenience for string LITERALS (auto F()) ---
#define DL(x)   DPRINT(F(x))
#define DLLN(x) DPRINTLN(F(x))

// --- AVR-safe tiny printf ---
inline void DPRINTF(const char* fmt, ...) {
  char buf[96];
  va_list ap; va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  Serial.print(buf);
}

// --- Extra helpers that folks expect ---
inline void DF(const __FlashStringHelper* s)  { Serial.print(s); }
inline void DFLN(const __FlashStringHelper* s){ Serial.println(s); }
inline void DHEX(uint32_t x) { Serial.print(F("0x")); Serial.println(x, HEX); }

#else  // DEBUG off → compile away

inline void DEBUG_BEGIN() {}
inline void DPRINT(const __FlashStringHelper*) {}
inline void DPRINTLN(const __FlashStringHelper*) {}
template<typename T> inline void DPRINT(const T&) {}
template<typename T> inline void DPRINTLN(const T&) {}
template<typename T> inline void DPRINT(const T&, int) {}
template<typename T> inline void DPRINTLN(const T&, int) {}
inline void DPRINTF(const char*, ...) {}
inline void DF(const __FlashStringHelper*) {}
inline void DFLN(const __FlashStringHelper*) {}
inline void DHEX(uint32_t) {}
#define DL(x)
#define DLLN(x)

#endif
