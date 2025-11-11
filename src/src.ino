// MyClock – Stage 1 (Arduino): Panel + Touch baseline
// Layout: this sketch + gfx_driver.* + pmic_axp2101.* + touch_input.* + config.h
//
// What this does:
//  - Powers up PMIC & panel (gfx_driver_init)
//  - Inits LVGL and registers display (gfx_lvgl_register)
//  - Inits touch (touch_input_init) [your module]
//  - Shows a label and attaches a visible cursor to the touch indev

#include <Arduino.h>

// Ensure LVGL can use a local lv_conf.h if you add one later
#define LV_CONF_INCLUDE_SIMPLE
#include <lvgl.h>

#include "config.h"
#include "gfx_driver.h"
#include "pmic_axp2101.h"
#include "touch_input.h"   // provides touch_input_init()

// ---------- UI ----------
static lv_obj_t *label;
static uint32_t frames = 0;
static uint32_t last_ms = 0;

// Try to find a POINTER input device and attach a visible cursor.
// Returns true once attached.
static bool attach_pointer_cursor_try() {
  // Find the first pointer-type input device registered with LVGL
  lv_indev_t *indev = nullptr;
  for (lv_indev_t *i = nullptr; (i = lv_indev_get_next(i)); ) {
    if (lv_indev_get_type(i) == LV_INDEV_TYPE_POINTER) { indev = i; break; }
  }
  if (!indev) return false;

  // Cursor object: black circle with white border (visible on white backgrounds)
  lv_obj_t *cursor = lv_obj_create(lv_screen_active());
  lv_obj_set_size(cursor, 16, 16);
  lv_obj_set_style_radius(cursor, 8, 0);
  lv_obj_set_style_bg_color(cursor, lv_color_black(), 0);
  lv_obj_set_style_border_width(cursor, 2, 0);
  lv_obj_set_style_border_color(cursor, lv_color_white(), 0);
  lv_obj_add_flag(cursor, LV_OBJ_FLAG_IGNORE_LAYOUT);

  lv_indev_set_cursor(indev, cursor);
  return true;
}

static void ui_init() {
  lv_obj_t *scr = lv_screen_active();

  // Optional: comment this out if you prefer LVGL's default bg
  // lv_obj_set_style_bg_color(scr, lv_color_white(), 0);

  label = lv_label_create(scr);
  lv_label_set_text(label, "MyClock Stage 1\nPanel + Touch");
  lv_obj_center(label);
}

// ---------- Arduino entry points ----------
void setup() {
  Serial.begin(115200);
  delay(200);

  // 1) Power rails + panel (your driver handles AXP2101 + QSPI CO5300 bring-up)
  gfx_driver_init();

  // 2) LVGL core + display registration (your driver provides flush)
  lv_init();
  gfx_lvgl_register();

  // 3) Touch init (your module registers the touch controller with its driver)
  touch_input_init();

  // 4) Minimal UI
  ui_init();

  last_ms = millis();
}

void loop() {
  // Run LVGL
  lv_timer_handler();

  // Provide LVGL ticks (1 ms)
  static uint32_t last_tick = 0;
  uint32_t now = millis();
  lv_tick_inc(now - last_tick);
  last_tick = now;

  // Attach a visible cursor once the touch indev appears
  static bool cursor_attached = false;
  if (!cursor_attached) {
    cursor_attached = attach_pointer_cursor_try();
  }

  // Simple FPS readout on the label
  frames++;
  if (now - last_ms >= 500) {
    float fps = (frames * 1000.0f) / (now - last_ms);
    frames = 0;
    last_ms = now;

    char buf[64];
    snprintf(buf, sizeof(buf), "MyClock Stage 1\n%.1f FPS", fps);
    lv_label_set_text(label, buf);
  }

  delay(5);
}
