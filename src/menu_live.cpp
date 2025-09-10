

// menu_live.cpp NEW
// LIVE (Grid) context — primary performance screen

#include "menu_live.h"
#include "context_registry.h"
#include "context_state.h"
#include "input_codes.h"
#include <U8g2lib.h>
#include <avr/pgmspace.h>

LiveModeContext::LiveModeContext()
  : ContextObject("LIVE_MODE", "MAIN_MENU", /*subs*/ nullptr, /*count*/ 0) {
  // clear grid
  for (uint8_t r = 0; r < ROWS; ++r)
    for (uint8_t c = 0; c < COLS; ++c)
      steps[r][c] = false;
}

void LiveModeContext::draw(void* gfx) {
  U8G2* gfxU8 = static_cast<U8G2*>(gfx);
  if (!gfxU8) return;

  // Simple proof-of-life: title + 8×8 grid
  const uint8_t cell = 5;      // cell size in pixels
  const uint8_t pad  = 1;      // spacing between cells
  const uint8_t offX = 10;      // top-left X
  const uint8_t offY = 2;     // top-left Y

  gfxU8->firstPage();
  do {
    
    for (uint8_t r = 0; r < ROWS; ++r) {
      for (uint8_t c = 0; c < COLS; ++c) {
        uint8_t x = offX + c * (cell + pad);
        uint8_t y = offY + r * (cell + pad);
        bool on = steps[r][c];
        bool isCursor   = (r == cursorRow && c == cursorCol);
        bool isPlayhead = (c == playhead);

        if (on) gfxU8->drawBox(x, y, cell, cell);
        else    gfxU8->drawFrame(x, y, cell, cell);

        if (isCursor)   gfxU8->drawFrame(x-1, y-1, cell+2, cell+2);
        if (isPlayhead) gfxU8->drawHLine(x, y+cell+1, cell);
      }
    }
  } while (gfxU8->nextPage());
}

void LiveModeContext::toggleStep(uint8_t r, uint8_t c) {
  if (r < ROWS && c < COLS) steps[r][c] = !steps[r][c];
}

void LiveModeContext::nextColumn() {
  playhead = (uint8_t)((playhead + 1) % COLS);
}

void LiveModeContext::update(void* /*gfx*/) {
  // super basic “clock” just to show playhead moving without timers
  if (playing) {
    if (++tick > 3000) {  // tune this or drive via real clock/ISR
      tick = 0;
      nextColumn();
    }
  }
}

void LiveModeContext::handleInput(int input) {
  switch (input) {
    case IN_MENU:
      setContextByName_P(PSTR("MAIN_MENU"));
      break;

    case IN_UP:
      if (cursorRow > 0) cursorRow--;
      break;
    case IN_DOWN:
      if (cursorRow + 1 < ROWS) cursorRow++;
      break;
    case IN_LEFT:
      if (cursorCol > 0) cursorCol--;
      break;
    case IN_RIGHT:
      if (cursorCol + 1 < COLS) cursorCol++;
      break;

    case IN_TOGGLE:
      toggleStep(cursorRow, cursorCol);
      break;

    case IN_PLAY:
      playing = true;
      break;
    case IN_STOP:
      playing = false;
      break;

    default:
      // ignore
      break;
    case IN_SELECT:
      setContextByName_P(PSTR("MAIN_MENU"));
      break;
  }
}
LiveModeContext liveModeContext;

void registerLiveModeContext() {
  registerContext("LIVE_MODE", &liveModeContext);
}
