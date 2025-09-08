// context_state.h
#ifndef CONTEXT_STATE_H
#define CONTEXT_STATE_H

#include "object_classes.h"

// Switch to a context by its registered name. Returns true if switched.
bool setContextByName(const char* name);

// Navigate back (history > parentName > "MAIN"). Returns true if switched.
bool goBack();

// Read-only pointer to the current context (may be null at boot).
ContextObject* currentContext();

#endif
