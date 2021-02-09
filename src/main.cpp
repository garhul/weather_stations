#include <Arduino.h>
#include <Hash.h>

#include <types.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <MQTT.h>

#include <Utils.h>
#include <Network.h>
#include <WebServer.h>
ADC_MODE(ADC_VCC);
#define BUFF_SIZE 1024
#define VBAT_ALERT_THRESHOLD 3.5f
#define VBAT_DISCONNECT_THRESHOLD 3.4f

#define MODE_SWITCH D5
#define MODE_SETUP 0
#define MODE_REPORT 1


WiFiClient net;
MQTTClient client(BUFF_SIZE);

void setup(void) {
  int start = millis();
  // system_deep_sleep_set_option(2); // No RF calibration
  pinMode(MODE_SWITCH, INPUT_PULLUP);
  bool cfgBtn = digitalRead(MODE_SWITCH);

  digitalWrite(LED_BUILTIN, cfgBtn);

  byte attempts = 0;
  Utils::initStorage();
  Serial.begin(115200);

  Network::init(Utils::settings.ssid, Utils::settings.pass);

  if (cfgBtn == MODE_REPORT) {
    if (Network::getMode() == Network::MODES::ST) {
      client.begin(Utils::settings.broker, net);

      while (!client.connect(Utils::getDeviceId().c_str()) && attempts < 3) {
        Serial.print(".");
        delay(10);
        attempts++;
      }

      if (!client.connected()) return;

      sensorData d = Utils::getSensorData();
      String report = String("{\"data\": {") +
        "\"t\":\"" + String(d.temp) +
        "\",\"h\":\"" + String(d.humidity) +
        "\",\"p\":\"" + String(d.pressure) +
        "\",\"vbat\":\"" + String(d.vbat) +
        "\"},\"id\":\"" + String(Utils::getDeviceId()) +
        "\",\"name\":\"" + String(Utils::settings.name) + "\"}";

      Serial.println(report);

      //data package expects: {"d":"string", "t":"", "p": "vbat" }
      client.publish(String(Utils::settings.topic), report);

      Serial.println("Duration: " + String(millis() - start, DEC));
      delay(10);

      ESP.deepSleep(((Utils::settings.sleep * 1000000)));
    }
  }
  else {
    Serial.println("Starting config mode");
    WebServer::init();
  }
}

void loop(void) {
  yield();
  WebServer::loop();
  if (digitalRead(MODE_SWITCH) == MODE_REPORT) {
    Serial.println("Restarting");
    ESP.deepSleep(((Utils::settings.sleep * 1000)));
  }
}
