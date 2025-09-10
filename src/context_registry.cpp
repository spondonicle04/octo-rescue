// context_registry.cpp
// Implementation of global context registration and lookup.

#include "context_registry.h"
#include "debug.h"
#include <string.h> // for strcmp
#include <avr/pgmspace.h>

static const char* contextNames[MAX_CONTEXTS];
static ContextObject* contextPointers[MAX_CONTEXTS];
static uint8_t contextCount = 0;

void registerContext(const char* name, ContextObject* ctx) {
  if (contextCount >= MAX_CONTEXTS) return; // silently ignore overflow
  contextNames[contextCount] = name;
  contextPointers[contextCount] = ctx;
  contextCount++;
}

ContextObject* getContextByName(const char* nameRAM) {
  if (!nameRAM) return nullptr;
  for (uint8_t i = 0; i < contextCount; ++i) {
    // Try comparing with the stored pointer treated as PROGMEM first.
    if (strcmp_P(nameRAM, (PGM_P)contextNames[i]) == 0) {
      return contextPointers[i];
    }
    // Fallback: if the name was stored as a RAM string, compare directly.
    if (strcmp(contextNames[i], nameRAM) == 0) {
      return contextPointers[i];
    }
  }
  return nullptr;
}

uint8_t getRegisteredContextCount() {
  return contextCount;
}

ContextObject* getContextByIndex(uint8_t index) {
  if (index >= contextCount) return nullptr;
  return contextPointers[index];
}

void debugPrintRegisteredContextCount() {
#if DEBUG_SERIAL
  Serial.print(F("contexts: "));
  Serial.println(getRegisteredContextCount());
#endif
}
