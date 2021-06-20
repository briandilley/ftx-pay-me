
#ifndef SETTINGS_H
#define SETTINGS_H

#include <EEPROM.h>

struct FtxPayMeSettings {
    char valid;
    int fttTokens;
    bool btc;
    bool eth;
    int scrollSpeed;
    char message[128];
    bool configured;
};

class Settings {
public:

    static Settings* getInstance();

    FtxPayMeSettings* get();

    void save();
    bool load();
    void clear();

private:
    static Settings* instance;
    Settings();
    FtxPayMeSettings settings;
};


#endif
