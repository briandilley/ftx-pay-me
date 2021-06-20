
#include "Settings.h"

const char* DEFAULT_MESSAGE = "(___)__)===jjjj===D ~  ~   ~";

Settings* Settings::instance = nullptr;

Settings::Settings() {
    load();
    if (settings.valid != 'v') {
        loadDefaults();
        save();
    }
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
        return true;
    } else {
        return false;
    }
}

void Settings::loadDefaults() {
    settings.valid = 'v';
    settings.fttTokens = 0;
    settings.btc = true;
    settings.eth = true;
    settings.scrollSpeed = 40;
    settings.configured = false;
    settings.valid = 'v';
    settings.dataUpdateFrequencySeconds = 10;
    strcpy(settings.message, DEFAULT_MESSAGE);
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
