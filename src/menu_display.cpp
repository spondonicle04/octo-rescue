#include "menu_display.h"
#include <U8g2lib.h>
#include <avr/pgmspace.h>
#include "ui_draw.h"
#include "settings_store.h"
#include "context_state.h"
#include "context_registry.h"

DisplayOptionsContext::DisplayOptionsContext()
  : ContextObject("DISPLAY_OPTIONS", "SETTINGS", nullptr, 0), sel(0), initialized(false), maxPct(100), invert(false) {}

void DisplayOptionsContext::update(void* /*gfx*/) {
  // Always reflect latest settings so sub-screens edits show here.
  auto& s = settings_get();
  maxPct = s.bl_max_percent;
  invert = (s.bl_invert != 0);
  initialized = true;
}

static void drawLine(U8G2* g, int y, const char* label, const char* value, bool sel) {
  if (sel) { g->drawBox(0, y - 10, 128, 12); g->setDrawColor(0); }
  g->drawStr(4, y, label);
  if (value) {
    int w = g->getDisplayWidth();
    int tw = g->getUTF8Width(value);
    g->drawStr(w - tw - 4, y, value);
  }
  if (sel) { g->setDrawColor(1); }
}

void DisplayOptionsContext::draw(void* gfx) {
  static const char T_DISP_OPTS[]   PROGMEM = "Display Options";
  static const char L_MAX_BRIGHT[]  PROGMEM = "Max Brightness";
  static const char L_INVERT[]      PROGMEM = "Invert";
  static const char L_UNICORN[]     PROGMEM = "Unicorn";
  static const char L_SAVE[]        PROGMEM = "Save";
  static const char L_SAVE_SEL[]    PROGMEM = "> Save";
  static const char V_ON[]          PROGMEM = "On";
  static const char V_OFF[]         PROGMEM = "Off";
  static const char V_UNICORN[]     PROGMEM = "UNiCORN!";

  U8G2* g = (U8G2*)gfx;
  g->firstPage();
  do {
    drawTitleWithLines_P(g, T_DISP_OPTS, 12, 6);
    g->setFont(u8g2_font_6x10_tf);

    char v1[8]; snprintf(v1, sizeof(v1), "%u%%", (unsigned)maxPct);
    char lab[20];

    // Max Brightness
    strncpy_P(lab, L_MAX_BRIGHT, sizeof(lab)-1); lab[sizeof(lab)-1] = '\0';
    drawLine(g, 26, lab, v1, sel == 0);

    // Invert line with PROGMEM values
    strncpy_P(lab, L_INVERT, sizeof(lab)-1); lab[sizeof(lab)-1] = '\0';
    char v2[8]; strcpy_P(v2, invert ? V_ON : V_OFF);
    drawLine(g, 38, lab, v2, sel == 1);

    // Unicorn
    strncpy_P(lab, L_UNICORN, sizeof(lab)-1); lab[sizeof(lab)-1] = '\0';
    char v3[16]; strcpy_P(v3, V_UNICORN);
    drawLine(g, 50, lab, v3, sel == 2);

    // Save
    strncpy_P(lab, (sel == 3) ? L_SAVE_SEL : L_SAVE, sizeof(lab)-1); lab[sizeof(lab)-1] = '\0';
    drawLine(g, 62, lab, nullptr, sel == 3);
  } while (g->nextPage());
}

void DisplayOptionsContext::handleInput(int input) {
  if (input == KEY_DOWN) {
    sel = (uint8_t)((sel + 1) % 4);
  } else if (input == KEY_UP) {
    sel = (uint8_t)((sel + 3) % 4);
  } else if (input == KEY_SELECT) {
    if (sel == 3) {
      // Save
      auto& s = settings_get();
      s.bl_max_percent = maxPct;
      s.bl_invert = invert ? 1 : 0;
      settings_save();
      settings_apply_runtime();
      (void)goBack();
    } else if (sel == 0) {
      setContextByName_P(PSTR("DISPLAY_BRIGHTNESS"));
      return;
    } else if (sel == 1) {
      setContextByName_P(PSTR("DISPLAY_INVERT"));
      return;
    } else if (sel == 2) {
      // Unicorn easter egg: flip invert for fun
      invert = !invert;
    }
  } else if (input == KEY_BACK) {
    (void)goBack();
  }
}

DisplayOptionsContext displayOptionsContext;
void registerDisplayOptionsContext() { registerContext("DISPLAY_OPTIONS", &displayOptionsContext); }

// --- Subscreen: Display Brightness ---
class DisplayBrightnessContext : public ContextObject {
public:
  DisplayBrightnessContext() : ContextObject("DISPLAY_BRIGHTNESS", "DISPLAY_OPTIONS", nullptr, 0) {}
  void draw(void* gfx) override {
    static const char T_MAX_BRIGHT[] PROGMEM = "Max Brightness";
    static const char MSG_ADJ[]      PROGMEM = "Use Up/Down to adjust";
    U8G2* g = (U8G2*)gfx;
    auto& s = settings_get();
    char v[12]; snprintf(v, sizeof(v), "%u%%", (unsigned)s.bl_max_percent);
    g->firstPage();
    do {
      drawTitleWithLines_P(g, T_MAX_BRIGHT, 12, 6);
      g->setFont(u8g2_font_6x10_tf);
      drawProgmemStr(g, 4, 38, MSG_ADJ);
      int w = g->getDisplayWidth(); int tw = g->getUTF8Width(v);
      g->drawStr(w - tw - 4, 54, v);
    } while (g->nextPage());
  }
  void handleInput(int input) override {
    auto& s = settings_get();
    if (input == KEY_UP && s.bl_max_percent < 100) { s.bl_max_percent++; settings_apply_runtime(); }
    else if (input == KEY_DOWN && s.bl_max_percent > 0) { s.bl_max_percent--; settings_apply_runtime(); }
    else if (input == KEY_BACK || input == KEY_SELECT) { (void)goBack(); }
  }
  void update(void* /*gfx*/) override {}
};

static DisplayBrightnessContext displayBrightnessContext;
void registerDisplayBrightnessContext() { registerContext("DISPLAY_BRIGHTNESS", &displayBrightnessContext); }

// --- Subscreen: Display Invert ---
class DisplayInvertContext : public ContextObject {
public:
  DisplayInvertContext() : ContextObject("DISPLAY_INVERT", "DISPLAY_OPTIONS", nullptr, 0) {}
  void draw(void* gfx) override {
    static const char T_INV[]   PROGMEM = "Display Invert";
    static const char MSG_TGL[] PROGMEM = "Up/Down or Select to toggle";
    static const char V_ON[]    PROGMEM = "On";
    static const char V_OFF[]   PROGMEM = "Off";
    U8G2* g = (U8G2*)gfx;
    auto& s = settings_get();
    char v[8]; strcpy_P(v, s.bl_invert ? V_ON : V_OFF);
    g->firstPage();
    do {
      drawTitleWithLines_P(g, T_INV, 12, 6);
      g->setFont(u8g2_font_6x10_tf);
      drawProgmemStr(g, 4, 38, MSG_TGL);
      int w = g->getDisplayWidth(); int tw = g->getUTF8Width(v);
      g->drawStr(w - tw - 4, 54, v);
    } while (g->nextPage());
  }
  void handleInput(int input) override {
    auto& s = settings_get();
    if (input == KEY_UP || input == KEY_DOWN || input == KEY_SELECT) {
      s.bl_invert = s.bl_invert ? 0 : 1;
      settings_apply_runtime();
    } else if (input == KEY_BACK) {
      (void)goBack();
    }
  }
  void update(void* /*gfx*/) override {}
};

static DisplayInvertContext displayInvertContext;
void registerDisplayInvertContext() { registerContext("DISPLAY_INVERT", &displayInvertContext); }
