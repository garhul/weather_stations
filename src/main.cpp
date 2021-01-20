#include <Arduino.h>
#include <Hash.h>

#include <types.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <MQTT.h>

#include <Utils.h>
// #include <Network.h>
// #include <WebServer.h>

#define BUFF_SIZE 1024
#define VBAT_ALERT_THRESHOLD 3.5f
#define VBAT_DISCONNECT_THRESHOLD 3.4f

WiFiClient net;
MQTTClient client(BUFF_SIZE);

void setup(void) {
  Serial.begin(115200);
  // system_deep_sleep_set_option(2); // No RF calibration
  // int start = millis();
  // pinMode(D5, INPUT_PULLUP);
  // bool cfgBtn = false; //digitalRead(D5);
  // digitalWrite(LED_BUILTIN, !cfgBtn);
  // byte attempts = 0;
  // // Utils::initStorage();
  // // Serial.begin(115200);

  // // Network::init(Utils::settings.ssid, Utils::settings.pass);

  // // if (Network::getMode() == Network::MODES::ST && !cfgBtn) {
  // //   client.begin(Utils::settings.broker, net);

  // //   while (!client.connect(Utils::getDeviceId().c_str()) && attempts < 3) {
  // //     Serial.print(".");
  // //     delay(10);
  // //     attempts++;
  // //   }
  // //   // TODO: Check analog read and if it's lower than vbat threshold raise an alert

  // //   client.publish(String(Utils::settings.topic), Utils::getSensorValues());
  // //   Serial.println("Duration: " + String(millis() - start, DEC));
  // //   delay(10);

  // //   ESP.deepSleep(((Utils::settings.sleep * 1000000)));
  // // }

  // // Serial.println("Starting config mode");
  // // WebServer::init();
}

void loop(void) {
  sensorData d = Utils::readSensors();
  Serial.println(
    "temp: " + d.temp + "\n" +
    "vbat: " + d.vbat + "\n" +
    "humidity: " + d.humidity + "\n" +
    "pressure: " + d.pressure + "\n ------------- ");

  delay(1000);

  // yield();
  // // Network::checkAlive();
  // WebServer::loop();
  // if (!digitalRead(D5)) {
  //   Serial.println("going back");
  //   ESP.deepSleep(((Utils::settings.sleep * 1000)));
  // }
}
