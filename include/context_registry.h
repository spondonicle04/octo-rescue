// context_registry.h
// Provides global context registration and lookup by name.

#ifndef CONTEXT_REGISTRY_H
#define CONTEXT_REGISTRY_H

#include "object_classes.h"

// Maximum number of contexts that can be registered
#define MAX_CONTEXTS 16

// Registers a context by name
void registerContext(const char* name, ContextObject* ctx);

// Retrieves a context by its registered name
ContextObject* getContextByName(const char* name);

// Retrieves the number of registered contexts
uint8_t getRegisteredContextCount();

// Retrieves a context by index (for debug/UI iteration)
ContextObject* getContextByIndex(uint8_t index);

#endif // CONTEXT_REGISTRY_H
