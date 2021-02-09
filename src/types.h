#ifndef _TYPES_H_
#define _TYPES_H_
#include <Arduino.h>
/** EeEPROM stored settings **/
typedef struct {
  char ssid[32];
  char pass[32];
  char topic[32];
  char broker[32];
  int sleep;
  char name[100];
  char ap_ssid[32];
} settings_t;

/** sensor data object **/
typedef struct {
  // String dht_t;
  // String dht_h;
  String vbat;
  String temp;
  String pressure;
  String humidity;
} sensorData;

#endif
