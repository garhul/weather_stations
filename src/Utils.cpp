
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
    return String("WeatherStation_") + String(ESP.getChipId(), HEX);
  }

  String getSensorValues() {    
    DHT dht(DHTPIN, DHTTYPE);
    Adafruit_BMP280 bme; 
    dht.begin();
  
    if (!bme.begin())
      return String("{\"error\":\"Could not find a valid BMP280 sesnor\"}");      
  
    sensorData data;
    data.vbat = String((analogRead(A0) * VBAT_MULTIPLIER), 2);
    data.dht_h = String(dht.readHumidity(), 1);
    data.dht_t = String(dht.readTemperature(), 1);
    data.bmp280_t = String(bme.readTemperature(), 1);
    data.bmp280_p = String(bme.readPressure(), 1);

     String info = "{\"vbat\":\"" + data.vbat + 
    "\",\"dht_h\":\"" + data.dht_h +
    "\",\"dht_t\":\"" + data.dht_t +
    "\",\"bmp280_p\":\"" + data.bmp280_p +
    "\",\"bmp280_t\":\"" + data.bmp280_t + 
    "\",\"deviceInfo\":" + Utils::getInfoJson() + "}";

    return info;
  }

  String getInfoJson() {    
    String info = "{\"ssid\":\"" + String(settings.ssid) + 
    "\",\"ap_ssid\":\"" + String(settings.ap_ssid) +
    "\",\"description\":\"" + String(settings.description) +
    "\",\"sleep_time\":\"" + String(settings.sleep) +
    "\",\"device_id\":\"" + String(getDeviceId()) +
    "\",\"broker\":\"" + String(settings.broker) +
    "\",\"topic\":\"" + String(settings.topic) +    
    "\",\"build\":\"" + VERSION +
    "\",\"ip\":\"" + WiFi.localIP().toString() + "\"}";

    return info;
  }
}
