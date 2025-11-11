# Inter Font Conversion Guide (LVGL 9.3)

Use `lv_font_conv` (Node.js) to subset Inter fonts.

```bash
npx lv_font_conv --font ./Inter-SemiBold.ttf --size 130 --bpp 4 --range 0x20-0x7E,0x00B0 --symbols "0123456789: " --format lvgl --lv-include lvgl.h --output ./fonts/INTER_SEMI_130.c
npx lv_font_conv --font ./Inter-Medium.ttf --size 26 --bpp 4 --range 0x20-0x7E,0x00B0 --output ./fonts/INTER_MED_26.c
```
Embed in code:
```cpp
extern const lv_font_t INTER_SEMI_130;
extern const lv_font_t INTER_MED_26;
```
