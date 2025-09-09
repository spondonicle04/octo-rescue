// settings_store.cpp
#include "settings_store.h"
#include <EEPROM.h>
#include "hal_backlight.h"

// Layout: [magic:4][version:1][Settings struct:N][checksum:1]
static const uint32_t MAGIC = 0x4F523031; // 'OR01' (Octo-Rescue v01)
static const uint8_t  VER   = 1;

static Settings g_settings;

static uint8_t simple_checksum(const uint8_t* p, size_t n) {
  uint16_t s = 0;
  for (size_t i = 0; i < n; ++i) s += p[i];
  return (uint8_t)(s & 0xFF);
}

static void set_defaults(Settings& s) {
  s.bl_max_percent = 100;
  s.bl_invert      = 0;
  s.ws_brightness  = 128;
  s.ws_hit_color   = 0xFFFFFF; // white
  s.ws_step_color  = 0x00FF00; // green
}

void settings_init() {
  // Try to read header
  uint32_t magic = 0; uint8_t ver = 0; uint8_t chk = 0;
  size_t addr = 0;
  EEPROM.get(addr, magic); addr += sizeof(magic);
  EEPROM.get(addr, ver);   addr += sizeof(ver);

  Settings tmp;
  if (magic == MAGIC && ver == VER) {
    EEPROM.get(addr, tmp); addr += sizeof(tmp);
    EEPROM.get(addr, chk);
    uint8_t calc = simple_checksum((const uint8_t*)&tmp, sizeof(tmp));
    if (chk == calc) {
      g_settings = tmp;
      settings_apply_runtime();
      return;
    }
  }
  // Fallback to defaults and save
  set_defaults(g_settings);
  settings_save();
  settings_apply_runtime();
}

Settings& settings_get() { return g_settings; }

void settings_save() {
  size_t addr = 0;
  EEPROM.put(addr, MAGIC); addr += sizeof(MAGIC);
  EEPROM.put(addr, VER);   addr += sizeof(VER);
  EEPROM.put(addr, g_settings); addr += sizeof(g_settings);
  uint8_t chk = simple_checksum((const uint8_t*)&g_settings, sizeof(g_settings));
  EEPROM.put(addr, chk);
}

void settings_apply_runtime() {
  // Backlight: apply max percent and invert live
  bl_set_max_percent(settings_get().bl_max_percent);
  bl_set_invert(settings_get().bl_invert != 0);
}

