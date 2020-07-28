#ifndef _TYPES_H_
#define _TYPES_H_

/** EeEPROM stored settings **/
typedef struct  {
  char ssid[32];
  char pass[32];
  char topic[32];
  char announce_topic[16];
  char human_name[32];
  char broker[32];
  char ap_ssid[32];
  unsigned int strip_size;
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
