#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "esp_sleep.h"

// ----- Sensor pins -----
int guvaPin = 0;
int rainPin = 1;
#define BME_SDA 8
#define BME_SCL 9
#define LED_PIN 2

Adafruit_BME280 bme;

// ----- BLE settings -----
#define SERVICE_UUID        "9f5b0001-8b6d-4e2b-9a37-8a6f1e4d9a90"
#define CHARACTERISTIC_UUID "9f5b0002-8b6d-4e2b-9a37-8a6f1e4d9a90"

BLECharacteristic *pCharacteristic;
BLEServer* pServer;

// ---- Measure sensors and send data ----
void sendSensorData() {
  int guvaVal = analogRead(guvaPin);
  float guvaVoltage = guvaVal * (3.3 / 4095.0);

  int rainVal = analogRead(rainPin);
  float rainVoltage = rainVal * (3.3 / 4095.0);

  bme.takeForcedMeasurement();
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  char dataString[128];
  snprintf(dataString, sizeof(dataString),
           "T:%.2f,H:%.2f,P:%.2f,U:%.3f,R:%.3f",
           temperature, humidity, pressure, guvaVoltage, rainVoltage);

  pCharacteristic->setValue(dataString);
  pCharacteristic->notify();
}

// ---- BLE server callback ----
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    sendSensorData(); // Send data when client connects
  }

  void onDisconnect(BLEServer* pServer) {
    // Put the device into deep sleep after disconnect
    Wire.beginTransmission(0x76);
    Wire.write(0xF4);
    Wire.write(0x00);
    Wire.endTransmission();
    digitalWrite(LED_PIN, LOW);
    esp_sleep_enable_timer_wakeup(110 * 1000000ULL);
    esp_deep_sleep_start();
  }
};

void initBME280Loop() {
  if (bme.begin(0x76, &Wire)) {
    bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1,
                    Adafruit_BME280::SAMPLING_X1,
                    Adafruit_BME280::SAMPLING_X1,
                    Adafruit_BME280::FILTER_OFF);
  }
}

void setup() {
  Wire.begin(BME_SDA, BME_SCL);
  initBME280Loop();

  BLEDevice::init("WeatherWizard 32-C3");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

  digitalWrite(LED_PIN, HIGH);

  // Enter deep sleep if no client connects within 10 seconds
  delay(10000);
  if (pServer->getConnectedCount() == 0) {
    digitalWrite(LED_PIN, LOW);
    esp_sleep_enable_timer_wakeup(110 * 1000000ULL);
    esp_deep_sleep_start();
  }
}

void loop() {
  // Nothing to do here, runs once after wake-up
}
