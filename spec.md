# MyClock Project Specification

## Hardware
- Waveshare ESP32-S3 1.75″ round AMOLED (CO5300)
- CST9217 touch controller
- AXP2101 PMIC (battery & charge management)
- Onboard audio amplifier & speaker
- Optional external 1S Li-ion (800 mAh, protected recommended)

## Core Functions
- LVGL 9.3 UI (round layout, Tron-inspired theme)
- SNTP time with TZ=America/Toronto
- OpenWeatherMap weather (current + hourly/daily)
- Wi-Fi setup on-device (scan → password via LVGL keyboard)
- Auto theme via sunrise/sunset
- Manual brightness (DCS 0x51) + NVS persistence
- City selection (search + manual lat/lon)
- Later: battery telemetry + alarms

## Power Policy (deferred stage)
- Charge target 4.10 V @ 400 mA
- Warn 3.10 V, cut 3.00 V
- Protected Li-ion cell recommended

## Fonts & Icons
- Fonts: Inter SemiBold/Medium (OFL)
- Icons: Remix Icons (Apache-2.0)  
  Weather mapping in [`icon-mapping.md`](icon-mapping.md)

## Development
- Arduino-ESP32 (IDF 5.x)
- LVGL 9.3 C++ API
- Dual-core: Core 1 UI / Core 0 Network
- Assets converted via LVGL Image Converter
