#pragma once
#include <stdint.h>

// Build all UI pages (call after lv_init() + gfx_lvgl_register() + touch_input_init()).
void ui_init();

// Update the small FPS readout on the Home page (optional)
void ui_update_fps(float fps);

// ---- Data update stubs (call these later when you wire RTC/NTP & weather) ----
void ui_set_time_text(const char *hhmm);        // e.g. "12:34"
void ui_set_date_text(const char *datestr);     // e.g. "Mon, Nov 10"
void ui_set_temp_c(float celsius);              // e.g. 21.5
void ui_set_humidity_pct(uint8_t pct);          // e.g. 45
