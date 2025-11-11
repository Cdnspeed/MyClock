# MyClock – Progress Update (2025-11-11)

## ✅ What’s done

### Stage 1 – Panel + Touch baseline
- CO5300 round AMOLED brought up via **Arduino_GFX_Library** over **QSPI**.
- LVGL display integration with **partial rendering** using **PSRAM line buffers**.
- Tearing/artifacts addressed:
  - Even-width (RGB565) alignment and strip rounding.
  - Safe rounder handling (builds even if symbol not exported).
- Touch wired (CST9217) per board pinout via `config.h` (no changes to your touch driver needed).

### Stage 2 – Brightness control
- Backlight via panel DCS **0x51** in `gfx_set_backlight(float)`.
- **Settings page** with brightness **bar + ± buttons** (instant updates, no throttling).
- **NVS persistence** (Preferences): value restored on boot, saved on each step.
- LVGL code moved out of the sketch:
  - `src/Clock.ino` = init + LVGL loop only.
  - `src/ui/ui.h`, `src/ui/ui.cpp` = all widgets/pages + NVS.

### Main UI (Home page) per mock
- **Black** background (AMOLED-friendly).
- Layout + sizes (LVGL built-in fonts for now):
  - **Time** `48` centered above date.
  - **Date** `40`, **right edge on vertical center**, **bottom 20 px above center**.
  - **Temp** `46`, **Humidity** `46` (shown as `NN%`, no “RH”), placed **35 px lower**.
  - **Separator** line **400×3** dark grey, **20 px below** horizontal center.
- Small **FPS** label for performance checks.

---

## 📁 Key files & structure

