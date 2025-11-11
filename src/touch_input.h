#pragma once
#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialize CSTxxx touch and register LVGL input device.
// Safe defaults are used if pins/addr not defined.
void touch_input_init();

#ifdef __cplusplus
}
#endif
