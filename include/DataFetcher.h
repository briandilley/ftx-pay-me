
#ifndef DATA_FETCHER_H
#define DATA_FETCHER_H

#include <string>
#include <WiFiClientSecure.h>
#include <HttpClient.h>
#include <ArduinoJson.h>
#include "Settings.h"

#define MSG_FTT_PRICE 0
#define MSG_FTT_PAYOUT 1
#define MSG_BTC_PRICE 2
#define MSG_ETH_PRICE 3
#define MSG_PERSONALIZED_MESSAGE 4
#define MSG_FIRST 0
#define MSG_LAST 4

class DataFetcher {
public:
    DataFetcher();
    void setup();
    void maybeFetchData(bool forceLoad);
    const char* getMessage();

private:
    double requestJson(std::string coinpair);
    int currentMessage; 
    int lastUpdatedMessage; 
    unsigned long lastUpdate;
    String messages[5];
    StaticJsonDocument<256> json;

};

#endif
