
#include <WiFiManager.h>

#include "Settings.h"
#include "WebConfigurationPortal.h"
#include "Banner.h"
#include "DataFetcher.h"

/* ------------------------------------------------- */
#define PORTAL_TIMEOUT  10 * 60 // seconds
#define AP_NAME         "FTX PAY ME"
#define AP_PASSWORD     ""
/* ------------------------------------------------- */

Settings* settings = Settings::getInstance();
WebConfigurationPortal webConfigurationPortal;
Banner banner(MD_MAX72XX::FC16_HW, 15, 4, 1024, 50);
DataFetcher dataFetcher;

void setup() {
  Serial.begin(115200);
  while (!Serial) { }
  delay(3000);

  banner.setup();
  banner.setMessage("FTX PAY ME");

  /*
  ESP.eraseConfig();
  delay(2000);
  ESP.reset();
  delay(2000);
  */

  WiFiManager wifiManager;
  wifiManager.setDebugOutput(true);
  if (!wifiManager.autoConnect(AP_NAME, AP_PASSWORD)) {
    Serial.println("failed to connect and hit timeout");
    ESP.reset();
    delay(1000);
    return;
  }

  dataFetcher.setup();
  Serial.println("setup complete");
}

void loop() {

  bool doneAnimating = banner.loop();

  // wifi not setup yet
  if (WiFi.status() != WL_CONNECTED) {
    if (doneAnimating) {
      banner.setMessage("Wireless connection is unavailable");
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
      IPAddress addr = WiFi.localIP();
      banner.setMessage((String("go to http://") + addr.toString() + " and configure me").c_str());
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
