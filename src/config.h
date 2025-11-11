#pragma once
// MyClock – Stage 1 (Arduino) configuration
// Hardware: Waveshare 1.75" ESP32-S3 round AMOLED (CO5300) + CST9217 touch

// ---------------------------
// Panel geometry
// ---------------------------
#define LCD_WIDTH   466
#define LCD_HEIGHT  466

// ---------------------------
// QSPI display (CO5300) pins
// ---------------------------
// Data lines
#define LCD_SDIO0   4
#define LCD_SDIO1   5
#define LCD_SDIO2   6
#define LCD_SDIO3   7
// Clock & chip-select
#define LCD_SCLK    38
#define LCD_CS      12
// Reset (set to -1 if not wired)
#define LCD_RESET   9
// Optional tearing input (not used in Stage 1); keep -1 if not wired
// #define LCD_TE   -1

// QSPI clock (Hz). Many boards run 80 MHz reliably.
#define LCD_QSPI_HZ 80000000UL

// ---------------------------
// I2C touch (CST9217) pins
// ---------------------------
#define IIC_SDA     15
#define IIC_SCL     14

// (Touch INT pin not required; driver polls over I2C)
// #define TP_INT   -1

// ---------------------------
// End of config
// ---------------------------
