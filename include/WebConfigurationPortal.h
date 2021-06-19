
#ifndef WEB_CONFIGURATION_PORTAL_H
#define WEB_CONFIGURATION_PORTAL_H

#include <WiFiManager.h>
#include <aWOT.h>

class WebConfigurationPortal {
public:
    WebConfigurationPortal();

    bool isReady();
    void setup();
    void loop();

private:
    WiFiServer server;
    Application app;
    bool ready;
};

#endif // WEB_CONFIGURATION_PORTAL_H
