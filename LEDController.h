
#ifndef __LED_CONTROLLER_HEADER__
#define __LED_CONTROLLER_HEADER__

#include "Arduino.h"
#include <FastLED.h>
#include "MemoryController.h"

#define NUM_LEDS 9
#define MEM_LED_BRIGHTNESS 12

class LEDController
{
public:
    void begin(uint8_t redPin, uint8_t greenPin, MemoryController *memoryController);
    void loop();
    void showSolidRed();
    void breathe();

private:
    uint8_t redPin;
    uint8_t greenPin;
    bool solidRed = false;
    CRGB leds[NUM_LEDS];
    MemoryController *memoryController;
};

#endif