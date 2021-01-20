
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

  sensorData readSensors() {
#ifdef USE_DHT
    DHT dht(DHTPIN, DHTTYPE);
    dht.begin();
#endif

#ifdef USE_BMP280
    Adafruit_BMP280 bmp;
    sensorData d;
    d.vbat = String((analogRead(A0) * VBAT_MULTIPLIER), 2);
    if (!bmp.begin()) {
      d.humidity = "NA";
      d.temp = "NA";
      d.pressure = "NA";
    }
    else {
      d.humidity = "NA";
      d.temp = String(bmp.readTemperature(), 1);
      d.pressure = String(bmp.readPressure(), 1);
    }
    return d;
#endif

#ifdef USE_BME280
    Adafruit_BME280 bme;
    sensorData d;
    d.vbat = String((analogRead(A0) * VBAT_MULTIPLIER), 2);
    if (!bme.begin()) {
      d.humidity = "NA";
      d.temp = "NA";
      d.pressure = "NA";
    }
    else {
      d.humidity = String(bme.readHumidity(), 1);
      d.temp = String(bme.readTemperature(), 1);
      d.pressure = String(bme.readPressure(), 1);
    }
    return d;
#endif


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
