// menu_boot.h
// Declaration for the BOOT context.

#ifndef MENU_BOOT_H
#define MENU_BOOT_H

#pragma once
#include "object_classes.h"   // has ContextObject

// Subclass of ContextObject with custom behavior
class BootContext : public ContextObject {
public:
  BootContext();
  ~BootContext() override = default;      // inline dtor = fine

  void draw(void* gfx) override;          // must match ContextObject
  void handleInput(int input) override;   // must match ContextObject
};

extern BootContext bootContext;           // declaration only (no init here)


#endif // MENU_BOOT_H
