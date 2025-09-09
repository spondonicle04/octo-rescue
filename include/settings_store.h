#pragma once

#include <Arduino.h>

struct Settings {
  uint8_t bl_max_percent;    // 0..100
  uint8_t bl_invert;         // 0/1
  uint8_t ws_brightness;     // 0..255
  uint32_t ws_hit_color;     // 0xRRGGBB
  uint32_t ws_step_color;    // 0xRRGGBB
};

// Initialize settings (load from EEPROM or create defaults)
void settings_init();

// Access current settings (live copy)
Settings& settings_get();

// Save current settings to EEPROM
void settings_save();

// Apply settings to subsystems that care (e.g., backlight)
void settings_apply_runtime();

