
#include "Settings.h"

Settings* Settings::instance = nullptr;

Settings::Settings() {
    load();
}

Settings* Settings::getInstance() {
    if (Settings::instance == nullptr) {
        Settings::instance = new Settings();
    }
    return Settings::instance;
}

bool Settings::load() {
    EEPROM.begin(sizeof(FtxPayMeSettings));
    if (EEPROM.length() >= 0) {
        EEPROM.get(0, settings);
        if (settings.version != CURRENT_SETTINGS_VERSION) {
            loadDefaults();
            save();
        }
        return true;
    } else {
        return false;
    }
}

void Settings::loadDefaults() {
    settings.version = CURRENT_SETTINGS_VERSION;
    settings.fttTokens = 0;
    settings.enablePayoutDisplay = false;
    settings.enablePersonalMessageDisplay = false;
    settings.btc = true;
    settings.eth = true;
    settings.scrollSpeed = 40;
    settings.dataUpdateFrequencySeconds = 10;
    strcpy(settings.message, DEFAULT_MESSAGE);
    settings.configured = false;
}

void Settings::save() {
    EEPROM.put(0, settings);
    EEPROM.commit();
}

void Settings::clear() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

FtxPayMeSettings* Settings::get() {
    return &settings;
}
