#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include <avr/pgmspace.h>
#include <string.h>

// Read a char* from a PROGMEM table of pointers.
inline const char* readPtrP(const char* const* tableP, uint8_t index) {
  return (const char*)pgm_read_ptr(&tableP[index]);
}

// ⬇️ THIS LINE IS THE FIX — it MUST start with U8G2* gfx
inline void drawProgmemStr(U8G2* gfx, int x, int y, const char* p) {
  if (!gfx || !p) return;
  char buf[22];
  strncpy_P(buf, p, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';
  gfx->drawStr(x, y, buf);
}

// Centered title with side lines (RAM title).
inline void drawTitleWithLines(U8G2* gfx, const char* title, int y = 12, int gapPx = 6) {
  if (!gfx || !title) return;

  gfx->setFont(u8g2_font_6x13_tf);
  const int W   = gfx->getDisplayWidth();
  const int txt = gfx->getUTF8Width(title);
  int x = (W - txt) / 2; if (x < 0) x = 0;

  gfx->drawStr(x, y, title);

  const int lineY   = y - 5;
  const int leftEnd = x - gapPx;
  const int rightBeg= x + txt + gapPx;

  if (leftEnd > 0)  gfx->drawHLine(0,        lineY, leftEnd);
  if (rightBeg < W) gfx->drawHLine(rightBeg, lineY, W - rightBeg);
}

// Centered title with side lines (PROGMEM title).
inline void drawTitleWithLines_P(U8G2* gfx, const char* titleP, int y = 12, int gapPx = 6) {
  if (!gfx || !titleP) return;

  char buf[24];
  strncpy_P(buf, titleP, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';

  gfx->setFont(u8g2_font_6x13_tf);
  const int W   = gfx->getDisplayWidth();
  const int txt = gfx->getUTF8Width(buf);
  int x = (W - txt) / 2; if (x < 0) x = 0;

  gfx->drawStr(x, y, buf);

  const int lineY   = y - 5;
  const int leftEnd = x - gapPx;
  const int rightBeg= x + txt + gapPx;

  if (leftEnd > 0)  gfx->drawHLine(0,        lineY, leftEnd);
  if (rightBeg < W) gfx->drawHLine(rightBeg, lineY, W - rightBeg);
}

// Paged menu (RAM items).
inline void drawMenuPaged(U8G2* gfx,
                          const char* title,
                          const char* const* items,
                          uint8_t count,
                          uint8_t selected,
                          uint8_t rows = 4)
{
  if (!gfx) return;
  if (rows < 1) rows = 1;

  const uint8_t maxStart = (count > rows) ? (count - rows) : 0;
  uint8_t start = 0;
  if (count > rows) {
    int center = (int)selected - (int)rows / 2;
    if (center < 0) center = 0;
    if ((uint8_t)center > maxStart) center = (int)maxStart;
    start = (uint8_t)center;
  }

  gfx->firstPage();
  do {
    if (title) drawTitleWithLines(gfx, title, 12, 6);

    gfx->setFont(u8g2_font_6x10_tf);
    const int startY = 26;
    const int lineH  = 12;

    for (uint8_t row = 0; row < rows && (start + row) < count; ++row) {
      const uint8_t i = start + row;
      const int y = startY + row * lineH;

      if (i == selected) {
        gfx->drawBox(0, y - 10, 128, 12);
        gfx->setDrawColor(0);
        gfx->drawStr(4, y, items[i]);
        gfx->setDrawColor(1);
      } else {
        gfx->drawStr(4, y, items[i]);
      }
    }

    if (count > rows) {
      const int barX = 125, barTop = 16, barH = 48;
      gfx->drawVLine(barX, barTop, barH);
      int knobH = (rows * barH) / (int)count; if (knobH < 6) knobH = 6;
      const int denom = (int)(maxStart ? maxStart : 1);
      const int numer = (int)start * (barH - knobH);
      const int knobY = barTop + (numer / denom);
      gfx->drawBox(barX - 1, knobY, 3, knobH);
    }
  } while (gfx->nextPage());
}

// Paged menu (PROGMEM items table).
inline void drawMenuPagedP(U8G2* gfx,
                           const char* title,
                           const char* const* itemsP,
                           uint8_t count,
                           uint8_t selected,
                           uint8_t rows = 4)
{
  if (!gfx) return;
  if (rows < 1) rows = 1;

  const uint8_t maxStart = (count > rows) ? (count - rows) : 0;
  uint8_t start = 0;
  if (count > rows) {
    int center = (int)selected - (int)rows / 2;
    if (center < 0) center = 0;
    if ((uint8_t)center > maxStart) center = (int)maxStart;
    start = (uint8_t)center;
  }

  gfx->firstPage();
  do {
    if (title) drawTitleWithLines(gfx, title, 12, 6);

    gfx->setFont(u8g2_font_6x10_tf);
    const int startY = 26;
    const int lineH  = 12;

    for (uint8_t row = 0; row < rows && (start + row) < count; ++row) {
      const uint8_t i = start + row;
      const int y = startY + row * lineH;

      const char* p = readPtrP(itemsP, i);

      if (i == selected) {
        gfx->drawBox(0, y - 10, 128, 12);
        gfx->setDrawColor(0);
        drawProgmemStr(gfx, 4, y, p);
        gfx->setDrawColor(1);
      } else {
        drawProgmemStr(gfx, 4, y, p);
      }
    }

    if (count > rows) {
      const int barX = 125, barTop = 16, barH = 48;
      gfx->drawVLine(barX, barTop, barH);
      int knobH = (rows * barH) / (int)count; if (knobH < 6) knobH = 6;
      const int denom = (int)(maxStart ? maxStart : 1);
      const int numer = (int)start * (barH - knobH);
      const int knobY = barTop + (numer / denom);
      gfx->drawBox(barX - 1, knobY, 3, knobH);
    }
  } while (gfx->nextPage());
}

// Back-compat alias some files might call.
inline void drawMenuPaged_P(U8G2* gfx,
                            const char* title,
                            const char* const* itemsP,
                            uint8_t count,
                            uint8_t selected,
                            uint8_t rows = 4)
{
  drawMenuPagedP(gfx, title, itemsP, count, selected, rows);
}
