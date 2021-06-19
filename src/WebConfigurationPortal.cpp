
#include "WebConfigurationPortal.h"
#include "html.h"

WebConfigurationPortal::WebConfigurationPortal() :
        server(80),
        app(),
        ready(false) {
}

void WebConfigurationPortal::setup() {

    app.setTimeout(10000);

    app.get("/", [] (Request &request, Response &response) {

        response.set("Content-Type", "text/html");
        response.status(200);
        response.println(HTML_BODY_START.c_str());
        response.println(HTML_FORM(10, true, false).c_str());
        response.println(HTML_BODY_END.c_str());
        response.end();
        
    });

    app.post("/save", [] (Request &request, Response &response) {

        char name[512];
        char value[512];

        int fttTokens = 0;
        bool btc = false;
        bool eth = false;
        std::string message = "";

       while (request.left()) {
            request.form(name, 512, value, 512);

            if (std::string("fttTokens") == name) {
                fttTokens = std::stoi(value);
                
            } else if (std::string("btc") == name) {
                btc = true;

            } else if (std::string("eth") == name) {
                eth = true;

            } else if (std::string("message") == name) {
                message = std::string(value);
            }
        }

        response.set("Content-Type", "text/html");
        response.status(200);
        response.println(HTML_BODY_START.c_str());
        response.println(HTML_CONFIRMATION.c_str());
        response.println(HTML_BODY_END.c_str());
        response.end();
        
    });

    server.begin();
    this->ready = true;
}

bool WebConfigurationPortal::isReady() {
    return this->ready;
}

void WebConfigurationPortal::loop() {
    WiFiClient client = server.available();

    if (client.connected()) {
        app.process(&client);
    }

}
