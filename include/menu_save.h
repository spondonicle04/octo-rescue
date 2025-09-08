// menu_save.h
#ifndef MENU_SAVE_H
#define MENU_SAVE_H

#include "object_classes.h"

class SaveMenuContext : public MenuObject {
public:
  SaveMenuContext();
  void draw(void* gfx) override;
  void update(void* gfx) override;
  void handleInput(int input) override;
};

#endif // MENU_SAVE_H
