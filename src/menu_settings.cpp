#include "menu_settings.h"
#include "context_registry.h"
#include "context_state.h"
#include <U8g2lib.h>
#include <avr/pgmspace.h>
#include "ui_draw.h"
#include "events.h"
#include "transitions.h"

// ----- PROGMEM labels -----
const char S_ITEM_0[] PROGMEM = "Audio Settings";
const char S_ITEM_1[] PROGMEM = "Display Options";
const char S_ITEM_2[] PROGMEM = "LED Options";
const char S_ITEM_3[] PROGMEM = "MIDI Config";
const char S_ITEM_4[] PROGMEM = "Back";
const char* const MENU_SETTINGS_ITEMS[] PROGMEM = {
  S_ITEM_0, S_ITEM_1, S_ITEM_2, S_ITEM_3, S_ITEM_4
};

// ----- PROGMEM destinations -----
const char* const MENU_SETTINGS_SUBS[] PROGMEM = {
  "MAIN_MENU",        // TODO: "AUDIO_SETTINGS"
  "DISPLAY_OPTIONS",  // display options
  "LED_OPTIONS",      // led options
  "MAIN_MENU",        // TODO: "MIDI_CONFIG"
  "MAIN_MENU",
};
static const uint8_t MENU_SETTINGS_COUNT =
  sizeof(MENU_SETTINGS_ITEMS) / sizeof(MENU_SETTINGS_ITEMS[0]);

const char TITLE_SETTINGS[] PROGMEM = "Settings";

SettingsMenuContext::SettingsMenuContext()
  : MenuObject("SETTINGS", "MAIN_MENU",
               MENU_SETTINGS_SUBS, MENU_SETTINGS_COUNT,
               MENU_SETTINGS_ITEMS, MENU_SETTINGS_COUNT) {}

void SettingsMenuContext::draw(void* gfx) {
  U8G2* gfxU8 = (U8G2*)gfx;
  char t[24]; strncpy_P(t, TITLE_SETTINGS, sizeof(t)-1); t[sizeof(t)-1] = '\0';
  drawMenuPagedP(gfxU8, t, items, itemCount, selectedIndex, 4);
}

void SettingsMenuContext::handleInput(int input) {
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
void SettingsMenuContext::update(void* /*gfx*/) {}

SettingsMenuContext settingsMenuContext;
void registerSettingsMenuContext() { registerContext("SETTINGS", &settingsMenuContext); }
