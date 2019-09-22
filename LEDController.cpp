#include "LEDController.h"

void LEDController::begin(uint8_t redPin, uint8_t greenPin, MemoryController *memoryController)
{
    this->redPin = redPin;
    this->greenPin = greenPin;
    this->memoryController = memoryController;

    FastLED.addLeds<WS2812B, 13, GRB>(this->leds, NUM_LEDS);
}

void LEDController::loop()
{
    digitalWrite(this->redPin, this->solidRed);

    FastLED.setBrightness(this->memoryController->read(MEM_LED_BRIGHTNESS));

    if (this->solidRed)
    {
        if ((millis() % 110) == 0)
        {
            for (int ix = 0; ix < NUM_LEDS; ix++)
            {
                this->leds[ix] = (ix % 2 == 0) ? CRGB::DarkOrange : CRGB::DarkRed;
                this->leds[ix].fadeToBlackBy(random(120) + 135);
            }
            FastLED.show();
        }

        digitalWrite(this->greenPin, LOW);
        return;
    }

    // Keep breathing! See Sean Voisen great post from which I grabbed the formula.
    // https://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
    float val = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;

    analogWrite(this->greenPin, val);

    for (int ix = 0; ix < NUM_LEDS; ix++)
    {
        this->leds[ix] = CRGB::DarkGreen;
        this->leds[ix].fadeToBlackBy(255 - val);
    }
    FastLED.show();
}

void LEDController::showSolidRed()
{
    this->solidRed = true;
}

void LEDController::breathe()
{
    this->solidRed = false;
}