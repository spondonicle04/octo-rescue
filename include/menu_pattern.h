#pragma once
#include "object_classes.h"

class PatternMenuContext : public MenuObject {
public:
  PatternMenuContext();
  void draw(void* gfx) override;
  void update(void* gfx) override;
  void handleInput(int input) override;
};

extern PatternMenuContext patternMenuContext;
