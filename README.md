# 🌤️ WeatherWizard 32-C3

**A low-power BLE weather station using an ESP32-C3 and a Raspberry Pi 5.**

This project measures **temperature**, **humidity**, **pressure**, **UV intensity**, and **rain level** using:
- **BME280** (I²C, temperature/humidity/pressure)
- **GUVA-S12SD** (UV sensor)
- **Rain detection sensor**

The ESP32-C3 sends the readings via **Bluetooth Low Energy (BLE)** to a **Raspberry Pi 5**, which updates **Home Assistant** sensors through its REST API.

---

## 🔧 Hardware Used

| Component | Description |
|------------|-------------|
| ESP32-C3 SuperMini | BLE + WiFi microcontroller |
| BME280 Sensor | Temp / Humidity / Pressure |
| GUVA-S12SD | UV intensity sensor |
| Rain Sensor Module | Analog rain detector |
| TP4056 Lithium Battery Charger | Battery charger with solar power |
| Solar panel | To give power to device |
| Samsung 18650 3250 mAh battery | The battery |

---
