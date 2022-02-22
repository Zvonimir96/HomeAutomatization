#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "Wire.h"
#include "SHT2x.h"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic temperatureCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);

SHT2x sht;
float temperature = 0;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      sht.read();
      temperature = sht.getTemperature();
      char tempString[8];
      dtostrf(temperature, 1, 2, tempString);

      Serial.print("Temperature: ");
      Serial.println(tempString);

      temperatureCharacteristic.setValue(tempString);
      temperatureCharacteristic.notify();
    };

    void onDisconnect(BLEServer* pServer) {
      pServer->getAdvertising()->start();
    }
};

void setup() {
  Serial.begin(9600);
  Serial.println("Starting BLE work!");
  sht.begin();

  BLEDevice::init("Long name works now");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pService->addCharacteristic(&temperatureCharacteristic);
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
}

void loop() {
  delay(10000);
}