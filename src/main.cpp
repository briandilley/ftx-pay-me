
#include <WiFiManager.h>

#include "Settings.h"
#include "WebConfigurationPortal.h"
#include "Banner.h"
#include "DataFetcher.h"
#include "ESPBattery.h"

/* ------------------------------------------------- */
#define PORTAL_TIMEOUT  10 * 60 // seconds
#define AP_NAME         "FTX PAY ME"
#define AP_PASSWORD     ""
#define HOSTNAME        "ftxpayme"
#define BATTERY_ENABLED false
/* ------------------------------------------------- */

Settings* settings = Settings::getInstance();
ESPBattery battery;
WebConfigurationPortal webConfigurationPortal;
Banner banner(MD_MAX72XX::FC16_HW, 15, 4, 1024, 50);
DataFetcher dataFetcher;
WiFiManager wifiManager;
unsigned long lastBatteryCheck = 0;

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

  wifiManager.setHostname(HOSTNAME);
  wifiManager.setConfigPortalBlocking(false);
  wifiManager.setDebugOutput(true);
  wifiManager.autoConnect(AP_NAME, AP_PASSWORD);
  wifiManager.setSaveConfigCallback([]() {
    ESP.reset();
    delay(2000);
  });

  dataFetcher.setup();
}

void loop() {

  unsigned long now = millis();
  bool doneAnimating = banner.loop();

  // check battery
  battery.loop();
  if (BATTERY_ENABLED && doneAnimating && (now - lastBatteryCheck) > 60000) {
    lastBatteryCheck = now;
    int state = battery.getState();
    switch (state) {
      case ESPBATTERY_FULL:
        banner.setMessage("Battery is full");
        break;
      case ESPBATTERY_CHARGING:
        banner.setMessage("Battery is charging");
        break;
      case ESPBATTERY_LOW:
        banner.setMessage("Battery is getting low");
        break;
      case ESPBATTERY_CRITICAL:
        banner.setMessage("Battery is critically low");
        break;
      case ESPBATTERY_OK:
        // pass
        break;
    }
    if (state != ESPBATTERY_OK) {
         return;
    }
  }

  // wifi manager stuff
  if (WiFi.status() != WL_CONNECTED && !wifiManager.process()) {
    if (doneAnimating) {
      banner.setMessage("Connect to \"" AP_NAME "\" wireless network to configure me");
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
