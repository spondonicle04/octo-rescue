// menu_debug.h
#ifndef MENU_DEBUG_H
#define MENU_DEBUG_H

#include "object_classes.h"

class DebugMenuContext : public MenuObject {
public:
  DebugMenuContext();
  void draw(void* gfx) override;
  void update(void* gfx) override;
  void handleInput(int input) override;
};

#endif // MENU_DEBUG_H
