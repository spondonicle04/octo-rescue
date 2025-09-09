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
  U8G2* g = (U8G2*)gfx;
  g->firstPage();
  do {
    drawTitleWithLines(g, "Display Options", 12, 6);
    g->setFont(u8g2_font_6x10_tf);

    char v1[8]; snprintf(v1, sizeof(v1), "%u%%", (unsigned)maxPct);
    drawLine(g, 26, "Max Brightness", v1, sel == 0);

    drawLine(g, 38, "Invert", invert ? "On" : "Off", sel == 1);

    drawLine(g, 50, "Unicorn", "UNiCORN!", sel == 2);

    drawLine(g, 62, sel == 3 ? "> Save" : "Save", nullptr, sel == 3);
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
      setContextByName("DISPLAY_BRIGHTNESS");
      return;
    } else if (sel == 1) {
      setContextByName("DISPLAY_INVERT");
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
    U8G2* g = (U8G2*)gfx;
    auto& s = settings_get();
    char v[12]; snprintf(v, sizeof(v), "%u%%", (unsigned)s.bl_max_percent);
    g->firstPage();
    do {
      drawTitleWithLines(g, "Max Brightness", 12, 6);
      g->setFont(u8g2_font_6x10_tf);
      int w = g->getDisplayWidth(); int tw = g->getUTF8Width(v);
      g->drawStr(4, 38, "Use Up/Down to adjust");
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
    U8G2* g = (U8G2*)gfx;
    auto& s = settings_get();
    const char* v = s.bl_invert ? "On" : "Off";
    g->firstPage();
    do {
      drawTitleWithLines(g, "Display Invert", 12, 6);
      g->setFont(u8g2_font_6x10_tf);
      g->drawStr(4, 38, "Up/Down or Select to toggle");
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
