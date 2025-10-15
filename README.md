# 🌤️ WeatherWizard 32-C3

**A low-power BLE weather station using an ESP32-C3 and a Raspberry Pi 5.**

This project measures **temperature**, **humidity**, **pressure**, **UV intensity**, and **rain level** using:
- **BME280** (I²C, temperature/humidity/pressure)
- **GUVA-S12SD** (UV sensor)
- **Rain detection sensor**

The ESP32-C3 sends the readings via **Bluetooth Low Energy (BLE)** to a **Raspberry Pi 5**, which updates **Home Assistant** sensors through its REST API.

---

## 🔧 Hardware Used

| Component | Description | Store |
|------------|-------------|--------|
| ESP32-C3 SuperMini | BLE + WiFi microcontroller | HZWDONE / Huaxingfu |
| BME280 Sensor | Temp / Humidity / Pressure | HNX DIY Electronics |
| GUVA-S12SD | UV intensity sensor | YI-YUE Store |
| Rain Sensor Module | Analog rain detector | Estardyn Choice Store |
| INA219 | Power monitor (optional) | TENSTAR Store |

---
