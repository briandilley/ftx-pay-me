
#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H

#include <WiFiManager.h>
#include <string>

class WiFiController {
public:
    WiFiController(std::string name, std::string password, int portalTimeout);
    bool isConnected();
    IPAddress getIPAddress();
    
    void setup();

private:
    WiFiManager wifiManager;
    std::string name;
    std::string password;
    int portalTimeout;

};

#endif // WIFI_CONTROLLER_H