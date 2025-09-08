// context_state.cpp
#include "object_classes.h"
#include "context_registry.h"   // getContextByName(...)
#include "context_state.h"
#include <Arduino.h>

// Feature flag: enable/disable history at compile time.
// If CTX_HISTORY_ENABLED is defined elsewhere, we won't overwrite it.
#ifndef CTX_HISTORY_ENABLED
#define CTX_HISTORY_ENABLED 1
#endif

// --- Current context pointer (owned here) ---
static ContextObject* gCurrent = nullptr;

ContextObject* currentContext() { return gCurrent; }

// --- Tiny ring-buffer history (power-of-two size) ---
#if CTX_HISTORY_ENABLED
static const uint8_t HIST_LEN  = 8;
static const uint8_t HIST_MASK = HIST_LEN - 1;

static ContextObject* gHist[HIST_LEN];
static uint8_t gHead = 0;  // next write
static uint8_t gTail = 0;  // oldest

static inline bool histEmpty() { return gHead == gTail; }
static inline void histPush(ContextObject* c) {
  uint8_t nxt = (uint8_t)((gHead + 1) & HIST_MASK);
  if (nxt == gTail) {                 // full → drop oldest
    gTail = (uint8_t)((gTail + 1) & HIST_MASK);
  }
  gHist[gHead] = c;
  gHead = nxt;
}
static inline ContextObject* histPop() {
  if (histEmpty()) return nullptr;
  gHead = (uint8_t)((gHead - 1) & HIST_MASK);
  return gHist[gHead];
}
#endif // CTX_HISTORY_ENABLED

bool setContextByName(const char* nextName) {
  if (!nextName) return false;
  ContextObject* next = getContextByName(nextName);
  if (!next || next == gCurrent) return false;

  // Remember where we came from (if history is enabled)
#if CTX_HISTORY_ENABLED
  if (gCurrent) histPush(gCurrent);
#endif

  // Swap current pointer with interrupts briefly off
  noInterrupts();
  //ContextObject* prev = gCurrent;
  gCurrent = next;
  interrupts();

  // Do heavyweight work outside the critical section

  // No lifecycle hooks defined on ContextObject in your codebase.
  // If you add them later, you can re-enable calls here.

  // if (prev)       prev->onExit();
  //if (gCurrent)   gCurrent->onEnter();

  return true;
}

bool goBack() {
  // Prefer real history first (e.g., after LIVE jump)
#if CTX_HISTORY_ENABLED
  if (ContextObject* prev = histPop()) {
    noInterrupts();
    //ContextObject* old = gCurrent;
    gCurrent = prev;
    interrupts();
    // No lifecycle hooks defined on ContextObject in your codebase.
  // If you add them later, you can re-enable calls here.

  //  if (old)      old->onExit();
  //  if (gCurrent) gCurrent->onEnter();
    return true;
  }
#endif

  // Fallback: declared parent
  if (gCurrent && gCurrent->parentName && gCurrent->parentName[0]) {
    return setContextByName(gCurrent->parentName);
  }

  // Last resort: root
  return setContextByName("MAIN");
}
