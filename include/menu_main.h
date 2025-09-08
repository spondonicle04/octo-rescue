// menu_main.h
// Declaration for the MAIN_MENU context

#ifndef MENU_MAIN_H
#define MENU_MAIN_H

#include "object_classes.h"

class MainMenuContext : public MenuObject {
public:
  MainMenuContext();
  void draw(void* gfx) override;
  void update(void* gfx) override;
  void handleInput(int input) override;
      
};

extern MainMenuContext mainMenuContext;

#endif // MENU_MAIN_H
