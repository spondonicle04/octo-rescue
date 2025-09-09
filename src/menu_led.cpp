#include "menu_led.h"
#include <U8g2lib.h>
#include "ui_draw.h"
#include "settings_store.h"
#include "context_state.h"
#include "context_registry.h"

// Simple preset colors (name + RGB)
struct Preset { const char* name; uint32_t rgb; };
static const Preset PRESETS[] = {
  { "White", 0xFFFFFF },
  { "Warm",  0xFFA040 },
  { "Red",   0xFF0000 },
  { "Green", 0x00FF00 },
  { "Blue",  0x0080FF },
  { "Cyan",  0x00FFFF },
  { "Magenta",0xFF00FF },
  { "Yellow",0xFFFF00 },
};
static const uint8_t NPRE = sizeof(PRESETS)/sizeof(PRESETS[0]);

static uint8_t findPresetIndex(uint32_t rgb) {
  for (uint8_t i = 0; i < NPRE; ++i) if (PRESETS[i].rgb == rgb) return i;
  return 0; // default to first
}

LedOptionsContext::LedOptionsContext()
  : ContextObject("LED_OPTIONS", "SETTINGS", nullptr, 0), sel(0), initialized(false), brightness(128), hitIndex(0), stepIndex(0) {}

void LedOptionsContext::update(void* /*gfx*/) {
  // Always reflect latest settings so sub-screens edits show here.
  auto& s = settings_get();
  brightness = s.ws_brightness;
  hitIndex   = findPresetIndex(s.ws_hit_color);
  stepIndex  = findPresetIndex(s.ws_step_color);
  initialized = true;
}

static void drawLineR(U8G2* g, int y, const char* label, const char* value, bool sel) {
  if (sel) { g->drawBox(0, y - 10, 128, 12); g->setDrawColor(0); }
  g->drawStr(4, y, label);
  if (value) {
    int w = g->getDisplayWidth(); int tw = g->getUTF8Width(value);
    g->drawStr(w - tw - 4, y, value);
  }
  if (sel) g->setDrawColor(1);
}

void LedOptionsContext::draw(void* gfx) {
  U8G2* g = (U8G2*)gfx;
  g->firstPage();
  do {
    drawTitleWithLines(g, "LED Options", 12, 6);
    g->setFont(u8g2_font_6x10_tf);
    char vb[8]; snprintf(vb, sizeof(vb), "%u%%", (unsigned)((brightness * 100u) / 255u));
    drawLineR(g, 26, "Brightness", vb, sel == 0);
    drawLineR(g, 38, "Hit Color", PRESETS[hitIndex].name, sel == 1);
    drawLineR(g, 50, "Step Color", PRESETS[stepIndex].name, sel == 2);
    drawLineR(g, 62, sel == 3 ? "> Save" : "Save", nullptr, sel == 3);
  } while (g->nextPage());
}

void LedOptionsContext::handleInput(int input) {
  if (input == KEY_DOWN) {
    sel = (uint8_t)((sel + 1) % 4);
  } else if (input == KEY_UP) {
    sel = (uint8_t)((sel + 3) % 4);
  } else if (input == KEY_SELECT) {
    if (sel == 3) {
      auto& s = settings_get();
      s.ws_brightness = brightness;
      s.ws_hit_color  = PRESETS[hitIndex].rgb;
      s.ws_step_color = PRESETS[stepIndex].rgb;
      settings_save();
      // No LED runtime apply yet (placeholder)
      (void)goBack();
    } else if (sel == 0) {
      setContextByName("LED_BRIGHTNESS");
      return;
    } else if (sel == 1) {
      setContextByName("LED_HIT_COLOR");
      return;
    } else if (sel == 2) {
      setContextByName("LED_STEP_COLOR");
      return;
    }
  } else if (input == KEY_BACK) {
    (void)goBack();
  }
}

LedOptionsContext ledOptionsContext;
void registerLedOptionsContext() { registerContext("LED_OPTIONS", &ledOptionsContext); }

// --- LED sub: Brightness ---
class LedBrightnessContext : public ContextObject {
public:
  LedBrightnessContext() : ContextObject("LED_BRIGHTNESS", "LED_OPTIONS", nullptr, 0) {}
  void draw(void* gfx) override {
    U8G2* g = (U8G2*)gfx;
    auto& s = settings_get();
    uint8_t pct = (uint16_t)s.ws_brightness * 100u / 255u;
    char v[16]; snprintf(v, sizeof(v), "%u (%u%%)", (unsigned)s.ws_brightness, (unsigned)pct);
    g->firstPage();
    do {
      drawTitleWithLines(g, "LED Brightness", 12, 6);
      g->setFont(u8g2_font_6x10_tf);
      g->drawStr(4, 38, "Use Up/Down to adjust");
      int w = g->getDisplayWidth(); int tw = g->getUTF8Width(v);
      g->drawStr(w - tw - 4, 54, v);
    } while (g->nextPage());
  }
  void handleInput(int input) override {
    auto& s = settings_get();
    if (input == KEY_UP && s.ws_brightness < 255) { s.ws_brightness++; }
    else if (input == KEY_DOWN && s.ws_brightness > 0) { s.ws_brightness--; }
    else if (input == KEY_BACK || input == KEY_SELECT) { (void)goBack(); }
  }
  void update(void* /*gfx*/) override {}
};

static LedBrightnessContext ledBrightnessContext;
void registerLedBrightnessContext() { registerContext("LED_BRIGHTNESS", &ledBrightnessContext); }

// --- LED sub: Hit Color ---
class LedHitColorContext : public ContextObject {
public:
  LedHitColorContext() : ContextObject("LED_HIT_COLOR", "LED_OPTIONS", nullptr, 0) {}
  void draw(void* gfx) override {
    U8G2* g = (U8G2*)gfx;
    auto& s = settings_get();
    uint8_t idx = findPresetIndex(s.ws_hit_color);
    const char* name = PRESETS[idx].name;
    g->firstPage();
    do {
      drawTitleWithLines(g, "Hit Color", 12, 6);
      g->setFont(u8g2_font_6x10_tf);
      g->drawStr(4, 38, "Up/Down to change preset");
      int w = g->getDisplayWidth(); int tw = g->getUTF8Width(name);
      g->drawStr(w - tw - 4, 54, name);
    } while (g->nextPage());
  }
  void handleInput(int input) override {
    auto& s = settings_get();
    uint8_t idx = findPresetIndex(s.ws_hit_color);
    if (input == KEY_UP)   { idx = (uint8_t)((idx + 1) % NPRE); s.ws_hit_color = PRESETS[idx].rgb; }
    else if (input == KEY_DOWN) { idx = (uint8_t)((idx + NPRE - 1) % NPRE); s.ws_hit_color = PRESETS[idx].rgb; }
    else if (input == KEY_BACK || input == KEY_SELECT) { (void)goBack(); }
  }
  void update(void* /*gfx*/) override {}
};

static LedHitColorContext ledHitColorContext;
void registerLedHitColorContext() { registerContext("LED_HIT_COLOR", &ledHitColorContext); }

// --- LED sub: Step Color ---
class LedStepColorContext : public ContextObject {
public:
  LedStepColorContext() : ContextObject("LED_STEP_COLOR", "LED_OPTIONS", nullptr, 0) {}
  void draw(void* gfx) override {
    U8G2* g = (U8G2*)gfx;
    auto& s = settings_get();
    uint8_t idx = findPresetIndex(s.ws_step_color);
    const char* name = PRESETS[idx].name;
    g->firstPage();
    do {
      drawTitleWithLines(g, "Step Color", 12, 6);
      g->setFont(u8g2_font_6x10_tf);
      g->drawStr(4, 38, "Up/Down to change preset");
      int w = g->getDisplayWidth(); int tw = g->getUTF8Width(name);
      g->drawStr(w - tw - 4, 54, name);
    } while (g->nextPage());
  }
  void handleInput(int input) override {
    auto& s = settings_get();
    uint8_t idx = findPresetIndex(s.ws_step_color);
    if (input == KEY_UP)   { idx = (uint8_t)((idx + 1) % NPRE); s.ws_step_color = PRESETS[idx].rgb; }
    else if (input == KEY_DOWN) { idx = (uint8_t)((idx + NPRE - 1) % NPRE); s.ws_step_color = PRESETS[idx].rgb; }
    else if (input == KEY_BACK || input == KEY_SELECT) { (void)goBack(); }
  }
  void update(void* /*gfx*/) override {}
};

static LedStepColorContext ledStepColorContext;
void registerLedStepColorContext() { registerContext("LED_STEP_COLOR", &ledStepColorContext); }
