#pragma once
// --- Core Buses ---

#ifndef PIN_SPI_MOSI
#define PIN_SPI_MOSI 51
#endif
#ifndef PIN_SPI_MISO
#define PIN_SPI_MISO 50
#endif
#ifndef PIN_SPI_SCK
#define PIN_SPI_SCK 52
#endif
#ifndef PIN_SD_CS
#define PIN_SD_CS 53
#endif
#define PIN_I2C_SDA 20
#define PIN_I2C_SCL 21


// --- Display ---
#define LCD_CLK 13
#define LCD_MOSI 11
#define LCD_CS 10


// --- Backlight & Pot ---
#define PIN_BL_PWM 12
#define PIN_BRIGHT_POT A0


// --- Encoder & Clock ---
#define PIN_ENC_A 47
#define PIN_ENC_B 48
#define PIN_ENC_BTN 49
#define PIN_EXT_CLOCK 2


// --- LEDs ---
#define PIN_WS2812 6


// --- DACs (chip-selects) ---
#define PIN_DAC_CS1 9
#define PIN_DAC_CS2 8
#define PIN_DAC_CS3 7
#define PIN_DAC_CS4 5
#define PIN_DAC_CS5 4
#define PIN_DAC_CS6 3


// --- Analog Mux (CD4067) ---
#define PIN_MUX_SIG A15
#define PIN_MUX_S0 22
#define PIN_MUX_S1 23
#define PIN_MUX_S2 24
#define PIN_MUX_S3 25
#define PIN_MUX_EN 26


// --- Direct Instrument Pots ---
#define PIN_INST_POT_1 A1
#define PIN_INST_POT_2 A2
#define PIN_INST_POT_3 A3
#define PIN_INST_POT_4 A4
#define PIN_INST_POT_5 A5
#define PIN_INST_POT_6 A6
#define PIN_INST_POT_7 A7
#define PIN_INST_POT_8 A8
#define PIN_INST_POT_9 A9
#define PIN_INST_POT_10 A10


// --- MCP23017 for Button Matrix ---
#define I2C_ADDR_MCP 0x20

// --- Temp buttons for setup/test
 #define BTN_LEFT    40
 #define BTN_RIGHT   41
 #define BTN_LIVE    42
 #define BTN_DOWN    43
 #define BTN_UP      44
 #define BTN_SELECT  45
 #define BTN_BACK    46