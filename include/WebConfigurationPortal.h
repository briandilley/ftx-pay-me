
#ifndef WEB_CONFIGURATION_PORTAL_H
#define WEB_CONFIGURATION_PORTAL_H

#include <WiFiManager.h>
#include <aWOT.h>
#include <string>
#include "Settings.h"

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

void replaceAll(std::string& str, const std::string& from, const std::string& to);

#define HTML(...) #__VA_ARGS__

const std::string HTML_BODY_START = HTML(
<!doctype html>
<html lang="en-US">
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>FTX Pay Me Configurator</title>
        <link rel="stylesheet" href="https://cdn.simplecss.org/simple.min.css">
    </head>
    <body>
    <header>
        <h1>FTX Pay Me Configurator</h1>
        <p>Gimmie that money, bitch</p>
    </header>
);

const std::string HTML_CONFIRMATION = HTML(
    <p>
        <p><strong>Configuration saved</strong></p>
        <p><a href="/">back</a></p>
    </p>
);

const std::string HTML_RESET = HTML(
    <p>
        <p><strong>Device rest, wait a few seconds and then re-configure</strong></p>
    </p>
);

const std::string HTML_BODY_END = HTML(
        <footer>
            <p>FTX Pay Me Configurator.</p>
        </footer>
    </body>
</html>
);

const std::string HTML_FORM(FtxPayMeSettings* settings);


#endif // WEB_CONFIGURATION_PORTAL_H
