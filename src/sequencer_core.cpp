// core.cpp
#include "sequencer_core.h"
void seq_reset(Pattern& p){ p.pos=0; }
void seq_tick(Pattern& p){
p.pos = (p.pos + 1) % p.length;
// In real-time code we’d emit events per unmuted Track here
}