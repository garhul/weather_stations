
#include <Utils.h>

namespace Utils {
  settings_t settings = {};

  void initStorage() {
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

  sensorData getSensorData() {
    sensorData d;
    d.humidity = "NA";
    d.temp = "NA";
    d.pressure = "NA";
    d.vbat = String(ESP.getVcc() / 900.9f);

#ifdef USE_DHT11
    DHT dht(DHTPIN, DHTTYPE);
    dht.begin();
    d.humidity = String(dht.readHumidity(), 1);
#endif

#ifdef USE_BMP280
    Adafruit_BMP280 bmp;
    if (bmp.begin()) {
      d.temp = String(bmp.readTemperature(), 1);
      d.pressure = String(bmp.readPressure() / 100.0f, 1);
    }
#endif

#ifdef USE_BME280
    Adafruit_BME280 bme;
    if (bme.begin()) {
      d.humidity = String(bme.readHumidity(), 1);
      d.temp = String(bme.readTemperature(), 1);
      d.pressure = String(bmp.readPressure() / 100.0f, 1);
    }
#endif
    return d;
  }

  String getInfoJson() {
    String info = "{\"ssid\":\"" + String(settings.ssid) +
      "\",\"ap_ssid\":\"" + String(settings.ap_ssid) +
      "\",\"name\":\"" + String(settings.name) +
      "\",\"sleep_time\":\"" + String(settings.sleep) +
      "\",\"device_id\":\"" + String(getDeviceId()) +
      "\",\"broker\":\"" + String(settings.broker) +
      "\",\"topic\":\"" + String(settings.topic) +
      "\",\"build\":\"" + VERSION +
      "\",\"ip\":\"" + WiFi.localIP().toString() + "\"}";

    return info;
  }
}
