// menu_boot.cpp
// Boot context: splash screen or welcome screen
#include "menu_boot.h"
#include "context_registry.h"
#include "object_classes.h"
#include "context_state.h"
#include "events.h"
#include "transitions.h"
#include "ui_draw.h"

static const Transition XXX_TRANSITIONS[] = {
  {"LIVE_MODE", 0, "MAIN_MENU"},       // example for Live
  {"PATTERN_MENU", 0, "MAIN_MENU"},    // example for Pattern
  {"SAVE_MENU", 0, "MAIN_MENU"},       // example for Save
  {"DEBUG", 0, "MAIN_MENU"},           // example for Debug
};
static const unsigned XXX_COUNT = sizeof(XXX_TRANSITIONS)/sizeof(XXX_TRANSITIONS[0]);

// --- BootContext class definition ---
BootContext::BootContext()
  : ContextObject("BOOT", nullptr, nullptr, 0) {}

// Define ALL declared virtuals with EXACT signatures
void BootContext::draw(void* gfx) {
  U8G2* gfxU8 = static_cast<U8G2*>(gfx);
  if (!gfxU8) return;

  static const char TITLE_BOOT[] PROGMEM = "Boot";
  static const char BOOT_STARTING[] PROGMEM = "Starting...";

  gfxU8->firstPage();
  do {
    drawTitleWithLines_P(gfxU8, TITLE_BOOT, 12, 6);
    gfxU8->setFont(u8g2_font_6x10_tf);
    drawProgmemStr(gfxU8, 4, 28, BOOT_STARTING);
  } while (gfxU8->nextPage());
}

void BootContext::handleInput(int input) {
  if (input == KEY_SELECT || input == KEY_BACK) {
    setContextByName_P(PSTR("MAIN_MENU"));
  }
}
// --- Global instance + registration ---
BootContext bootContext;
void registerBootContext() { registerContext("BOOT", &bootContext); }
