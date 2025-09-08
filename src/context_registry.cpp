// context_registry.cpp
// Implementation of global context registration and lookup.

#include "context_registry.h"
#include <string.h> // for strcmp

static const char* contextNames[MAX_CONTEXTS];
static ContextObject* contextPointers[MAX_CONTEXTS];
static uint8_t contextCount = 0;

void registerContext(const char* name, ContextObject* ctx) {
  if (contextCount >= MAX_CONTEXTS) return; // silently ignore overflow
  contextNames[contextCount] = name;
  contextPointers[contextCount] = ctx;
  contextCount++;
}

ContextObject* getContextByName(const char* name) {
  for (uint8_t i = 0; i < contextCount; ++i) {
    if (strcmp(contextNames[i], name) == 0) {
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
