
#ifndef BANNER_H
#define BANNER_H

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <string>

class Banner {
public:
    Banner(MD_MAX72XX::moduleType_t hardwareType, int csPin, int maxDevices, int bufferSize, int scrollSpeed);

    void setup();
    bool loop();

    void setScrollSpeed(int speed);
    void setMessage(const char* message);
private:
    MD_Parola hw;
    char* buffer;
    char* pendingBuffer;
    const int bufferSize;
    int scrollSpeed;
};

#endif
