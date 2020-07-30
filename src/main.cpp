#include <Arduino.h>
#include <Hash.h>

#include <types.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <MQTT.h>

#include <Utils.h>
#include <Network.h>
#include <Utils.h>
#include <WebServer.h>

#define BUFF_SIZE 1024

WiFiClient net;
MQTTClient client(BUFF_SIZE);

void setup ( void ) {  
  int start = millis();
  pinMode(D5, INPUT_PULLUP);  
  bool cfgBtn = digitalRead(D5);
  digitalWrite(LED_BUILTIN, !cfgBtn);
  byte attempts = 0;
  Utils::initStorage();
  Serial.begin(115200);

  Network::init(Utils::settings.ssid, Utils::settings.pass);  
  Serial.println("el pin dice " + String(digitalRead(D5),10));  
  if (Network::getMode() == Network::MODES::ST && !cfgBtn) {
    client.begin(Utils::settings.broker, net);

    while (!client.connect(Utils::getDeviceId().c_str()) && attempts < 3) {
      Serial.print(".");
      delay(10);
      attempts++;
    }

    client.publish(String(Utils::settings.topic), Utils::getSensorValues());
    Serial.println("Duration: " + String(millis() - start, DEC));
    delay(10);
    ESP.deepSleep(((Utils::settings.sleep * 1000)));
  }

  Serial.println("Starting config mode");
  WebServer::init();
}

void loop ( void ) {
  yield();
  // Network::checkAlive();
  WebServer::loop();  
  if (!digitalRead(D5)) {
    Serial.println("going back");
    ESP.deepSleep(((Utils::settings.sleep * 1000)));
  }
}
