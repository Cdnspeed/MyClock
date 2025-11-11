# MyClock ⏰  
*A smart round desktop clock built on the Waveshare 1.75" ESP32-S3 AMOLED*

![Clock Render](docs/images/mockup.png)

## Overview
**MyClock** is an open-source ESP32-S3-based desktop clock that displays live weather, time, and forecasts on a stunning 1.75-inch round AMOLED display.  
It features smooth LVGL 9.3 graphics, auto day/night themes, city selection, Wi-Fi configuration on device, and (later) battery telemetry and alarms.

---

## ✨ Features
- ⚙️ **Hardware:** Waveshare ESP32-S3 Round Display (CO5300 driver, CST9217 touch, onboard audio)
- 🕒 **Clock:** SNTP-synced time with timezone support (America/Toronto)
- ☁️ **Weather:** OpenWeatherMap (current, hourly, daily forecast)
- 🌗 **Themes:** Day/night auto-switch via sunrise/sunset
- 💡 **Brightness:** DCS (0x51) control with slider, stored in NVS
- 📡 **Wi-Fi:** In-app scan → select → password keyboard
- 🗺️ **Location Selector:** Search city or enter coordinates manually
- 🔉 **Audio (later):** Onboard speaker alarms and tones
- 🔋 **Battery (later):** 4.10 V charge, 3.10 V warn, 3.00 V cut, with AXP2101 telemetry

---

## 🧩 Tech Stack
- **Framework:** Arduino-ESP32 (IDF 5.x base)
- **UI:** LVGL 9.3
- **Networking:** Wi-FiClient + HTTPClient
- **Assets:** Remix Icons (Apache 2.0), Inter fonts (OFL)
- **Build:** Arduino IDE / Arduino CLI

---

## 🧱 Repository Layout
