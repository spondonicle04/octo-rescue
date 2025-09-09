#include "menu_main.h"
#include "context_registry.h"
#include "context_state.h"
#include <U8g2lib.h>
#include <avr/pgmspace.h>
#include "ui_draw.h"
#include "events.h"
#include "transitions.h"

// ----- PROGMEM labels -----
const char M_MAIN_0[] PROGMEM = "Pattern Menu";
const char M_MAIN_1[] PROGMEM = "Live Menu";
const char M_MAIN_2[] PROGMEM = "Settings";
const char M_MAIN_3[] PROGMEM = "Save Menu";
const char M_MAIN_4[] PROGMEM = "Debug Menu";
const char* const MENU_MAIN_ITEMS[] PROGMEM = {
  M_MAIN_0, M_MAIN_1, M_MAIN_2, M_MAIN_3, M_MAIN_4
};

// ----- PROGMEM destinations (same order) -----
const char* const MENU_MAIN_SUBS[] PROGMEM = {
  "PATTERN_MENU",
  "LIVE_MODE",
  "SETTINGS",
  "SAVE_MENU",
  "DEBUG",
};
static const uint8_t MENU_MAIN_COUNT =
  sizeof(MENU_MAIN_ITEMS) / sizeof(MENU_MAIN_ITEMS[0]);

// Optional: title in PROGMEM
const char TITLE_MAIN[] PROGMEM = "Main Menu";

MainMenuContext::MainMenuContext()
  : MenuObject("MAIN_MENU", "BOOT",
               /*subs*/ MENU_MAIN_SUBS, /*subCount*/ MENU_MAIN_COUNT,
               /*menuItems*/ MENU_MAIN_ITEMS, /*menuItemCount*/ MENU_MAIN_COUNT) {}

void MainMenuContext::draw(void* gfx) {
  U8G2* gfxU8 = (U8G2*)gfx;
  char t[24]; strncpy_P(t, TITLE_MAIN, sizeof(t)-1); t[sizeof(t)-1] = '\0';
  drawMenuPagedP(gfxU8, t, items, itemCount, selectedIndex, 4);
}

void MainMenuContext::handleInput(int input) {
  if (input == 1) { // select
    if (subcontextNames && selectedIndex < subcontextCount) {
      const char* dest = (const char*)pgm_read_ptr(&subcontextNames[selectedIndex]);
      setContextByName(dest);
    }
    return;
  } else if (input == 2) { // down
    selectedIndex = (uint8_t)((selectedIndex + 1) % itemCount);
  } else if (input == 3) { // up
    selectedIndex = (uint8_t)((selectedIndex + itemCount - 1) % itemCount);
  } else if (input == 0) { // back
    if (goBack()) return;
  }
}

void MainMenuContext::update(void* /*gfx*/) {}

MainMenuContext mainMenuContext;
void registerMainMenuContext() { registerContext("MAIN_MENU", &mainMenuContext); }
