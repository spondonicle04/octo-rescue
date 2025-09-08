// step_pots_4067.cpp
#include <Arduino.h>
#include "config.h"
void stepPots_init(){ pinMode(PIN_MUX_EN, OUTPUT); digitalWrite(PIN_MUX_EN, LOW); }
static void muxSel(uint8_t i){
digitalWrite(PIN_MUX_S0, i&1); digitalWrite(PIN_MUX_S1, (i>>1)&1);
digitalWrite(PIN_MUX_S2, (i>>2)&1); digitalWrite(PIN_MUX_S3, (i>>3)&1);
}
uint16_t stepPots_read(uint8_t idx){
muxSel(idx&0x0F);
delayMicroseconds(5); // settle
return analogRead(PIN_MUX_SIG);
}