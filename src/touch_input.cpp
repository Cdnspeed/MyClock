#include <Arduino.h>
#include <Wire.h>
#include <lvgl.h>
#include "TouchDrvCSTXXX.hpp"
#include "config.h"
#include "touch_input.h"

// Touch driver instance
static TouchDrvCSTXXX touch;
static int16_t tp_x[5], tp_y[5];

#if LVGL_VERSION_MAJOR >= 9
// LVGL9+ input dev
static void lv_touch_read_cb(lv_indev_t * indev, lv_indev_data_t * data) {
  (void)indev;
  uint8_t n = touch.getPoint(tp_x, tp_y, touch.getSupportTouchPoint());
  if (n > 0) {
    data->state   = LV_INDEV_STATE_PRESSED;
    data->point.x = tp_x[0];
    data->point.y = tp_y[0];
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}
#else
// LVGL8 and earlier driver-style callback
static void lv_touch_read_cb(lv_indev_drv_t * drv, lv_indev_data_t * data) {
  (void)drv;
  uint8_t n = touch.getPoint(tp_x, tp_y, touch.getSupportTouchPoint());
  if (n > 0) {
    data->state   = LV_INDEV_STATE_PR;
    data->point.x = tp_x[0];
    data->point.y = tp_y[0];
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}
#endif

void touch_input_init() {
  // I2C for touch
  Wire.begin(IIC_SDA, IIC_SCL);

  // Touch IC address and pins
  const uint8_t addr = 0x5A;         // CSTxxx typical
  touch.setPins(TP_RESET, TP_INT);   // RST, INT
  (void)touch.begin(Wire, addr, IIC_SDA, IIC_SCL);

  // Match panel resolution / orientation
  touch.setMaxCoordinates(LCD_WIDTH, LCD_HEIGHT);
  touch.setMirrorXY(true, true);

#if LVGL_VERSION_MAJOR >= 9
  // LVGL v9+ input device API
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, lv_touch_read_cb);
#else
  // LVGL v8 input device driver API
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = lv_touch_read_cb;
  lv_indev_drv_register(&indev_drv);
#endif
}
