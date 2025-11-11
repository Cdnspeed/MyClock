#pragma once
// Simple AXP2101 PMIC helper for the Waveshare ESP32-S3 1.75" board.

#include <Arduino.h>
#include <Wire.h>
#include <XPowersLib.h>
#include "config.h"

// Initialise I2C + AXP2101 and enable display power rails.
// Returns true if the PMIC was detected.
bool pmic_init();

// Explicitly (re)enable the rails used by the panel.
void pmic_enable_display_power();

// Optional brightness hook; for this board we normally use CO5300 DCS
// brightness instead, so this can be a no-op.
void pmic_set_brightness(uint8_t level_0_to_10);
