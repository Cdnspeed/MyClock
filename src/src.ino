// MyClock – Stage 2 (Arduino): Brightness control page (LVGL moved out of .ino)

#include <Arduino.h>
#define LV_CONF_INCLUDE_SIMPLE
#include <lvgl.h>

#include "config.h"
#include "gfx_driver.h"
#include "pmic_axp2101.h"
#include "touch_input.h"
#include "ui.h"   // <- new UI module

static uint32_t frames = 0;
static uint32_t last_ms = 0;

void setup() {
  Serial.begin(115200);
  delay(200);

  // Power + panel
  gfx_driver_init();

  // LVGL core + display registration
  lv_init();
  gfx_lvgl_register();

  // Touch
  touch_input_init();

  // Build UI (brightness page)
  ui_init();

  last_ms = millis();
}

void loop() {
  // Run LVGL
  lv_timer_handler();

  // LVGL tick (1 ms)
  static uint32_t last_tick = 0;
  uint32_t now = millis();
  lv_tick_inc(now - last_tick);
  last_tick = now;

  // FPS (update twice per second) – no LVGL calls here; delegate to UI
  frames++;
  if (now - last_ms >= 500) {
    float fps = (frames * 1000.0f) / (now - last_ms);
    frames = 0;
    last_ms = now;
    ui_update_fps(fps);
  }

  delay(5);
}
