#include <Arduino.h>
#include <Hash.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <MQTT.h>

#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
#include <Utils.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define BUFF_SIZE 1024


//TODO:: move this to EEPROM config
#define VBAT_MULTIPLIER 0.0198f
#define SLEEP_TIME 10e6 //microseconds
#define LOCAL_SSID "Brothel Misa" //TODO:: move this to some config stuff
#define LOCAL_PWD "H1p0l1t0Pr351d3nt3"
#define BROKER "192.168.0.10"
#define TOPIC "home/living"

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bme; 

WiFiClient net;
MQTTClient client(BUFF_SIZE);


void connect() { // TODO:: set timeout on wifi
  WiFi.persistent(false);
  WiFi.begin(LOCAL_SSID, LOCAL_PWD);
  client.begin(BROKER, net);

  Serial.print("connecting to wifi");

  while (WiFi.status() != WL_CONNECTED){
    delay(10);
  }

  Serial.print("\nConnecting to broker"); // TODO :: implement device_id
  while (!client.connect("weather_st", "try", "try")) {
    Serial.print(".");
    delay(10);
  }
}

void setup() {
  int start = millis();
  WiFi.disconnect();
  Serial.begin(115200);
  digitalWrite(LED_BUILTIN, HIGH);
  dht.begin();
  
  if (!bme.begin()) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  
  while(true) {
    yield();
    Serial.println(analogRead(A0));
    Serial.println(analogRead(A0));

    sensorData vals;
    vals.vbat = String((analogRead(A0) * VBAT_MULTIPLIER), 2);
    vals.dht_h = String(dht.readHumidity(), 1);
    vals.dht_t = String(dht.readTemperature(), 1);
    vals.bmp280_t = String(bme.readTemperature(), 1);
    vals.bmp280_p = String(bme.readPressure(), 1);

    connect();
    client.publish(String(TOPIC) + "/sensor", Utils::getSensorValues(vals));

    Serial.println( Utils::getSensorValues(vals));
    delay(3000);
  }
  // sensorData vals;
  // vals.vbat = String((analogRead(A0) * VBAT_MULTIPLIER), 2);
  // vals.dht_h = String(dht.readHumidity(), 1);
  // vals.dht_t = String(dht.readTemperature(), 1);
  // vals.bmp280_t = String(bme.readTemperature(), 1);
  // vals.bmp280_p = String(bme.readPressure(), 1);

  // connect();
  // client.publish(String(TOPIC) + "/sensor", Utils::getSensorValues(vals));

  // Serial.println( Utils::getSensorValues(vals));
  // // Serial.println("Waking time: ");Serial.print(millis()-start);Serial.println("ms");
  // delay(100);
  // ESP.deepSleep(SLEEP_TIME); //todo:: implement settings endpoint and button for going into main after bootup
}

void loop() {}