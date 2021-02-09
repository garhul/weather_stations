#ifndef _UTILS_H_
#define _UTILS_H_

#define EEPROM_SIZE 512
#define SETTINGS_ADDR 0x00
#include <EEPROM.h>
#include <Arduino.h>
#include <types.h>
#include <ESP8266WiFi.h>

#define USE_DHT11
#ifdef USE_DHT11

#include "DHT.h"
#define DHTPIN D3
#define DHTTYPE DHT11

#endif

#define USE_BMP280

#ifdef USE_BMP280
#include <Adafruit_BMP280.h>

#endif

// #define USE_BME280

#ifdef USE_BME280

#include <Adafruit_BME280.h>

#endif

#include <Wire.h>
#include <SPI.h>

#define VBAT_MULTIPLIER 0.0046f

namespace Utils {
  extern settings_t settings;
  void initStorage();
  bool clearStorage();
  bool storeSettings(settings_t settings);
  settings_t getSettings();
  String getInfoJson();
  String getAnnounceInfo();
  String getDeviceId();
  sensorData getSensorData();
}

#endif
