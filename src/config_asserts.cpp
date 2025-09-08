// src/config_asserts.cpp
#include <stdint.h>
#include <stddef.h>

#if !defined(__INTELLISENSE__) && defined(__AVR__) && (UINTPTR_MAX == 0xFFFFu)
static_assert(sizeof(void*) == 2, "AVR expected 16-bit pointers");
#endif
