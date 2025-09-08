#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "config.h"
#include <Arduino.h>
#include "config.h"
#include "debug.h"

void i2c_scan(){
DL("I2C scan:");
Wire.begin();
for (uint8_t addr=1; addr<127; ++addr){
Wire.beginTransmission(addr);
uint8_t err = Wire.endTransmission();
if(err==0){ DL(" - 0x"); DPRINTLN(addr, HEX); }
}
}


bool sd_init(){
DL("SD init CS="); DPRINTLN(PIN_SD_CS);
if(SD.begin(PIN_SD_CS)){
DL("SD: OK");
File root = SD.open("/");
while(true){
File f = root.openNextFile();
if(!f) break;
DL(" "); DPRINT(f.name());
if(f.isDirectory()) DL("/"); else DL("");
f.close();
}
root.close();
return true;
}
DL("SD: FAIL");
return false;
}


void run_boot_checks(){ i2c_scan(); sd_init(); }