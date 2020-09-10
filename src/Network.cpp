#include <Network.h>

namespace Network {
  byte mode;

  void init(String ssid, String pwd) {
    mode = MODES::DISCONNECTED;
    
    if (beginST(ssid.c_str(), pwd.c_str())) {
      mode = MODES::ST;
    } else {
      mode = MODES::AP;
      beginAP();
    }
  }
 
  byte getMode() {
   return mode;
  }

  void beginAP() {
    WiFi.mode(WIFI_AP);

    #if AP_USE_PWD
      WiFi.softAP(String(st.ap_ssid), AP_PWD);
    #else
      settings_t st = Utils::getSettings();      
      WiFi.softAP(String(st.ap_ssid));
    #endif

    WiFi.printDiag(Serial);
    Serial.println(WiFi.softAPIP());
  }

  bool beginST(const char* ssid, const char* pwd) {  
    int attempts = 0;

    #ifdef __NO_DHCP
      IPAddress staticIP(192, 168, 0, 20); //static IP address
      IPAddress gateway(192, 168, 0, 1); //Router's IP address
      IPAddress subnet(255, 255, 255, 0);
      IPAddress dns(8, 8, 8, 8);
      WiFi.hostname(Utils::getDeviceId());
      WiFi.config(staticIP, subnet, gateway, dns);
    #endif
    
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(); // disconnect from any previously connected network (open networks?) 
    
    // while (WiFi.status() != WL_CONNECTED && attempts < ST_RETRIES) {
    WiFi.begin (ssid, pwd);
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED){
      delay(10);
      if ((millis() - start) > ST_CONN_TIMEOUT) return false;
    }

    Serial.println("Station startup successful");
    WiFi.printDiag(Serial);
    Serial.println (WiFi.localIP());

    return true;
  }

  /**
   * checks the connection is still alive, if not resets the device
  */  
  void checkAlive() {
    Serial.println("Wifi not connected");
    if (!WiFi.isConnected()) ESP.reset();
  }
}
