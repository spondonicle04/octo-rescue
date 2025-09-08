// button_matrix.cpp (sketchy stub)
#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "button_matrix.h"

// Using MCP23017: GPA=rows (outputs), GPB=cols (inputs with pullups)
// 6 rows, 6 cols → 36 buttons, with per-key diodes to prevent ghosting.


static inline void mcp_write(uint8_t reg, uint8_t val){ Wire.beginTransmission(I2C_ADDR_MCP);
     Wire.write(reg);
      Wire.write(val);
       Wire.endTransmission(); }
static inline uint8_t mcp_read(uint8_t reg){ Wire.beginTransmission(I2C_ADDR_MCP);
     Wire.write(reg); Wire.endTransmission(false);
      Wire.requestFrom((uint8_t)I2C_ADDR_MCP, (uint8_t)1);
       return Wire.read(); }


void btnmx_init(){
Wire.begin();
// IODIRA=rows outputs (lower 6 used), IODIRB=cols inputs
mcp_write(0x00, 0x00); // IODIRA all outputs
mcp_write(0x01, 0xFF); // IODIRB all inputs
mcp_write(0x0D, 0xFF); // GPPUB pullups on B
}


uint32_t btnmx_read(){
uint32_t bits=0; // we’ll pack 36 bits LSB..MSB
for(uint8_t r=0; r<6; ++r){
    uint8_t rowMask = (1<<r);
        mcp_write(0x12, rowMask); // GPIOA set active row high
        delayMicroseconds(50);
    uint8_t cols = mcp_read(0x13); // GPIOB
        cols = ~cols; // active high when pressed
    for(uint8_t c=0; c<6; ++c){
        uint8_t pressed = (cols>>c)&1;
        uint8_t idx = r*6 + c; // 0..35
        if(pressed) bits |= (1UL<<idx);
    }
}
// Clear rows
mcp_write(0x12, 0x00);
return bits;
}