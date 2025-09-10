#include "menu_debug.h"
#include "context_registry.h"
#include "context_state.h"
#include <U8g2lib.h>
#include <avr/pgmspace.h>
#include "ui_draw.h"
#include "events.h"
#include "transitions.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "config_pins.h"
#include "hal_backlight.h"


// ----- PROGMEM labels -----
const char D_ITEM_0[] PROGMEM = "System Info";
const char D_ITEM_1[] PROGMEM = "Test Beep";
const char D_ITEM_2[] PROGMEM = "Back";
const char* const MENU_DEBUG_ITEMS[] PROGMEM = {
  D_ITEM_0, D_ITEM_1, D_ITEM_2
};

// ----- PROGMEM destinations -----
const char* const MENU_DEBUG_SUBS[] PROGMEM = {
  "SYS_INFO",
  "TEST_BEEP",
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
void DebugMenuContext::update(void* /*gfx*/) {}

DebugMenuContext debugMenuContext;
void registerDebugMenuContext() { registerContext("DEBUG", &debugMenuContext); }

// -------------------------
// System Info / Diagnostics
// -------------------------
class SystemInfoContext : public ContextObject {
public:
  SystemInfoContext() : ContextObject("SYS_INFO", "DEBUG", nullptr, 0),
                        ran(false), showUntil(0) {
    result[0] = '\0';
  }
  void update(void* /*gfx*/) override {
    if (ran) return;
    ran = true;
    runDiagnostics();
    showUntil = millis() + 8000; // show results ~8s
  }
  void draw(void* gfx) override {
    static const char T_SYSINFO[] PROGMEM = "System Info";
    U8G2* g = (U8G2*)gfx;
    g->firstPage();
    do {
      drawTitleWithLines_P(g, T_SYSINFO, 12, 6);
      g->setFont(u8g2_font_6x10_tf);
      // Render up to 5 lines of results
      int y = 26;
      const int lineH = 12;
      const char* p = result;
      for (int lines = 0; lines < 5 && *p; ++lines) {
        const char* nl = strchr(p, '\n');
        char buf[48];
        if (!nl) {
          strncpy(buf, p, sizeof(buf)-1); buf[sizeof(buf)-1] = '\0';
          g->drawStr(2, y, buf);
          break;
        } else {
          size_t n = (size_t)(nl - p);
          if (n >= sizeof(buf)) n = sizeof(buf)-1;
          memcpy(buf, p, n); buf[n] = '\0';
          g->drawStr(2, y, buf);
          y += lineH; p = nl + 1;
        }
      }
    } while (g->nextPage());
  }
  void handleInput(int input) override {
    if (input == KEY_BACK || input == KEY_SELECT) {
      (void)goBack();
      return;
    }
    if (ran && millis() > showUntil) {
      (void)goBack();
    }
  }
private:
  bool ran;
  unsigned long showUntil;
  char result[192];

  void append(const char* s) {
    strncat(result, s, sizeof(result)-strlen(result)-1);
  }
  void appendP(const char* sP) {
    char buf[64];
    strncpy_P(buf, sP, sizeof(buf)-1);
    buf[sizeof(buf)-1] = '\0';
    append(buf);
  }
  void appendLineP(const char* sP) {
    appendP(sP); append("\n");
  }
  void appendLine(const char* s) {
    append(s); append("\n");
  }

  void runDiagnostics() {
    result[0] = '\0';
    static const char M_I2C[]  PROGMEM = "I2C: ";
    static const char M_MOK[]  PROGMEM = "MCP23017: OK";
    static const char M_MNOK[] PROGMEM = "MCP23017: not found";
    static const char M_ENOK[] PROGMEM = "EEPROM: not found";
    static const char M_SDOk[] PROGMEM = "SD: OK";
    static const char M_SDFa[] PROGMEM = "SD: FAIL";
    static const char M_LBL[]  PROGMEM = "Lights: backlight pulse";
    static const char M_WIG[]  PROGMEM = "Triggers: wiggle CS pins";
    static const char M_DONE[] PROGMEM = "Done.";
    // No intro line to keep key info within first page

    // I2C scan
    Wire.begin();
    appendP(M_I2C);
    bool any = false;
    for (uint8_t addr = 1; addr < 127; ++addr) {
      Wire.beginTransmission(addr);
      uint8_t err = Wire.endTransmission();
      if (err == 0) {
        any = true;
        char b[8]; snprintf(b, sizeof(b), "0x%02X ", addr);
        append(b);
      }
    }
    if (!any) {
      static const char M_NONE[] PROGMEM = "none";
      appendP(M_NONE);
    }
    append("\n");

    // MCP23017 presence
    bool mcp = probeI2C(I2C_ADDR_MCP);
    appendLineP(mcp ? M_MOK : M_MNOK);

    // EEPROM presence (24xx @ 0x50-0x57)
    bool eep = false; uint8_t eepAddr = 0x50;
    for (uint8_t a = 0x50; a <= 0x57; ++a) { if (probeI2C(a)) { eep = true; eepAddr = a; break; } }
    if (eep) {
      char b[24]; snprintf(b, sizeof(b), "EEPROM @0x%02X: OK", eepAddr); appendLine(b);
    } else {
      appendLineP(M_ENOK);
    }

    // SD card test and read text file
    if (SD.begin(PIN_SD_CS)) {
      appendLineP(M_SDOk);
      readTextFileToResult();
    } else {
      appendLineP(M_SDFa);
    }

    // Lights/backlight quick pulse
    appendLineP(M_LBL);
    pulseBacklight();

    // Trigger quick wiggle on DAC CS pins
    appendLineP(M_WIG);
    wiggleCsPins();

    appendLineP(M_DONE);
  }

  static bool probeI2C(uint8_t addr) {
    Wire.beginTransmission(addr);
    return (Wire.endTransmission() == 0);
  }

  void readTextFileToResult() {
    const char* candidates[] = { "/system.txt", "/info.txt" };
    File f;
    for (size_t i = 0; i < sizeof(candidates)/sizeof(candidates[0]); ++i) {
      f = SD.open(candidates[i]);
      if (f) break;
    }
    if (!f) {
      // find first .txt in root
      File root = SD.open("/");
      while (true) {
        File ent = root.openNextFile();
        if (!ent) break;
        if (!ent.isDirectory()) {
          const char* nm = ent.name();
          const char* dot = strrchr(nm, '.');
          if (dot && ((strcmp(dot, ".txt") == 0) || (strcmp(dot, ".TXT") == 0))) { f = ent; break; }
        }
        ent.close();
      }
      root.close();
    }
    if (f) {
      static const char M_SDTXT[] PROGMEM = "SD Text:";
      appendLineP(M_SDTXT);
      size_t remain = 120; // cap read to keep screen readable
      while (f.available() && remain--) {
        char c = (char)f.read();
        if (c == '\r') continue; // normalize
        char s[2] = { c, 0 };
        append(s);
      }
      append("\n");
      f.close();
    } else {
      static const char M_NONETXT[] PROGMEM = "(no .txt file found)";
      appendLineP(M_NONETXT);
    }
  }

  void pulseBacklight() {
    const uint8_t oldMax = bl_get_max_percent();
    for (uint8_t i = 0; i < 3; ++i) {
      bl_set_max_percent(100); delay(120);
      bl_set_max_percent(20);  delay(120);
    }
    bl_set_max_percent(oldMax);
  }

  void wiggleCsPins() {
    const uint8_t pins[] = { PIN_DAC_CS1, PIN_DAC_CS2, PIN_DAC_CS3, PIN_DAC_CS4, PIN_DAC_CS5, PIN_DAC_CS6 };
    const uint8_t n = (uint8_t)(sizeof(pins) / sizeof(pins[0]));
    for (uint8_t i = 0; i < n; ++i) pinMode(pins[i], OUTPUT);
    for (uint8_t r = 0; r < 2; ++r) {
      for (uint8_t i = 0; i < n; ++i) { digitalWrite(pins[i], LOW); delay(40); digitalWrite(pins[i], HIGH); }
    }
  }
};

static SystemInfoContext systemInfoContext;
void registerSystemInfoContext() { registerContext("SYS_INFO", &systemInfoContext); }

// -------------------------
// Test Beep
// -------------------------
class TestBeepContext : public ContextObject {
public:
  TestBeepContext() : ContextObject("TEST_BEEP", "DEBUG", nullptr, 0), freq(880), durMs(200) {}
  void draw(void* gfx) override {
    static const char T_BEEP[]      PROGMEM = "Test Beep";
    U8G2* g = (U8G2*)gfx;
    g->firstPage();
    do {
      drawTitleWithLines_P(g, T_BEEP, 12, 6);
      g->setFont(u8g2_font_6x10_tf);
      char line1[28]; snprintf(line1, sizeof(line1), "Freq: %u Hz", (unsigned)freq);
#ifdef PIN_BUZZER
      static const char MSG_CTRL[]    PROGMEM = "Select=Beep  Up/Down=Adj  Back";
      char line2[28]; snprintf(line2, sizeof(line2), "Pin: %u", (unsigned)PIN_BUZZER);
      g->drawStr(4, 30, line1);
      g->drawStr(4, 42, line2);
      drawProgmemStr(g, 4, 54, MSG_CTRL);
#else
      static const char MSG_NOBUZZ[]  PROGMEM = "No buzzer pin set (PIN_BUZZER)";
      static const char MSG_BACK[]    PROGMEM = "Back to exit";
      g->drawStr(4, 30, line1);
      drawProgmemStr(g, 4, 42, MSG_NOBUZZ);
      drawProgmemStr(g, 4, 54, MSG_BACK);
#endif
    } while (g->nextPage());
  }
  void handleInput(int input) override {
    if (input == KEY_BACK) { (void)goBack(); return; }
    if (input == KEY_UP)   { if (freq < 4000) freq += (freq >= 2000 ? 200 : 50); }
    else if (input == KEY_DOWN) { if (freq > 100) freq -= (freq > 2000 ? 200 : 50); }
    else if (input == KEY_SELECT) {
#ifdef PIN_BUZZER
      pinMode(PIN_BUZZER, OUTPUT);
      tone(PIN_BUZZER, freq, durMs);
#endif
    }
  }
  void update(void* /*gfx*/) override {}
private:
  uint16_t freq;
  uint16_t durMs;
};

static TestBeepContext testBeepContext;
void registerTestBeepContext() { registerContext("TEST_BEEP", &testBeepContext); }
