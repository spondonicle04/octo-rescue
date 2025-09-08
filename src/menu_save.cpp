#include "menu_save.h"
#include "context_registry.h"
#include "context_state.h"
#include <U8g2lib.h>
#include <avr/pgmspace.h>
#include "ui_draw.h"
#include "events.h"
#include "transitions.h"

// ----- PROGMEM labels -----
const char SV_ITEM_0[] PROGMEM = "Save Pattern";
const char SV_ITEM_1[] PROGMEM = "Save All";
const char SV_ITEM_2[] PROGMEM = "Back";
const char* const MENU_SAVE_ITEMS[] PROGMEM = {
  SV_ITEM_0, SV_ITEM_1, SV_ITEM_2
};

// ----- PROGMEM destinations -----
const char* const MENU_SAVE_SUBS[] PROGMEM = {
  "MAIN_MENU",    // TODO: "SAVE_PATTERN"
  "MAIN_MENU",    // TODO: "SAVE_ALL"
  "MAIN_MENU",
};
static const uint8_t MENU_SAVE_COUNT =
  sizeof(MENU_SAVE_ITEMS) / sizeof(MENU_SAVE_ITEMS[0]);

const char TITLE_SAVE[] PROGMEM = "Save Menu";

SaveMenuContext::SaveMenuContext()
  : MenuObject("SAVE_MENU", "MAIN_MENU",
               MENU_SAVE_SUBS, MENU_SAVE_COUNT,
               MENU_SAVE_ITEMS, MENU_SAVE_COUNT) {}

void SaveMenuContext::draw(void* gfx) {
  U8G2* gfxU8 = (U8G2*)gfx;
  char t[24]; strncpy_P(t, TITLE_SAVE, sizeof(t)-1); t[sizeof(t)-1] = '\0';
  drawMenuPagedP(gfxU8, t, items, itemCount, selectedIndex, 4);
}

void SaveMenuContext::handleInput(int input) {
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
void SaveMenuContext::update(void* /*gfx*/) {}

SaveMenuContext saveMenuContext;
__attribute__((constructor))
void registerSaveMenuContext() { registerContext("SAVE_MENU", &saveMenuContext); }
