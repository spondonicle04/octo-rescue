#include "menu_led.h"
#include <U8g2lib.h>
#include "ui_draw.h"
#include "settings_store.h"
#include "context_state.h"
#include "context_registry.h"

// Simple preset colors (name stored in PROGMEM + RGB)
struct Preset { const char* nameP; uint32_t rgb; };
static const char N_WHITE[]  PROGMEM = "White";
static const char N_WARM[]   PROGMEM = "Warm";
static const char N_RED[]    PROGMEM = "Red";
static const char N_GREEN[]  PROGMEM = "Green";
static const char N_BLUE[]   PROGMEM = "Blue";
static const char N_CYAN[]   PROGMEM = "Cyan";
static const char N_MAG[]    PROGMEM = "Magenta";
static const char N_YEL[]    PROGMEM = "Yellow";
static const Preset PRESETS[] PROGMEM = {
  { N_WHITE, 0xFFFFFF },
  { N_WARM,  0xFFA040 },
  { N_RED,   0xFF0000 },
  { N_GREEN, 0x00FF00 },
  { N_BLUE,  0x0080FF },
  { N_CYAN,  0x00FFFF },
  { N_MAG,   0xFF00FF },
  { N_YEL,   0xFFFF00 },
};
static const uint8_t NPRE = sizeof(PRESETS)/sizeof(PRESETS[0]);

static uint8_t findPresetIndex(uint32_t rgb) {
  for (uint8_t i = 0; i < NPRE; ++i) {
    uint32_t pr = pgm_read_dword(&PRESETS[i].rgb);
    if (pr == rgb) return i;
  }
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
  static const char T_LED_OPTS[]  PROGMEM = "LED Options";
  static const char L_BRIGHT[]    PROGMEM = "Brightness";
  static const char L_HIT[]       PROGMEM = "Hit Color";
  static const char L_STEP[]      PROGMEM = "Step Color";
  static const char L_SAVE[]      PROGMEM = "Save";
  static const char L_SAVE_SEL[]  PROGMEM = "> Save";

  g->firstPage();
  do {
    drawTitleWithLines_P(g, T_LED_OPTS, 12, 6);
    g->setFont(u8g2_font_6x10_tf);
    char vb[8]; snprintf(vb, sizeof(vb), "%u%%", (unsigned)((brightness * 100u) / 255u));
    char lab[18];
    strncpy_P(lab, L_BRIGHT, sizeof(lab)-1); lab[sizeof(lab)-1] = '\0';
    drawLineR(g, 26, lab, vb, sel == 0);
    strncpy_P(lab, L_HIT, sizeof(lab)-1); lab[sizeof(lab)-1] = '\0';
    const char* pHit = (const char*)pgm_read_ptr(&PRESETS[hitIndex].nameP);
    char vhit[16]; strncpy_P(vhit, pHit, sizeof(vhit)-1); vhit[sizeof(vhit)-1] = '\0';
    drawLineR(g, 38, lab, vhit, sel == 1);
    strncpy_P(lab, L_STEP, sizeof(lab)-1); lab[sizeof(lab)-1] = '\0';
    const char* pStep = (const char*)pgm_read_ptr(&PRESETS[stepIndex].nameP);
    char vstep[16]; strncpy_P(vstep, pStep, sizeof(vstep)-1); vstep[sizeof(vstep)-1] = '\0';
    drawLineR(g, 50, lab, vstep, sel == 2);
    strncpy_P(lab, (sel == 3) ? L_SAVE_SEL : L_SAVE, sizeof(lab)-1); lab[sizeof(lab)-1] = '\0';
    drawLineR(g, 62, lab, nullptr, sel == 3);
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
      s.ws_hit_color  = pgm_read_dword(&PRESETS[hitIndex].rgb);
      s.ws_step_color = pgm_read_dword(&PRESETS[stepIndex].rgb);
      settings_save();
      // No LED runtime apply yet (placeholder)
      (void)goBack();
    } else if (sel == 0) {
      setContextByName_P(PSTR("LED_BRIGHTNESS"));
      return;
    } else if (sel == 1) {
      setContextByName_P(PSTR("LED_HIT_COLOR"));
      return;
    } else if (sel == 2) {
      setContextByName_P(PSTR("LED_STEP_COLOR"));
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
    static const char T_LED_BRIGHT[] PROGMEM = "LED Brightness";
    static const char MSG_ADJ[]      PROGMEM = "Use Up/Down to adjust";
    g->firstPage();
    do {
      drawTitleWithLines_P(g, T_LED_BRIGHT, 12, 6);
      g->setFont(u8g2_font_6x10_tf);
      drawProgmemStr(g, 4, 38, MSG_ADJ);
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
    const char* p = (const char*)pgm_read_ptr(&PRESETS[idx].nameP);
    char name[16]; strncpy_P(name, p, sizeof(name)-1); name[sizeof(name)-1] = '\0';
    static const char T_HIT[]   PROGMEM = "Hit Color";
    static const char MSG_PRE[] PROGMEM = "Up/Down to change preset";
    g->firstPage();
    do {
      drawTitleWithLines_P(g, T_HIT, 12, 6);
      g->setFont(u8g2_font_6x10_tf);
      drawProgmemStr(g, 4, 38, MSG_PRE);
      int w = g->getDisplayWidth(); int tw = g->getUTF8Width(name);
      g->drawStr(w - tw - 4, 54, name);
    } while (g->nextPage());
  }
  void handleInput(int input) override {
    auto& s = settings_get();
    uint8_t idx = findPresetIndex(s.ws_hit_color);
    if (input == KEY_UP)   { idx = (uint8_t)((idx + 1) % NPRE); s.ws_hit_color = pgm_read_dword(&PRESETS[idx].rgb); }
    else if (input == KEY_DOWN) { idx = (uint8_t)((idx + NPRE - 1) % NPRE); s.ws_hit_color = pgm_read_dword(&PRESETS[idx].rgb); }
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
    const char* p = (const char*)pgm_read_ptr(&PRESETS[idx].nameP);
    char name[16]; strncpy_P(name, p, sizeof(name)-1); name[sizeof(name)-1] = '\0';
    static const char T_STEP[]  PROGMEM = "Step Color";
    static const char MSG_PRE[] PROGMEM = "Up/Down to change preset";
    g->firstPage();
    do {
      drawTitleWithLines_P(g, T_STEP, 12, 6);
      g->setFont(u8g2_font_6x10_tf);
      drawProgmemStr(g, 4, 38, MSG_PRE);
      int w = g->getDisplayWidth(); int tw = g->getUTF8Width(name);
      g->drawStr(w - tw - 4, 54, name);
    } while (g->nextPage());
  }
  void handleInput(int input) override {
    auto& s = settings_get();
    uint8_t idx = findPresetIndex(s.ws_step_color);
    if (input == KEY_UP)   { idx = (uint8_t)((idx + 1) % NPRE); s.ws_step_color = pgm_read_dword(&PRESETS[idx].rgb); }
    else if (input == KEY_DOWN) { idx = (uint8_t)((idx + NPRE - 1) % NPRE); s.ws_step_color = pgm_read_dword(&PRESETS[idx].rgb); }
    else if (input == KEY_BACK || input == KEY_SELECT) { (void)goBack(); }
  }
  void update(void* /*gfx*/) override {}
};

static LedStepColorContext ledStepColorContext;
void registerLedStepColorContext() { registerContext("LED_STEP_COLOR", &ledStepColorContext); }
