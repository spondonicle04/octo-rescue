// event_router.cpp (final)
#include "event_router.h"
#include "context_state.h"
#include "input_codes.h"
#include "event_bus.h"
#include "hal_buttons_simple.h"   // FnKey enums
#include <Arduino.h>

static void deliverToContext(InputCode code) {
  if (auto* ctx = currentContext()) ctx->handleInput((int)code);
}

static void handleFnKey(uint8_t fn) {
  switch (fn) {
    case FN_SELECT: deliverToContext(IN_SELECT); break;
    case FN_UP:     deliverToContext(IN_UP);     break;
    case FN_DOWN:   deliverToContext(IN_DOWN);   break;
    // case FN_LEFT:   deliverToContext(IN_LEFT);   break;
    // case FN_RIGHT:  deliverToContext(IN_RIGHT);  break;
    case FN_BACK:   if (!goBack()) setContextByName("MAIN_MENU");
    break; // or "MAIN"
    case FN_LIVE:   setContextByName("LIVE_MODE");
    break;
    default: break;
  }
}

void route_events() {
  Event e;
  while (eb_pop(e)) {
    // Accept only presses from function-key source
    if (e.type == EVT_KEY_DOWN && (e.src == SRC_FN_KEYS || e.src == 0)) {
      handleFnKey((uint8_t)e.a);
    }
    // Add other sources here later...
  }
}
