#include <Arduino.h>
#define LV_CONF_INCLUDE_SIMPLE
#include <lvgl.h>
#include <Preferences.h>

#include "config.h"
#include "gfx_driver.h"
#include "ui.h"

// --- NVS handle (keeps brightness) ---
static Preferences prefs;

/* ----------------
   Page containers
   ---------------- */
static lv_obj_t *page_home     = nullptr;
static lv_obj_t *page_settings = nullptr;

/* -------------
   Home widgets
   ------------- */
static lv_obj_t *lbl_time = nullptr;     // big time (HH:MM)
static lv_obj_t *lbl_date = nullptr;     // day, month day
static lv_obj_t *lbl_temp = nullptr;     // "22°C"
static lv_obj_t *lbl_hum  = nullptr;     // "45%"
static lv_obj_t *lbl_fps  = nullptr;     // tiny FPS (optional)

/* -----------------
   Settings widgets
   ----------------- */
static lv_obj_t *label_bri = nullptr;
static lv_obj_t *bar_bri   = nullptr;
static lv_obj_t *btn_minus = nullptr;
static lv_obj_t *btn_plus  = nullptr;

/* -----------
   Utilities
   ----------- */
static inline int clamp_pct(int v) { return v < 0 ? 0 : (v > 100 ? 100 : v); }

static void set_brightness_percent(int pct) {
  pct = clamp_pct(pct);
  if (bar_bri)   lv_bar_set_value(bar_bri, pct, LV_ANIM_OFF);
  if (label_bri) lv_label_set_text_fmt(label_bri, "Brightness %d%%", pct);
  gfx_set_backlight(pct / 100.0f); // DCS 0x51 in gfx_driver.cpp
}

/* --------------------
   Navigation helpers
   -------------------- */
static void show_home() {
  if (page_home)     lv_obj_clear_flag(page_home, LV_OBJ_FLAG_HIDDEN);
  if (page_settings) lv_obj_add_flag(page_settings, LV_OBJ_FLAG_HIDDEN);
}
static void show_settings() {
  if (page_settings) lv_obj_clear_flag(page_settings, LV_OBJ_FLAG_HIDDEN);
  if (page_home)     lv_obj_add_flag(page_home, LV_OBJ_FLAG_HIDDEN);
}

/* -------- helpers to position DATE and TIME per spec --------
   DATE:
     - Font size 40
     - Bottom 20 px above the horizontal center line
     - Right edge aligned to the vertical center line (text hugs left of midline)
   TIME:
     - Font size 48
     - Centered horizontally over the DATE (same center X)
     - Bottom 10 px above DATE top
*/
static void position_date_label()
{
  if (!lbl_date) return;
  lv_obj_update_layout(lbl_date);

  const int date_w = (int)lv_obj_get_width(lbl_date);
  const int date_h = (int)lv_obj_get_height(lbl_date);
  const int cx     = LCD_WIDTH  / 2;
  const int cy     = LCD_HEIGHT / 2;

  // right = cx, bottom = cy - 20
  const int x = cx - date_w;
  const int y = (cy - 20) - date_h;
  lv_obj_set_pos(lbl_date, x, y);
}

static void position_time_label()
{
  if (!lbl_time || !lbl_date) return;

  // Ensure both sizes/positions are up to date
  lv_obj_update_layout(lbl_date);
  lv_obj_update_layout(lbl_time);

  const int date_x = (int)lv_obj_get_x(lbl_date);
  const int date_y = (int)lv_obj_get_y(lbl_date);
  const int date_w = (int)lv_obj_get_width(lbl_date);
  const int time_w = (int)lv_obj_get_width(lbl_time);
  const int time_h = (int)lv_obj_get_height(lbl_time);

  // Date center X
  const int date_cx = date_x + (date_w / 2);
  // Time: center X matches date_cx; bottom is 10 px above date top
  const int time_x = date_cx - (time_w / 2);
  const int time_y = (date_y - 10) - time_h;

  lv_obj_set_pos(lbl_time, time_x, time_y);
}

/* ----------------
   Build: HOME page
   ---------------- */
static void build_home(lv_obj_t *root) {
  page_home = lv_obj_create(root);
  lv_obj_remove_style_all(page_home);
  lv_obj_set_size(page_home, lv_pct(100), lv_pct(100));

  // Solid BLACK background
  lv_obj_set_style_bg_opa(page_home, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_color(page_home, lv_color_black(), 0);

  // Time (we'll place precisely after date is placed)
  lbl_time = lv_label_create(page_home);
  lv_label_set_text(lbl_time, "12:34");
  lv_obj_set_style_text_font(lbl_time, &lv_font_montserrat_48, 0);

  // Date (right-align text; font size 40; positioned per spec)
  lbl_date = lv_label_create(page_home);
  lv_label_set_text(lbl_date, "Mon, Nov 10");
  lv_obj_set_style_text_align(lbl_date, LV_TEXT_ALIGN_RIGHT, 0);
  lv_obj_set_style_text_font(lbl_date, &lv_font_montserrat_40, 0);
  position_date_label();
  position_time_label();   // center time above the date

  // --- Separator line: 400x3, centered horizontally, 20 px below center ---
  lv_obj_t *sep = lv_obj_create(page_home);
  lv_obj_remove_style_all(sep);
  lv_obj_set_size(sep, 400, 3);
  lv_obj_set_style_bg_opa(sep, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_color(sep, lv_color_hex(0x303030), 0); // dark grey
  lv_obj_align(sep, LV_ALIGN_CENTER, 0, 20);

  // Temp & Humidity row (move DOWN by 35 px: 70 -> 105) with font size 46
  lv_obj_t *row = lv_obj_create(page_home);
  lv_obj_remove_style_all(row);
  lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);
  lv_obj_set_size(row, lv_pct(90), LV_SIZE_CONTENT);
  lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_column(row, 18, 0);
  lv_obj_align(row, LV_ALIGN_CENTER, 0, 105);

  // Temp label (46)
  lbl_temp = lv_label_create(row);
  lv_obj_set_style_text_font(lbl_temp, &lv_font_montserrat_46, 0);
  lv_label_set_text(lbl_temp, "22°C");

  // Humidity label (46, no "RH")
  lbl_hum = lv_label_create(row);
  lv_obj_set_style_text_font(lbl_hum, &lv_font_montserrat_46, 0);
  lv_label_set_text(lbl_hum, "45%");

  // FPS (small, top-left)
  lbl_fps = lv_label_create(page_home);
  lv_label_set_text(lbl_fps, "— FPS");
  lv_obj_align(lbl_fps, LV_ALIGN_TOP_LEFT, 8, 8);

  // Settings button (bottom)
  lv_obj_t *btn_settings = lv_btn_create(page_home);
  lv_obj_set_size(btn_settings, 120, 42);
  lv_obj_align(btn_settings, LV_ALIGN_BOTTOM_MID, 0, -18);
  lv_obj_t *lbl = lv_label_create(btn_settings);
  lv_label_set_text(lbl, "Settings");
  lv_obj_add_event_cb(btn_settings, [](lv_event_t *e){
    (void)e; show_settings();
  }, LV_EVENT_CLICKED, nullptr);
}

/* --------------------
   Build: SETTINGS page
   -------------------- */
static void build_settings(lv_obj_t *root) {
  page_settings = lv_obj_create(root);
  lv_obj_remove_style_all(page_settings);
  lv_obj_set_size(page_settings, lv_pct(100), lv_pct(100));
  lv_obj_set_style_bg_opa(page_settings, LV_OPA_COVER, 0);

  // Back button (top-left)
  lv_obj_t *btn_back = lv_btn_create(page_settings);
  lv_obj_set_size(btn_back, 80, 36);
  lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 12, 12);
  lv_obj_t *lbl_b = lv_label_create(btn_back);
  lv_label_set_text(lbl_b, LV_SYMBOL_LEFT " Back");
  lv_obj_add_event_cb(btn_back, [](lv_event_t *e){
    (void)e; show_home();
  }, LV_EVENT_CLICKED, nullptr);

  // Container under center (brightness controls)
  lv_obj_t *cont = lv_obj_create(page_settings);
  lv_obj_set_size(cont, lv_pct(90), LV_SIZE_CONTENT);
  lv_obj_align(cont, LV_ALIGN_CENTER, 0, 100);
  lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(cont, 0, 0);
  lv_obj_set_style_pad_all(cont, 0, 0);

  // Label
  label_bri = lv_label_create(cont);
  lv_label_set_text(label_bri, "Brightness 80%");
  lv_obj_align(label_bri, LV_ALIGN_TOP_MID, 0, 0);

  // Row – [BAR] +
  lv_obj_t *row = lv_obj_create(cont);
  lv_obj_remove_style_all(row);
  lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);
  lv_obj_set_size(row, lv_pct(100), LV_SIZE_CONTENT);
  lv_obj_align(row, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_column(row, 12, 0);

  // -
  btn_minus = lv_btn_create(row);
  lv_obj_set_size(btn_minus, 40, 40);
  lv_obj_t *lbl_m = lv_label_create(btn_minus);
  lv_label_set_text(lbl_m, LV_SYMBOL_MINUS);

  // bar
  bar_bri = lv_bar_create(row);
  lv_obj_set_size(bar_bri, lv_pct(60), 16);
  lv_bar_set_range(bar_bri, 0, 100);

  // +
  btn_plus = lv_btn_create(row);
  lv_obj_set_size(btn_plus, 40, 40);
  lv_obj_t *lbl_p = lv_label_create(btn_plus);
  lv_label_set_text(lbl_p, LV_SYMBOL_PLUS);

  // Events (step 5%, persist)
  lv_obj_add_event_cb(btn_minus, [](lv_event_t *e){
    (void)e;
    int v = clamp_pct((int)lv_bar_get_value(bar_bri) - 5);
    set_brightness_percent(v);
    prefs.putUChar("bri", (uint8_t)v);
  }, LV_EVENT_CLICKED, nullptr);

  lv_obj_add_event_cb(btn_plus, [](lv_event_t *e){
    (void)e;
    int v = clamp_pct((int)lv_bar_get_value(bar_bri) + 5);
    set_brightness_percent(v);
    prefs.putUChar("bri", (uint8_t)v);
  }, LV_EVENT_CLICKED, nullptr);

  // Hide by default; Home is first
  lv_obj_add_flag(page_settings, LV_OBJ_FLAG_HIDDEN);
}

/* -------------
   Public API
   ------------- */
void ui_init() {
  // Open NVS (RW)
  prefs.begin("myclock", false);

  lv_obj_t *root = lv_screen_active();

  // Build both pages
  build_home(root);
  build_settings(root);

  // Restore brightness (default 80%)
  uint8_t saved = prefs.getUChar("bri", 80);
  set_brightness_percent(saved);

  // Start at Home
  show_home();

  // Seed placeholders
  ui_set_time_text("12:34");
  ui_set_date_text("Mon, Nov 10");
  ui_set_temp_c(22.0f);
  ui_set_humidity_pct(45);
}

void ui_update_fps(float fps) {
  if (lbl_fps) {
    lv_label_set_text_fmt(lbl_fps, "%.1f FPS", fps);
  }
}

/* ---------------------
   Data update “stubs”
   --------------------- */
void ui_set_time_text(const char *hhmm) {
  if (!lbl_time || !hhmm) return;
  lv_label_set_text(lbl_time, hhmm);
  position_time_label();  // keep centered over date if width changes
}
void ui_set_date_text(const char *datestr) {
  if (!lbl_date || !datestr) return;
  lv_label_set_text(lbl_date, datestr);
  position_date_label();
  position_time_label();
}
void ui_set_temp_c(float celsius) {
  if (!lbl_temp) return;
  float v = roundf(celsius * 2.0f) / 2.0f;
  lv_label_set_text_fmt(lbl_temp, "%.1f°C", v);
}
void ui_set_humidity_pct(uint8_t pct) {
  if (!lbl_hum) return;
  if (pct > 100) pct = 100;
  lv_label_set_text_fmt(lbl_hum, "%u%%", (unsigned)pct); // no "RH"
}
