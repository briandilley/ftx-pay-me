
#include "DataFetcher.h"

DataFetcher::DataFetcher() :
    ethernet(),
    http(ethernet),
    lastUpdate(0),
    message("Loading...") {

}

void DataFetcher::setup() {

}

bool DataFetcher::loop() {
    time_t current = now();
    time_t elapsed = current - lastUpdate;
    if (elapsed < 30) {
        message = std::to_string(elapsed);
        return true;
    }
    lastUpdate = current;

    if (http.get("ftx.com", "/api/markets/FTT/USD/candles/last?resolution=300") < 0) {
        message = "Unable to fetch data";
        return true;

    } else if (http.responseStatusCode() != 200) {
        message = std::string("Non 200 response code: ") + std::to_string(http.responseStatusCode());
        return true;
    }


    return true;
}

std::string& DataFetcher::getMessage() {
    return message;
}