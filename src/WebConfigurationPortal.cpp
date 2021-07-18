
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
        settings->loadDefaults();
        settings->save();

        WiFi.disconnect(true);
        ESP.eraseConfig();
        delay(2000);
        ESP.reset();
        delay(2000);
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
                
            } else if (std::string("enablePayoutDisplay") == name) {
                settings->get()->enablePayoutDisplay = std::string(value) == "true";
                
            } else if (std::string("enablePersonalMessageDisplay") == name) {
                settings->get()->enablePersonalMessageDisplay = std::string(value) == "true";
                
            } else if (std::string("btc") == name) {
                settings->get()->btc = std::string(value) == "true";

            } else if (std::string("eth") == name) {
                settings->get()->eth = std::string(value) == "true";

            } else if (std::string("message") == name) {
                strcpy(settings->get()->message, value);

            } else if (std::string("scrollSpeed") == name) {
                settings->get()->scrollSpeed = min(200, max(20, std::stoi(value)));

            } else if (std::string("dataUpdateFrequencySeconds") == name) {
                settings->get()->dataUpdateFrequencySeconds = min(10, max(60, std::stoi(value)));
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
        <form action="/save" method="POST">
            <p><strong>Fill out the form below to configure</strong></p>
            <p>
                <input onchange="setPayoutDisplayEnabled(this.checked)" type="checkbox" id="enablePayoutDisplay" name="enablePayoutDisplay" value="true" {enablePayoutDisplay} /> <label for="enablePayoutDisplay">Enable payout display</label> <br />
                <label for="fttTokens" id="fttTokens_label">How many FTT tokens are you awarded per month?<br>
                <input type="number" id="fttTokens" name="fttTokens" value="{fttTokens}">
                </label>
            </p>
            <p>
                <label>Include additional coiints</label> <br />
                <input type="checkbox" id="btc" name="btc" value="true" {btc} /> <label for="btc">BTC</label> <br />
                <input type="checkbox" id="eth" name="eth" value="true" {eth}/ > <label for="eth">ETH</label> <br />
            </p>
            <p>
                <input onchange="setPersonalMessageEnabled(this.checked)" type="checkbox" id="enablePersonalMessageDisplay" name="enablePersonalMessageDisplay" value="true" {enablePersonalMessageDisplay} /> <label for="enablePersonalMessageDisplay">Enable personal message</label> <br />
                <label for="message" id="message_label">Personalized message<br />
                <input type="text" name="message" value="{msg}" maxlength="1024">
                </label>
            </p>
            <p>
                <label>Scroll speed</label><br>
                <input type="range" name="scrollSpeed" value="{scrollSpeed}" min="20" max="200">
            </p>
            <p>
                <label>Data update frequency (seconds)</label><br>
                <input type="number" name="dataUpdateFrequencySeconds" value="{dataUpdateFrequencySeconds}">
            </p>
            <button type="submit">Save</button> <button onclick="resetDevice(); return false;">Reset</button>
        </form>
        <script>
            function resetDevice() {
                if (!confirm("This will reset the device and lose all of your configuration, are you sure?")) {
                    return;
                }
                document.getElementById("reset").submit();
            }
            function setPayoutDisplayEnabled(enabled) {
                document.getElementById("fttTokens_label").style.display = enabled ? "block" : "none";
                document.getElementById("fttTokens").style.display = enabled ? "block" : "none";
            }
            setPayoutDisplayEnabled("{enablePayoutDisplay}" == "checked");
            function setPersonalMessageEnabled(enabled) {
                document.getElementById("message_label").style.display = enabled ? "block" : "none";
                document.getElementById("message").style.display = enabled ? "block" : "none";
            }
            setPersonalMessageEnabled("{enablePersonalMessageDisplay}" == "checked");
        </script>
    );
    replaceAll(ret, "{fttTokens}", std::to_string(settings->fttTokens));
    replaceAll(ret, "{enablePayoutDisplay}", settings->enablePayoutDisplay ? "checked" : "");
    replaceAll(ret, "{btc}", settings->btc ? "checked" : "");
    replaceAll(ret, "{eth}", settings->eth ? "checked" : "");
    replaceAll(ret, "{enablePersonalMessageDisplay}", settings->enablePersonalMessageDisplay ? "checked" : "");
    replaceAll(ret, "{msg}", std::string(settings->message));
    replaceAll(ret, "{scrollSpeed}", std::to_string(settings->scrollSpeed));
    replaceAll(ret, "{dataUpdateFrequencySeconds}", std::to_string(settings->dataUpdateFrequencySeconds));
    return ret;
}
