#pragma once
#include <Arduino.h>

// Simple wrapper around the Waveshare / Arduino_GFX CO5300 driver so the
// main sketch only has to call three functions.

// Initialise the CO5300 display (QSPI bus + Arduino_GFX object).
void gfx_driver_init();

// Register the LVGL display driver for the 466x466 AMOLED panel.
// Must be called after lv_init().
void gfx_lvgl_register();

// Set panel brightness in the range 0.0f (off) .. 1.0f (full).
void gfx_set_backlight(float level);
