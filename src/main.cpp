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
#define VBAT_ALERT_THRESHOLD 3.2f
#define VBAT_DISCONNECT_THRESHOLD 3.1f

#define MODE_SWITCH D5
#define MODE_SETUP 0
#define MODE_REPORT 1


WiFiClient net;
MQTTClient client(BUFF_SIZE);

void setup(void) {
  WiFi.mode(WIFI_OFF);
  pinMode(MODE_SWITCH, INPUT_PULLUP);
  bool cfgBtn = digitalRead(MODE_SWITCH);
  digitalWrite(LED_BUILTIN, cfgBtn);
  Serial.begin(115200);
  byte attempts = 0;
  Utils::initStorage();
  uint64_t sleepTime = Utils::settings.sleep * 1000000;

  sensorData d = Utils::getSensorData();
  String report = String("{\"data\": {") +
    "\"t\":\"" + String(d.temp) +
    "\",\"h\":\"" + String(d.humidity) +
    "\",\"p\":\"" + String(d.pressure) +
    "\",\"vbat\":\"" + String(d.vbat) +
    "\"},\"id\":\"" + String(Utils::getDeviceId()) +
    "\",\"name\":\"" + String(Utils::settings.name) + "\"}";

  Serial.println(report);
  Network::init(Utils::settings.ssid, Utils::settings.pass);

  if (cfgBtn == MODE_SETUP) {
    Serial.println("Starting config mode");
    WebServer::init();
    return;
  }

  if (Network::getMode() == Network::MODES::ST) {
    client.begin(Utils::settings.broker, net);

    while (!client.connect(Utils::getDeviceId().c_str()) && attempts < 3) {
      Serial.print(".");
      delay(10);
      attempts++;
    }

    if (client.connected()) {
      client.publish(String(Utils::settings.topic), report);

      if (d.vbat.toFloat() < VBAT_DISCONNECT_THRESHOLD) {

        client.publish(String(Utils::settings.topic),
          "{ \"id\":\"" + String(Utils::getDeviceId()) + "\"," +
          "\"alert\":\"VBAT_CRITICAL\"}");
        sleepTime = 0;
      }
      else if (d.vbat.toFloat() < VBAT_ALERT_THRESHOLD) {
        client.publish(String(Utils::settings.topic),
          "{ \"id\":\"" + String(Utils::getDeviceId()) + "\"," +
          "\"alert\":\"VBAT_LOW\"}");
      }
    }
    else {
      Serial.println("unable to connect to broker");
    }
  }

  delay(100); //finish processing messages  
  WiFi.disconnect();
  WiFi.forceSleepBegin();
  delay(1);
  ESP.deepSleep(sleepTime, WAKE_RF_DEFAULT);
}

void loop(void) {
  yield();
  WebServer::loop();
  if (digitalRead(MODE_SWITCH) == MODE_REPORT) {
    Serial.println("Restarting");
    ESP.deepSleep(((Utils::settings.sleep * 1000)));
  }
}
