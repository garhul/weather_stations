
#include <WebServer.h>

namespace WebServer {
  ESP8266WebServer server(80);

  void init() {
    server.on("/setup", HTTP_POST, _setup);    
    server.on("/clear", HTTP_POST, _clearCredentials); //endpoint for clearing ssid / pwd
    server.on("/info", HTTP_ANY, _info);    
    server.onNotFound(_info);
    server.begin();    
  }

  void loop() {
    server.handleClient();
  }

  void _setup() {
    settings_t settings = {};
    
    //check post params exist
    if(
        !server.hasArg("pass") ||
        !server.hasArg("ssid") ||
        !server.hasArg("topic")||
        !server.hasArg("broker") ||
        !server.hasArg("sleep") ||
        !server.hasArg("description") ||
        !server.hasArg("ap_ssid")        
      ) {
      server.send(400, "text/plain", "Invalid params one of [pass, ssid, topic, broker, description, sleep, ap_ssid] is missing");
      return;
    }

    if (!(Utils::clearStorage())) {
      server.send(500, "text/plain", "Error clearing credentials");
      return;
    };

    server.arg("ssid").toCharArray(settings.ssid, 32);
    server.arg("pass").toCharArray(settings.pass, 32);
    server.arg("topic").toCharArray(settings.topic, 32);
    server.arg("broker").toCharArray(settings.broker, 32);    
    server.arg("ap_ssid").toCharArray(settings.ap_ssid, 32);
    server.arg("description").toCharArray(settings.description, 100);    
    settings.sleep = server.arg("sleep").toInt();

    if (Utils::storeSettings(settings)) {
      server.send(200, "text/plain", "Settings stored");
      delay(2000);
      ESP.restart();
      return;
    }

    server.send(500, "text/plain", "Unable to store settings");
  }

  void _info() {
    server.send(200, "application/json", Utils::getInfoJson());
  }

  void _clearCredentials() {
    if (Utils::clearStorage()) {
      server.send(200, "text/plain", "EEPROM cleared!");
      return;
    }

    server.send(500, "text/plain", "Error clearing EEEPROM'}");
  }
}
