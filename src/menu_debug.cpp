#include "menu_debug.h"
#include "context_registry.h"
#include "context_state.h"
#include <U8g2lib.h>
#include <avr/pgmspace.h>
#include "ui_draw.h"
#include "events.h"
#include "transitions.h"


// ----- PROGMEM labels -----
const char D_ITEM_0[] PROGMEM = "System Info";
const char D_ITEM_1[] PROGMEM = "Test Beep";
const char D_ITEM_2[] PROGMEM = "Back";
const char* const MENU_DEBUG_ITEMS[] PROGMEM = {
  D_ITEM_0, D_ITEM_1, D_ITEM_2
};

// ----- PROGMEM destinations -----
const char* const MENU_DEBUG_SUBS[] PROGMEM = {
  "MAIN_MENU",   // TODO: "SYS_INFO"
  "MAIN_MENU",   // TODO: "TEST_BEEP"
  "MAIN_MENU",
};
static const uint8_t MENU_DEBUG_COUNT =
  sizeof(MENU_DEBUG_ITEMS) / sizeof(MENU_DEBUG_ITEMS[0]);

const char TITLE_DEBUG[] PROGMEM = "Debug";

DebugMenuContext::DebugMenuContext()
  : MenuObject("DEBUG", "MAIN_MENU",
               MENU_DEBUG_SUBS, MENU_DEBUG_COUNT,
               MENU_DEBUG_ITEMS, MENU_DEBUG_COUNT) {}

void DebugMenuContext::draw(void* gfx) {
  U8G2* gfxU8 = (U8G2*)gfx;
  char t[24]; strncpy_P(t, TITLE_DEBUG, sizeof(t)-1); t[sizeof(t)-1] = '\0';
  drawMenuPagedP(gfxU8, t, items, itemCount, selectedIndex, 4);
}

void DebugMenuContext::handleInput(int input) {
  if (input == 1) {
    if (subcontextNames && selectedIndex < subcontextCount) {
      const char* dest = (const char*)pgm_read_ptr(&subcontextNames[selectedIndex]);
      setContextByName(dest);
    }
    return;
  } else if (input == 2) {
    selectedIndex = (uint8_t)((selectedIndex + 1) % itemCount);
  } else if (input == 3) {
    selectedIndex = (uint8_t)((selectedIndex + itemCount - 1) % itemCount);
  } else if (input == 0) {
    if (goBack()) return;
  }
}
void DebugMenuContext::update(void* /*gfx*/) {}

DebugMenuContext debugMenuContext;
__attribute__((constructor))
void registerDebugMenuContext() { registerContext("DEBUG", &debugMenuContext); }
