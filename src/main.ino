// MyClock – Stage 1 (Arduino): Panel + Touch baseline (Waveshare 1.75" AMOLED)
// Uses your config.h (CO5300 QSPI + CST9217 I2C)

// Core: esp32 by Espressif 3.1.x
// Libs: "GFX Library for Arduino" (Arduino_GFX), optional "SensorLib" for CST9217

#include <Arduino.h>
#include <Wire.h>
#include <Arduino_GFX_Library.h>
#include "config.h"   // <- place your config.h next to this file (src/config.h)

// ---- Resolve geometry from config.h ----
#ifndef LCD_WIDTH
  #define LCD_WIDTH  466
#endif
#ifndef LCD_HEIGHT
  #define LCD_HEIGHT 466
#endif
#define LCD_W  LCD_WIDTH
#define LCD_H  LCD_HEIGHT

// ---- Resolve QSPI pins from config.h ----
#ifndef LCD_SDIO0
  #error "LCD_SDIO0 not defined in config.h"
#endif
#ifndef LCD_SDIO1
  #error "LCD_SDIO1 not defined in config.h"
#endif
#ifndef LCD_SDIO2
  #error "LCD_SDIO2 not defined in config.h"
#endif
#ifndef LCD_SDIO3
  #error "LCD_SDIO3 not defined in config.h"
#endif
#ifndef LCD_SCLK
  #error "LCD_SCLK not defined in config.h"
#endif
#ifndef LCD_CS
  #error "LCD_CS not defined in config.h"
#endif
#ifndef LCD_RESET
  #define LCD_RESET -1
#endif
#ifndef LCD_QSPI_HZ
  #define LCD_QSPI_HZ 80000000UL
#endif

// ---- Resolve I2C pins for touch from config.h ----
#ifndef IIC_SDA
  #define IIC_SDA 15
#endif
#ifndef IIC_SCL
  #define IIC_SCL 14
#endif
#define TP_SDA  IIC_SDA
#define TP_SCL  IIC_SCL
#define TP_INT  -1
#define TP_ADDR 0x15  // common for CST9217

// Optional tearing input (not wired on this board)
#define LCD_TE  -1

// -----------------------------------------------------------------------------
// Arduino_GFX CO5300 / ICNA3311 setup
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    LCD_SDIO0 /* D0 */, LCD_SDIO1 /* D1 */, LCD_SDIO2 /* D2 */, LCD_SDIO3 /* D3 */,
    LCD_SCLK /* SCLK */, LCD_CS /* CS */);

// Prefer CO5300 class if present, otherwise ICNA3311 fallback.
#if __has_include("Arduino_CO5300.h") || defined(ARDUINO_CO5300_CLASS)
  Arduino_GFX *gfx = new Arduino_CO5300(bus, LCD_RESET /* RST */, 0 /* rot */, true /* IPS */, LCD_W, LCD_H);
#else
  Arduino_GFX *gfx = new Arduino_ICNA3311(bus, LCD_RESET /* RST */, 0 /* rot */, true /* IPS */, LCD_W, LCD_H);
#endif

// Optional touch via SensorLib (auto-disabled if not installed)
#if __has_include(<SensorLib.h>)
  #include <SensorLib.h>
  using namespace SensorLib;
  CST9217 touch;
  bool touch_ok = false;
#else
  bool touch_ok = false;
#endif

// -----------------------------------------------------------------------------
// Simple visual/touch validation
static uint32_t frame = 0, last_ms = 0;
static float fps = 0.0f;

static void draw_header() {
  gfx->fillRect(0, 0, LCD_W, 24, RGB565_BLACK);
  gfx->setCursor(8, 18);
  gfx->setTextColor(RGB565_WHITE);
  gfx->setTextSize(1);
  gfx->printf("MyClock Stage 1 | %ux%u | %.1f FPS", (unsigned)LCD_W, (unsigned)LCD_H, fps);
}

static void draw_background() {
  // radial rings to check scanout/center
  const int cx = LCD_W / 2;
  const int cy = LCD_H / 2;
  for (int r = 8; r < min(LCD_W, LCD_H) / 2; r += 10) {
    uint16_t c = (r / 10) % 2 ? RGB565_DARKGREEN : RGB565_DARKBLUE;
    gfx->drawCircle(cx, cy, r, c);
  }
}

static void draw_touch_point(int x, int y) {
  const int rad = 8;
  gfx->fillCircle(x, y, rad, RGB565_YELLOW);
  gfx->drawCircle(x, y, rad + 3, RGB565_RED);
  gfx->drawFastHLine(max(0, x - 20), y, 40, RGB565_RED);
  gfx->drawFastVLine(x, max(0, y - 20), 40, RGB565_RED);
}

static void i2c_scan_once() {
  Serial.println(F("[I2C] scanning..."));
  uint8_t found = 0;
  for (uint8_t addr = 8; addr < 120; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("[I2C] device @ 0x%02X\n", addr);
      found++;
    }
  }
  if (!found) Serial.println(F("[I2C] no devices"));
}

// -----------------------------------------------------------------------------
// Arduino entry points
void setup() {
  Serial.begin(115200);
  delay(200);

  // Display init
  if (!gfx->begin(LCD_QSPI_HZ)) {
    Serial.println(F("[LCD] gfx->begin() failed"));
    while (true) delay(1000);
  }
  if (LCD_TE >= 0) pinMode(LCD_TE, INPUT);

  gfx->fillScreen(RGB565_BLACK);
  gfx->setTextWrap(false);
  draw_background();
  draw_header();

  // I2C / touch init
  Wire.begin(TP_SDA, TP_SCL, 400000);
  delay(50);
  i2c_scan_once();

#if __has_include(<SensorLib.h>)
  if (touch.begin(Wire, TP_ADDR)) {
    touch_ok = true;
    Serial.println(F("[TP] CST9217 init OK"));
  } else {
    Serial.println(F("[TP] CST9217 init FAILED (check pins/address)"));
  }
#endif

  last_ms = millis();
}

void loop() {
  frame++;
  uint32_t now = millis();
  if (now - last_ms >= 500) {
    fps = (frame * 1000.0f) / (now - last_ms);
    frame = 0;
    last_ms = now;
    draw_header();
  }

#if __has_include(<SensorLib.h>)
  if (touch_ok) {
    uint16_t x = 0, y = 0;
    bool pressed = false;
    if (touch.getPoint(&x, &y, &pressed)) {
      if (pressed) draw_touch_point(x, y);
    }
  }
#endif
}
