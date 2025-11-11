#include <Arduino.h>

// Make sure LVGL finds the local config
#define LV_CONF_INCLUDE_SIMPLE
#include <lvgl.h>

#include "config.h"
#include "gfx_driver.h"
#include "pmic_axp2101.h"

#include <Arduino_GFX_Library.h>
#include <esp_heap_caps.h>

// ---------------- QSPI pin defaults (Waveshare 1.75" board) ----------------
#ifndef LCD_SDIO0
#define LCD_SDIO0   4
#endif
#ifndef LCD_SDIO1
#define LCD_SDIO1   5
#endif
#ifndef LCD_SDIO2
#define LCD_SDIO2   6
#endif
#ifndef LCD_SDIO3
#define LCD_SDIO3   7
#endif
#ifndef LCD_SCLK
#define LCD_SCLK   38
#endif
#ifndef LCD_CS
#define LCD_CS     12
#endif
#ifndef LCD_RESET
#define LCD_RESET   9
#endif

#ifndef LCD_WIDTH
#define LCD_WIDTH   466
#endif
#ifndef LCD_HEIGHT
#define LCD_HEIGHT  466
#endif

#ifndef LCD_QSPI_HZ
#define LCD_QSPI_HZ 80000000UL
#endif

// ---------------- Arduino_GFX objects ----------------

Arduino_DataBus *bus = nullptr;
Arduino_GFX     *gfx = nullptr;

// ---------------- LVGL display objects ----------------

static lv_display_t *s_disp = nullptr;
static lv_color_t   *s_draw_buf = nullptr;  // lives in PSRAM

// Flush callback: push LVGL's RGB565 buffer region to the CO5300 panel
static void my_flush_cb(lv_display_t *d, const lv_area_t *area, uint8_t *px_map)
{
    if (!gfx || !area || !px_map) {
        lv_display_flush_ready(d);
        return;
    }

    uint16_t *src = reinterpret_cast<uint16_t *>(px_map);
    uint32_t w = static_cast<uint32_t>(area->x2 - area->x1 + 1);
    uint32_t h = static_cast<uint32_t>(area->y2 - area->y1 + 1);

    gfx->draw16bitRGBBitmap(area->x1, area->y1, src, w, h);
    lv_display_flush_ready(d);
}

// ---------------- Public API ----------------

void gfx_driver_init()
{
    static bool initialised = false;
    if (initialised) return;
    initialised = true;

    // Power up the panel rails via AXP2101
    pmic_init();
    pmic_enable_display_power();

    // QSPI bus + CO5300 as per your known-good sketch
    bus = new Arduino_ESP32QSPI(
        LCD_CS, LCD_SCLK,
        LCD_SDIO0, LCD_SDIO1, LCD_SDIO2, LCD_SDIO3,
        LCD_QSPI_HZ);

    gfx = new Arduino_CO5300(
        bus,
        LCD_RESET,
        0,
        LCD_WIDTH, LCD_HEIGHT,
        6, 0, 0, 0);

    if (!gfx->begin()) {
        while (1) {
            delay(1000);
        }
    }

    // Enable DCS brightness control (0x51 uses this)
    if (bus) {
        bus->beginWrite();
        bus->writeC8D8(0x53, 0x20);   // CTRL Display: BCTRL=1
        bus->endWrite();
    }

    // Allocate full-screen LVGL buffer in PSRAM
    size_t buf_pixels = static_cast<size_t>(LCD_WIDTH) * static_cast<size_t>(LCD_HEIGHT);
    s_draw_buf = static_cast<lv_color_t *>(
        heap_caps_malloc(buf_pixels * sizeof(lv_color_t),
                         MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    if (!s_draw_buf) {
        // Fallback: don't crash, but you'll get no drawing.
        while (1) {
            Serial.println("LVGL buffer PSRAM alloc failed");
            delay(1000);
        }
    }

    // Default to full brightness
    gfx_set_backlight(1.0f);
}

void gfx_lvgl_register()
{
    if (s_disp) return;  // already registered
    if (!s_draw_buf) return;  // init failed

    s_disp = lv_display_create(LCD_WIDTH, LCD_HEIGHT);

    // Full-screen buffer, rendered + flushed in place.
    lv_display_set_flush_cb(s_disp, my_flush_cb);
    lv_display_set_buffers(
        s_disp,
        s_draw_buf,
        nullptr,
        LCD_WIDTH * LCD_HEIGHT * sizeof(lv_color_t),
        LV_DISPLAY_RENDER_MODE_FULL);
}

void gfx_set_backlight(float level)
{
    if (!bus) return;

    if (level < 0.0f) level = 0.0f;
    if (level > 1.0f) level = 1.0f;

    // Map 0.0..1.0 -> panel brightness 0x20..0xFF
    uint8_t dbv = static_cast<uint8_t>(0x20 + level * (0xFF - 0x20) + 0.5f);

    bus->beginWrite();
    bus->writeC8D8(0x51, dbv);   // WRDISBV: brightness value
    bus->endWrite();
}
