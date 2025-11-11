#pragma once

// ================== DISPLAY GEOMETRY ==================
#define TFT_WIDTH    240
#define TFT_HEIGHT   240

// Rotate 180 degrees from the original orientation
#define TFT_ROTATION 0

// GC9A01A GRAM offsets
#define TFT_X_OFFSET 0
#define TFT_Y_OFFSET 0

// ================== DISPLAY PINS (GC9A01A SPI) ==================
#define PIN_LCD_SCLK   10    // GPIO10
#define PIN_LCD_MOSI   11    // GPIO11
#define PIN_LCD_CS      9    // GPIO9
#define PIN_LCD_DC      8    // GPIO8
#define PIN_LCD_RST    12    // GPIO12
#define PIN_LCD_BL     40    // GPIO40 (backlight, active HIGH)


// ================== LVGL TICK ==================
#define LVGL_TICK_MS           5


// Native panel size
#define LCD_WIDTH   466
#define LCD_HEIGHT  466

#define TFT_WIDTH   LCD_WIDTH
#define TFT_HEIGHT  LCD_HEIGHT

// QSPI data bus pins
#ifndef LCD_SDIO0
#define LCD_SDIO0   4
#define LCD_SDIO1   5
#define LCD_SDIO2   6
#define LCD_SDIO3   7
#define LCD_SCLK   38
#define LCD_CS     12
#define LCD_RESET   9
#endif

// QSPI frequency
#ifndef LCD_QSPI_HZ
#define LCD_QSPI_HZ 80000000UL
#endif

// I2C pins for PMIC/touch
#ifndef IIC_SDA
#define IIC_SDA 15
#endif
#ifndef IIC_SCL
#define IIC_SCL 14
#endif

// Ensure LVGL tick is defined
#ifndef LVGL_TICK_MS
#define LVGL_TICK_MS 5
#endif
