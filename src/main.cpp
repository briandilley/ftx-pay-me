
#include "Settings.h"
#include "WiFiController.h"
#include "WebConfigurationPortal.h"
#include "Banner.h"
#include "DataFetcher.h"

/* ------------------------------------------------- */
#define PORTAL_TIMEOUT  10 * 60 // seconds
#define AP_NAME         "FTX PAY ME"
#define AP_PASSWORD     ""
/* ------------------------------------------------- */

Settings* settings = Settings::getInstance();
WiFiController wifiController(AP_NAME, AP_PASSWORD, PORTAL_TIMEOUT);
WebConfigurationPortal webConfigurationPortal;
Banner banner(MD_MAX72XX::FC16_HW, D8, 4, 1024, 50);
DataFetcher dataFetcher;

void setup() {
  wifiController.setup();
  banner.setup();
  banner.setMessage("FTX PAY ME");
  dataFetcher.setup();
  Serial.begin(115200);
  delay(1000);
}

void loop() {

  bool doneAnimating = banner.loop();

  // wifi not setup yet
  if (!wifiController.isConnected()) {
    if (doneAnimating) {
      banner.setMessage((std::string("Connect to SSID ") + AP_NAME + " and configure wireless connection").c_str());
    }
    return;
  }

  // make sure the web configuration portal is up
  if (!webConfigurationPortal.isReady()) {
    webConfigurationPortal.setup();
  }
  webConfigurationPortal.loop();

  // if we haven't ever been configured, prompt for that now
  if (!settings->get()->configured) {
    if (doneAnimating) {
      IPAddress addr = wifiController.getIPAddress();
      String message = String("go to http://") + addr.toString() + " and configure me";
      banner.setMessage(message.c_str());
    }
    return;
  }

  // update the banner
  banner.setScrollSpeed(settings->get()->scrollSpeed);

  // potentially load more data
  if (doneAnimating) {
    dataFetcher.maybeFetchData(false);
    banner.setMessage(dataFetcher.getMessage());
  }

}
