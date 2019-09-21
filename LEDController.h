
#ifndef __LED_CONTROLLER_HEADER__
#define __LED_CONTROLLER_HEADER__

#include "Arduino.h"

class LEDController
{
public:
    void begin(uint8_t redPin, uint8_t greenPin);
    void loop();
    void showSolidRed();
    void breathe();

private:
    uint8_t redPin;
    uint8_t greenPin;
    bool solidRed = false;    
};

#endif