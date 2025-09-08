#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include <string.h>
#include "context_state.h"  // setContextByName()
#include "object_classes.h" // ContextObject

struct Transition {
  const char* from;     // context name
  uint16_t onCode;      // input/action code
  const char* to;       // next context name
};

inline bool tryTransition(const char* from, uint16_t code, const Transition* table, unsigned int n) {
  for (unsigned int i = 0; i < n; ++i) {
    if (table[i].from && table[i].to && (code == table[i].onCode) && (strcmp(from, table[i].from) == 0)) {
      setContextByName(table[i].to);
      return true;
    }
  }
  return false;
}

#endif // TRANSITIONS_H
