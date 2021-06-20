
#include "WebConfigurationPortal.h"

WebConfigurationPortal::WebConfigurationPortal() :
        server(80),
        app(),
        ready(false) {
}

void WebConfigurationPortal::setup() {

    app.setTimeout(10000);

    app.get("/", [] (Request &request, Response &response) {

        Settings* settings = Settings::getInstance();

        response.set("Content-Type", "text/html");
        response.status(200);
        response.println(HTML_BODY_START.c_str());
        response.println(HTML_FORM(settings->get()).c_str());
        response.println(HTML_BODY_END.c_str());
        response.end();
        
    });

    app.post("/reset", [] (Request &request, Response &response) { 

        Settings* settings = Settings::getInstance();

        response.set("Content-Type", "text/html");
        response.status(200);
        response.println(HTML_BODY_START.c_str());
        response.println(HTML_RESET.c_str());
        response.println(HTML_BODY_END.c_str());
        response.end();
        response.flush();

        settings->clear();
        ESP.eraseConfig();
        ESP.reset();
    });

    app.post("/save", [] (Request &request, Response &response) {

        Settings* settings = Settings::getInstance();

        char name[512];
        char value[512];

        settings->get()->btc = false;
        settings->get()->eth = false;

       while (request.left()) {
            request.form(name, 512, value, 512);

            if (std::string("fttTokens") == name) {
                settings->get()->fttTokens = std::stoi(value);
                
            } else if (std::string("btc") == name) {
                settings->get()->btc = std::string(value) == "true";

            } else if (std::string("eth") == name) {
                settings->get()->eth = std::string(value) == "true";

            } else if (std::string("message") == name) {
                strcpy(settings->get()->message, value);

            } else if (std::string("scrollSpeed") == name) {
                settings->get()->scrollSpeed = min(200, max(20, std::stoi(value)));
            }
        }

        settings->get()->configured = true;
        settings->save();

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

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty()) return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

const std::string HTML_FORM(FtxPayMeSettings* settings) {
    std::string ret = HTML(
        <form action="/reset" method="POST" id="reset"></form>
        <script>
            function resetDevice() {
                if (!confirm("This will reset the device and lose all of your configuration, are you sure?")) {
                    return;
                }
                document.getElementById("reset").submit();
            }
        </script>
        <form action="/save" method="POST">
            <p><strong>Fill out the form below to configure</strong></p>
            <p>
                <label>How many FTT tokens are you awarded per month?</label><br>
                <input type="number" name="fttTokens" value="{fttTokens}">
            </p>
            <p>
                <label>Include additional coiints</label> <br />
                <input type="checkbox" name="btc" value="true" {btc} /> BTC <br />
                <input type="checkbox" name="eth" value="true" {eth}/ > ETH <br />
            </p>
            <p>
                <label>Personalized message</label><br />
                <input type="text" name="message" value="{msg}" maxlength="1024">
            </p>
            <p>
                <label>Scroll speed</label><br>
                <input type="range" name="scrollSpeed" value="{scrollSpeed}" min="20" max="200">
            </p>
            <button type="submit">Save</button> <button onclick="resetDevice(); return false;">Reset</button>
        </form>
    );
    replaceAll(ret, "{fttTokens}", std::to_string(settings->fttTokens));
    replaceAll(ret, "{btc}", settings->btc ? "checked" : "");
    replaceAll(ret, "{eth}", settings->eth ? "checked" : "");
    replaceAll(ret, "{msg}", std::string(settings->message));
    replaceAll(ret, "{scrollSpeed}", std::to_string(settings->scrollSpeed));
    return ret;
}
