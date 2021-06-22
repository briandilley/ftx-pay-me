
#ifndef SETTINGS_H
#define SETTINGS_H

#include <EEPROM.h>

#define DEFAULT_MESSAGE "(___)__)===jjjj===D ~  ~   ~"
#define CURRENT_SETTINGS_VERSION '2'

struct FtxPayMeSettings {
    char version;
    int fttTokens;
    bool enablePayoutDisplay;
    bool btc;
    bool eth;
    int scrollSpeed;
    unsigned int dataUpdateFrequencySeconds;
    bool enablePersonalMessageDisplay;
    char message[128];
    bool configured;
};

class Settings {
public:

    static Settings* getInstance();

    FtxPayMeSettings* get();

    void save();
    bool load();
    void loadDefaults();
    void clear();

private:
    static Settings* instance;
    Settings();
    FtxPayMeSettings settings;
};


#endif
