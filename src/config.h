#pragma once
// MyClock – Stage 1 (Arduino) configuration
// Board: Waveshare 1.75" ESP32-S3 round AMOLED (CO5300) + CST9217 touch

// ---------------------------
// Panel geometry
// ---------------------------
#define LCD_WIDTH   466
#define LCD_HEIGHT  466

// ---------------------------
// QSPI display (CO5300) pins
// ---------------------------
#define LCD_SDIO0   4
#define LCD_SDIO1   5
#define LCD_SDIO2   6
#define LCD_SDIO3   7
#define LCD_SCLK    38
#define LCD_CS      12
#define LCD_RESET   9   // set to -1 if not wired

// QSPI clock (Hz)
#define LCD_QSPI_HZ 80000000UL

// ---------------------------
// I2C / Touch (CST92xx)
// ---------------------------
#define IIC_SDA     15
#define IIC_SCL     14

// Panel touch interrupt/reset pins
#define TP_INT      11
#define TP_RESET    40

// Touch driver overrides (as used by touch_input.*)
#define TOUCH_I2C_ADDR 0x5A         // change to 0x15 if your scanner reports that address
#define TOUCH_INT_PIN  TP_INT
#define TOUCH_RST_PIN  TP_RESET     // use -1 if no reset wire

// ---------------------------
// Back-compat aliases (safe no-ops if unused elsewhere)
// ---------------------------
#define LCD_W  LCD_WIDTH
#define LCD_H  LCD_HEIGHT
