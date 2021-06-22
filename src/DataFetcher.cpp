
#include "DataFetcher.h"

DataFetcher::DataFetcher() :
    currentMessage(MSG_FTT_PRICE),
    lastUpdatedMessage(MSG_LAST),
    lastUpdate(0),
    messages({"Loading FTT", "Loading FTT Payout", "Loading BTC", "Loading ETH", "Loading Message"}),
    json() {

}

void DataFetcher::setup() {
   
}

const char* DataFetcher::getMessage() {
    return messages[currentMessage].c_str();
}

void DataFetcher::maybeFetchData(bool forceLoad) {

    Settings* settings = Settings::getInstance();
    messages[MSG_PERSONALIZED_MESSAGE] = settings->get()->message;
    bool payoutEnabled = settings->get()->enablePayoutDisplay;
    bool btcEnabled = settings->get()->btc;
    bool ethEnabled = settings->get()->eth;
    bool personalEnabled = settings->get()->enablePersonalMessageDisplay;

    // cycle messages
    currentMessage = currentMessage + 1;
    if (currentMessage == MSG_FTT_PAYOUT && !payoutEnabled) {
        currentMessage = currentMessage + 1;
    }
    if (currentMessage == MSG_BTC_PRICE && !btcEnabled) {
        currentMessage = currentMessage + 1;
    }
    if (currentMessage == MSG_ETH_PRICE && !ethEnabled) {
        currentMessage = currentMessage + 1;
    }
    if (currentMessage == MSG_PERSONALIZED_MESSAGE && !personalEnabled) {
        currentMessage = currentMessage + 1;
    }
    if (currentMessage > MSG_LAST) {
        currentMessage = MSG_FIRST;
    }

    // only fetch data if enough time has passed
    unsigned long current = millis();
    unsigned long elapsed = current - lastUpdate;
    if (!forceLoad
            && (elapsed < (10 * 1000))
            && !messages[currentMessage].startsWith("Loading")) {
        return;
    }
    lastUpdate = current;

    // cycle updates
    if (messages[currentMessage].startsWith("Loading")) {
        lastUpdatedMessage = currentMessage;

    } else {
        lastUpdatedMessage = lastUpdatedMessage + 1;
        if (lastUpdatedMessage == MSG_FTT_PAYOUT) {
            lastUpdatedMessage = lastUpdatedMessage + 1;
        }
        if (lastUpdatedMessage == MSG_BTC_PRICE && !btcEnabled) {
            lastUpdatedMessage = lastUpdatedMessage + 1;
        }
        if (lastUpdatedMessage == MSG_ETH_PRICE && !ethEnabled) {
            lastUpdatedMessage = lastUpdatedMessage + 1;
        }
        if (lastUpdatedMessage == MSG_PERSONALIZED_MESSAGE && !personalEnabled) {
            lastUpdatedMessage = lastUpdatedMessage + 1;
        }
        if (lastUpdatedMessage > MSG_LAST) {
            lastUpdatedMessage = MSG_FIRST;
        }
    }

    // FTT / Payout
    if (lastUpdatedMessage == MSG_FTT_PRICE) {
        double price = requestJson("FTT");
        if (price == -1) {
            messages[MSG_FTT_PRICE] = "Problem loading FTT price";
            return;
        }
        messages[MSG_FTT_PRICE] = "FTT: $" + String(price, 3);
        messages[MSG_FTT_PAYOUT] = "Your payout is: $" + String(settings->get()->fttTokens * price, 2);

    // BTC
    } else if (lastUpdatedMessage == MSG_BTC_PRICE) {
        double price = requestJson("BTC");
        if (price == -1) {
            messages[MSG_BTC_PRICE] = "Problem loading BTC price";
            return;
        }
        messages[MSG_BTC_PRICE] = "BTC: $" + String(price, 3);

    // ETH
    } else if (lastUpdatedMessage == MSG_ETH_PRICE) {
        double price = requestJson("ETH");
        if (price == -1) {
            messages[MSG_ETH_PRICE] = "Problem loading ETH price";
            return;
        }
        messages[MSG_ETH_PRICE] = "ETH: $" + String(price, 3);

    }

    return;
}

 double DataFetcher::requestJson(std::string coin) {

    WiFiClientSecure client;
    client.setInsecure();

    if (!client.connect("ftx.com", 443)) {
        return -1;
    }

    client.println((std::string("GET /api/markets/") + coin + "/USD/candles/last?resolution=300 HTTP/1.0").c_str());
    client.println("Host: ftx.com");
    client.println("Connection: close");
    client.println();

    // read headers
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }

    if (!client.connected()) {
        return -1;
    }

    // read body
    DeserializationError res = deserializeJson(json, client);
    if (res) {
        return -1;
    }

    double price = json["result"]["close"];

    // we're done
    client.stop();
    return price;
 }

