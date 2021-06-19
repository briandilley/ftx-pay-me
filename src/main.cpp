
#include "WiFiController.h"
#include "WebConfigurationPortal.h"

/* ------------------------------------------------- */
#define PORTAL_TIMEOUT  10 * 60 // seconds
#define AP_NAME         "FTX PAY ME"
#define AP_PASSWORD     ""
/* ------------------------------------------------- */

WiFiController wifiController(AP_NAME, AP_PASSWORD, PORTAL_TIMEOUT);
WebConfigurationPortal webConfigurationPortal;

void setup() {
  wifiController.setup();
}

void loop() {

  if (!wifiController.isConnected()) {
    return;
  }

  if (!webConfigurationPortal.isReady()) {
    webConfigurationPortal.setup();
  }

  webConfigurationPortal.loop();
}
