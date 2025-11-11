/**
 * Minimal local LVGL config for ESP32-S3 + 240x240 SPI panel.
 * Target: LVGL 9.x
 *
 * Place this file in the same folder as your .ino/.cpp files.
 * Then BEFORE including <lvgl.h> anywhere in a file, make sure this file
 * is included via:
 *   #define LV_CONF_INCLUDE_SIMPLE
 *   #include "lv_conf.h"
 *
 * Arduino IDE will then use these settings instead of the default library config.
 */

#ifndef LV_CONF_H
#define LV_CONF_H

/* -----------------------------
   Basic display settings
   ----------------------------- */
#define LV_COLOR_DEPTH              16      /* RGB565 for Arduino_GFX */
#define LV_COLOR_16_SWAP            0
#define LV_COLOR_SCREEN_TRANSP      0

/* Try ~60Hz refresh target */
#define LV_DISPLAY_DEF_REFR_PERIOD  16      /* ms */

/* -----------------------------
   Memory / performance
   ----------------------------- */

/* Internal logging off to save flash */
#define LV_USE_LOG                  0

/* We'll feed LVGL our own 2x DMA buffers in gfx_driver.cpp. */
#define LV_DRAW_BUF_STRIDE_ALIGN    4
#define LV_DRAW_BUF_SIZE            (240*40) /* match gfx_driver.cpp (40 lines high) */

/* -----------------------------
   Tick / timing
   ----------------------------- */
#define LV_USE_OS                   LV_OS_NONE

/* -----------------------------
   Drawing engines
   ----------------------------- */
#define LV_USE_DRAW_SW              1
    #define LV_DRAW_SW_COMPLEX      1
    #define LV_USE_DRAW_SW_SHADOW_CACHE 0
    #define LV_USE_DRAW_SW_LAYERED  0

#define LV_USE_DRAW_ARM2D           0
#define LV_USE_DRAW_NXP_PXP         0
#define LV_USE_DRAW_SDL             0

/* -----------------------------
   Widgets
   ----------------------------- */
#define LV_USE_OBJ                  1
#define LV_USE_LABEL                1
#define LV_USE_ARC                  1
#define LV_USE_BAR                  1
#define LV_USE_LINE                 1
#define LV_USE_IMG                  1
#define LV_USE_RECT                 1

/* Cut everything we don't need (saves flash/RAM) */
#define LV_USE_SWITCH               0
#define LV_USE_SLIDER               0
#define LV_USE_BTN                  0
#define LV_USE_DROPDOWN             0
#define LV_USE_ROLLER               0
#define LV_USE_CHART                0
#define LV_USE_TABLE                0
#define LV_USE_KEYBOARD             0
#define LV_USE_TEXTAREA             0
#define LV_USE_MSGBOX               0
#define LV_USE_SPINNER              0
#define LV_USE_SPINBOX              0
#define LV_USE_LIST                 0
#define LV_USE_MENU                 0
#define LV_USE_LED                  0
#define LV_USE_CHECKBOX             0
#define LV_USE_TILEVIEW             0
#define LV_USE_TABVIEW              0
#define LV_USE_WIN                  0
#define LV_USE_METER                0
#define LV_USE_IMG_BUTTON           0
#define LV_USE_ANIMIMG              0
#define LV_USE_CALENDAR             0
#define LV_USE_COLORWHEEL           0

/* -----------------------------
   Themes / styles
   ----------------------------- */
#define LV_USE_THEME_DEFAULT        1
    #define LV_THEME_DEFAULT_PALETTE_LIGHT   0   /* dark UI */
    #define LV_THEME_DEFAULT_PALETTE_DARK    1
    #define LV_THEME_DEFAULT_FONT_SMALL      &lv_font_montserrat_14
    #define LV_THEME_DEFAULT_FONT_NORMAL     &lv_font_montserrat_16
    #define LV_THEME_DEFAULT_FONT_LARGE      &lv_font_montserrat_28

/* Built-in fonts we actually want linked (saves flash) */
#define LV_FONT_MONTSERRAT_14       1
#define LV_FONT_MONTSERRAT_16       1
#define LV_FONT_MONTSERRAT_28       1

/* Disable all other Montserrat sizes */
#define LV_FONT_MONTSERRAT_8        0
#define LV_FONT_MONTSERRAT_10       0
#define LV_FONT_MONTSERRAT_12       1
#define LV_FONT_MONTSERRAT_18       0
#define LV_FONT_MONTSERRAT_20       0
#define LV_FONT_MONTSERRAT_22       0
#define LV_FONT_MONTSERRAT_24       0
#define LV_FONT_MONTSERRAT_26       0
#define LV_FONT_MONTSERRAT_30       0
#define LV_FONT_MONTSERRAT_32       0
#define LV_FONT_MONTSERRAT_34       0
#define LV_FONT_MONTSERRAT_36       0
#define LV_FONT_MONTSERRAT_38       0
#define LV_FONT_MONTSERRAT_40       1
#define LV_FONT_MONTSERRAT_42       0
#define LV_FONT_MONTSERRAT_44       0
#define LV_FONT_MONTSERRAT_46       1
#define LV_FONT_MONTSERRAT_48       1

#define LV_USE_BIDI                 0
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/* -----------------------------
   Debug / perf
   ----------------------------- */
#define LV_USE_DEBUG                0
#define LV_USE_ASSERT_NULL          0
#define LV_USE_ASSERT_MALLOC        0
#define LV_USE_ASSERT_STYLE         0

#define LV_USE_PERF_MONITOR         0
#define LV_USE_MEM_MONITOR          0

#endif /* LV_CONF_H */
