// // menu_live.h
// #ifndef MENU_LIVE_H
// #define MENU_LIVE_H

// #include "object_classes.h"

// class LiveModeContext : public MenuObject {
// public:
//   LiveModeContext();
//   void draw(void* gfx) override;
//   void update(void* gfx) override;
//   void handleInput(int input) override;
// };

// #endif // MENU_LIVE_H
// menu_live.h
#ifndef MENU_LIVE_H
#define MENU_LIVE_H

#include "object_classes.h"

class LiveModeContext : public ContextObject {
public:
  // You can tune these to match your sequencer
  static const uint8_t COLS = 8;
  static const uint8_t ROWS = 8;

  LiveModeContext();

  void draw(void* gfx) override;          // draws grid
  void handleInput(int input) override;   // reacts to hardware -> mapped inputs
  void update(void* gfx) override;        // advance playhead, blink, etc.

  // Simple grid state (1 = active step)
  bool steps[ROWS][COLS];
  uint8_t cursorCol = 0;
  uint8_t cursorRow = 0;
  uint8_t playhead  = 0;
  bool    playing   = false;

  // Hooks you can wire later
  void toggleStep(uint8_t r, uint8_t c);
  void nextColumn();

private:
  // tiny blink timer
  uint16_t tick = 0;
};

extern LiveModeContext liveModeContext;

#endif // MENU_LIVE_H
