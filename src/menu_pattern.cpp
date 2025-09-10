#include "menu_pattern.h"
#include "context_registry.h"
#include "context_state.h"
#include <U8g2lib.h>
#include <avr/pgmspace.h>
#include "ui_draw.h"
#include "events.h"
#include "transitions.h"

// ----- PROGMEM labels -----
const char P_ITEM_0[] PROGMEM = "Pattern 1";
const char P_ITEM_1[] PROGMEM = "Pattern 2";
const char P_ITEM_2[] PROGMEM = "Pattern 3";
const char* const MENU_PATTERN_ITEMS[] PROGMEM = {
  P_ITEM_0, P_ITEM_1, P_ITEM_2
};

// ----- PROGMEM destinations -----
const char* const MENU_PATTERN_SUBS[] PROGMEM = {
  "MAIN_MENU",   // Select goes back for now (adjust when sub-menus exist)
  "MAIN_MENU",
  "MAIN_MENU",
};
static const uint8_t MENU_PATTERN_COUNT =
  sizeof(MENU_PATTERN_ITEMS) / sizeof(MENU_PATTERN_ITEMS[0]);

const char TITLE_PATTERN[] PROGMEM = "Pattern Menu";

PatternMenuContext::PatternMenuContext()
  : MenuObject("PATTERN_MENU", "MAIN_MENU",
               MENU_PATTERN_SUBS, MENU_PATTERN_COUNT,
               MENU_PATTERN_ITEMS, MENU_PATTERN_COUNT) {}

void PatternMenuContext::draw(void* gfx) {
  U8G2* gfxU8 = (U8G2*)gfx;
  char t[24]; strncpy_P(t, TITLE_PATTERN, sizeof(t)-1); t[sizeof(t)-1] = '\0';
  drawMenuPagedP(gfxU8, t, items, itemCount, selectedIndex, 4);
}

void PatternMenuContext::handleInput(int input) {
  if (input == 1) {
    if (subcontextNames && selectedIndex < subcontextCount) {
      const char* dest = (const char*)pgm_read_ptr(&subcontextNames[selectedIndex]);
      setContextByName_P(dest);
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
void PatternMenuContext::update(void* /*gfx*/) {}

PatternMenuContext patternMenuContext;
void registerPatternMenuContext() { registerContext("PATTERN_MENU", &patternMenuContext); }
