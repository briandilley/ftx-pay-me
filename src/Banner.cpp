
#include "Banner.h"

Banner::Banner(MD_MAX72XX::moduleType_t hardwareType, int csPin, int maxDevices, int bufferSize, int scrollSpeed) :
    hw(hardwareType, csPin, maxDevices),
    buffer(new char[bufferSize]),
    pendingBuffer(new char[bufferSize]),
    bufferSize(bufferSize),
    scrollSpeed(scrollSpeed) {
    strcpy(buffer, "FTX PAY ME");
    strcpy(pendingBuffer, "");
}

void Banner::setup() {
    hw.begin();
    hw.displayClear();
    hw.displaySuspend(false);
    hw.setSpeed(scrollSpeed);
    hw.displayScroll(buffer, PA_LEFT, PA_SCROLL_LEFT, scrollSpeed);
}

void Banner::setMessage(std::string message) {
    memcpy(pendingBuffer, message.c_str(), min((int)message.length(), bufferSize));
}

bool Banner::loop() {
    bool ret = hw.displayAnimate();
    if (ret) {
        if (strlen(pendingBuffer) > 0) {
            memcpy(buffer, pendingBuffer, bufferSize);
            memset(pendingBuffer, 0, bufferSize);
        }
        hw.displayReset();
        hw.setSpeed(scrollSpeed);
    }
    return ret;
}

void Banner::setScrollSpeed(int speed) {
    scrollSpeed = speed;
    hw.setSpeed(scrollSpeed);
}