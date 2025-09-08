#pragma once
#include <stdint.h>
#include <limits.h>



static_assert(sizeof(uint8_t) == 1, "uint8_t must be 8-bit");
static_assert(UINT8_MAX == 255,     "uint8_t must be 0..255");

/* Bounds you can override before including this file */
#ifndef MAX_MENU_ITEMS
  #define MAX_MENU_ITEMS 32
#endif
#ifndef MAX_SUBCONTEXTS
  #define MAX_SUBCONTEXTS 16
#endif
#ifndef EVENT_QUEUE_LEN
  #define EVENT_QUEUE_LEN 16
#endif
#ifndef CTX_HISTORY_LEN
  #define CTX_HISTORY_LEN 8
#endif

static_assert(MAX_MENU_ITEMS  <= 255, "Menu items must fit in uint8_t");
static_assert(MAX_SUBCONTEXTS <= 255, "Subcontexts must fit in uint8_t");
static_assert(EVENT_QUEUE_LEN <= 255, "Event queue must fit in uint8_t");
static_assert(CTX_HISTORY_LEN <= 255, "History must fit in uint8_t");
