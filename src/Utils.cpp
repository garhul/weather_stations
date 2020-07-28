
#include <Utils.h>

namespace Utils {
  settings_t settings = {};

  void initStorage(){
    EEPROM.begin(EEPROM_SIZE);
    getSettings();
  }

  bool clearStorage() {   
    for (int i = 0; i < EEPROM_SIZE; ++i) {
      EEPROM.write(i, 1);
    }

    return EEPROM.commit();
  }

  bool storeSettings(settings_t settings) {
    EEPROM.put(SETTINGS_ADDR, settings);
    return EEPROM.commit();
  }

  settings_t getSettings() {    
    EEPROM.get(0x00, settings);
    return settings;
  }
  
  String getDeviceId() {
    return String("Aurora_") + String(ESP.getChipId(), HEX);
  }

  String getAnnounceInfo() {
    return getDeviceId() + "|" + WiFi.localIP().toString() + "|" + String(settings.human_name);
  }

  String getSensorValues(sensorData data) {
     String info = "{\"vbat\":\"" + data.vbat + 
    "\",\"dht_h\":\"" + data.dht_h +
    "\",\"dht_t\":\"" + data.dht_t +
    "\",\"bmp280_p\":\"" + data.bmp280_p +
    "\",\"bmp280_t\":\"" + data.bmp280_t + "\"}";

    return info;
  }

  String getInfoJson() {    
    String info = "{\"ssid\":\"" + String(settings.ssid) + 
    "\",\"ap_ssid\":\"" + String(settings.ap_ssid) +
    "\",\"human_name\":\"" + String(settings.human_name) +
    "\",\"announce_topic\":\"" + String(settings.announce_topic) +
    "\",\"device_id\":\"" + String(getDeviceId()) +
    "\",\"broker\":\"" + String(settings.broker) +
    "\",\"topic\":\"" + String(settings.topic) +
    "\",\"strip_size\":\"" + String(settings.strip_size, DEC) + 
    "\",\"build\":\"" + VERSION +
    "\",\"ip\":\"" + WiFi.localIP().toString() + "\"}";

    return info;
  }
}
