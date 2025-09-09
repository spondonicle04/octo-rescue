#pragma once

#include "object_classes.h"

class DisplayOptionsContext : public ContextObject {
public:
  DisplayOptionsContext();
  void draw(void* gfx) override;
  void update(void* gfx) override;
  void handleInput(int input) override;
private:
  uint8_t sel;           // which line selected
  bool initialized;      // copied from settings?
  // working copy
  uint8_t maxPct;
  bool invert;
};

extern DisplayOptionsContext displayOptionsContext;

