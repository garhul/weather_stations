#ifndef _TYPES_H_
#define _TYPES_H_
#include <Arduino.h>
/** EeEPROM stored settings **/
typedef struct  {
  char ssid[32];
  char pass[32];
  char topic[32];  
  char broker[32];
  int sleep;
  char description[100];
  char ap_ssid[32];  
} settings_t;

/** sensor data object **/
typedef struct {
  String vbat;
  String dht_t;
  String dht_h;
  String bmp280_t;
  String bmp280_p;
} sensorData;

#endif
