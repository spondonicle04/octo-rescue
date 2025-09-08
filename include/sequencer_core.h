// core.h
#pragma once
#include <stdint.h>
struct Track { bool mute=false; uint8_t steps[16]; }; // velocity/gate or on/off
struct Pattern { Track trk[10]; uint8_t length=16; uint8_t pos=0; };
void seq_reset(Pattern& p);
void seq_tick(Pattern& p); // advance pos