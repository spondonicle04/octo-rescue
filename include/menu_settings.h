// menu_settings.h
#ifndef MENU_SETTINGS_H
#define MENU_SETTINGS_H

#include "object_classes.h"

class SettingsMenuContext : public MenuObject {
public:
  SettingsMenuContext();
  void draw(void* gfx) override;
  void update(void* gfx) override;
  void handleInput(int input) override;
};

extern SettingsMenuContext settingsMenuContext;

#endif // MENU_SETTINGS_H
