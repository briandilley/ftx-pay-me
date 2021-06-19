
#include "WiFiController.h"

WiFiController::WiFiController(std::string name, std::string password, int portalTimeout) {
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

void WiFiController::setupWiFi() {

  // wifiManager.resetSettings();  // this will delete all credentials
  wifiManager.setDebugOutput(false);
  wifiManager.setConfigPortalTimeout(this->portalTimeout);
  wifiManager.setAPCallback([] (WiFiManager *myWiFiManager) {
    Serial.println("- No known wifi found");
    Serial.print("- Starting AP: ");
    //Serial.println(myWiFiManager->getConfigPortalSSID());
    Serial.println(WiFi.softAPIP());
  });

  // enable autoconnect
  if (!(this->password == "" ? 
    wifiManager.autoConnect(this->name.c_str()) : 
    wifiManager.autoConnect(this->name.c_str(), this->password.c_str()))
   ) {
    Serial.println("- Failed to connect and hit timeout");
    ESP.reset();
    delay(1000); 
  }

}