
#include "WiFiController.h"

WiFiController::WiFiController(std::string name, std::string password, int portalTimeout) :
      wifiManager() {
    this->name = name;
    this->password = password;
    this->portalTimeout = portalTimeout;
}

bool WiFiController::isConnected() {
    return (WiFi.status() == WL_CONNECTED);
}

IPAddress WiFiController::getIPAddress() {
    return WiFi.localIP();
}

void WiFiController::setup() {

  // wifiManager.resetSettings();  // this will delete all credentials
  wifiManager.setDebugOutput(false);
  wifiManager.setConfigPortalTimeout(this->portalTimeout);
  wifiManager.setAPCallback([] (WiFiManager *myWiFiManager) {
    
  });

  // enable autoconnect
  if (!(this->password == "" ? 
    wifiManager.autoConnect(this->name.c_str()) : 
    wifiManager.autoConnect(this->name.c_str(), this->password.c_str()))
   ) {
    ESP.reset();
    delay(1000); 
  }

}