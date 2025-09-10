// context_state.h
#ifndef CONTEXT_STATE_H
#define CONTEXT_STATE_H

#include "object_classes.h"

// Switch to a context by its registered name (RAM string). Returns true if switched.
bool setContextByName(const char* name);

// Switch using a PROGMEM string (PGM_P). Copies to a small buffer then dispatches.
bool setContextByName_P(const char* nameP);

// Navigate back (history > parentName > "MAIN"). Returns true if switched.
bool goBack();

// Read-only pointer to the current context (may be null at boot).
ContextObject* currentContext();

#endif
