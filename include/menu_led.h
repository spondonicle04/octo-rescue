#pragma once

#include "object_classes.h"

class LedOptionsContext : public ContextObject {
public:
  LedOptionsContext();
  void draw(void* gfx) override;
  void update(void* gfx) override;
  void handleInput(int input) override;
private:
  uint8_t sel;          // which line
  bool initialized;
  // working copy
  uint8_t brightness;   // 0..255
  uint8_t hitIndex;     // index into preset table
  uint8_t stepIndex;    // index into preset table
};

extern LedOptionsContext ledOptionsContext;

