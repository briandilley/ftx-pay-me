   
#include <WiFiManager.h>
#include <string>

class WiFiController {
public:
    WiFiController(std::string name, std::string password, int portalTimeout);
    bool isConnected();
    IPAddress getIPAddress();
    void setupWiFi();

private:
    WiFiManager wifiManager;
    std::string name;
    std::string password;
    int portalTimeout;

};
