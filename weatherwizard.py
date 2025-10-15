import asyncio
from bleak import BleakScanner, BleakClient
from datetime import datetime
import requests

# --- Home Assistant configuration ---
HA_TOKEN = "YOUR_HOME_ASSISTANT_TOKEN"
HEADERS = {
    "Authorization": f"Bearer {HA_TOKEN}",
    "Content-Type": "application/json",
}
HA_SERVICE_URL = "http://homeassistant.local:8123/api/services/input_number/set_value" # Your HomeAssistant's address

# --- BLE constants ---
ESP_NAME = "WeatherWizard 32-C3"
CHAR_UUID = "9f5b0002-8b6d-4e2b-9a37-8a6f1e4d9a90"

# --- entity_id mapping ---
HA_ENTITIES = {
    "temperature": "input_number.weatherwizard_temperature",
    "humidity":    "input_number.weatherwizard_humidity",
    "pressure":    "input_number.weatherwizard_pressure",
    "uv":          "input_number.weatherwizard_uv",
    "rain":        "input_number.weatherwizard_rain"
}

async def scan_and_send():
    while True:
        devices = await BleakScanner.discover(timeout=1)
        found = next((d for d in devices if d.name == ESP_NAME), None)

        if found:
            try:
                async with BleakClient(found.address) as client:
                    value = await client.read_gatt_char(CHAR_UUID)
                    data_str = value.decode()
                    print(f"📡 Received data: {data_str}")

                    parts = data_str.split(",")
                    temperature = float(parts[0].split(":")[1])
                    humidity = float(parts[1].split(":")[1])
                    pressure = float(parts[2].split(":")[1])
                    uv = float(parts[3].split(":")[1])
                    rain = float(parts[4].split(":")[1])

                    for key, value in [
                        ("temperature", temperature),
                        ("humidity", humidity),
                        ("pressure", pressure),
                        ("uv", uv),
                        ("rain", rain)
                    ]:
                        payload = {
                            "entity_id": HA_ENTITIES[key],
                            "value": value
                        }
                        response = requests.post(HA_SERVICE_URL, json=payload, headers=HEADERS)
                        if response.ok:
                            print(f"✅ Updated {key} = {value}")
                        else:
                            print(f"❌ Failed to update {key}: {response.status_code}")

                await asyncio.sleep(110)
            except Exception as e:
                print(f"⚠ Error while reading BLE data: {e}")
                pass
        else:
            print("🔍 No WeatherWizard device found.")
            pass

if __name__ == "__main__":
    asyncio.run(scan_and_send())
