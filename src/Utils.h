#ifndef _UTILS_H_
#define _UTILS_H_

#define EEPROM_SIZE 512
#define SETTINGS_ADDR 0x00

#include <EEPROM.h>
#include <Arduino.h>
#include <types.h>
#include <ESP8266WiFi.h>
#include <version.h>
#include "DHT.h"
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <SPI.h>
#define DHTPIN 2
#define DHTTYPE DHT11
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
  String getSensorValues();
}

#endif
