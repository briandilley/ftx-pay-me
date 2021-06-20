
#ifndef DATA_FETCHER_H
#define DATA_FETCHER_H

#include <Time.h>
#include <EthernetClient.h>
#include <HttpClient.h>
#include <ArduinoJson.h>
#include <string>

class DataFetcher {
public:
    DataFetcher();

    void setup();
    bool loop();

    std::string& getMessage();



private:
    time_t lastUpdate;
    std::string message;
    EthernetClient ethernet;
    HttpClient http;

};

#endif
