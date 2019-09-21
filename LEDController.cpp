#include "LEDController.h"

void LEDController::begin(uint8_t redPin, uint8_t greenPin)
{
    this->redPin = redPin;
    this->greenPin = greenPin;
}

void LEDController::loop()
{
    digitalWrite(this->redPin, this->solidRed);

    if (this->solidRed)
    {
        digitalWrite(this->greenPin, LOW);
        return;
    }

    // Keep breathing! See Sean Voisen great post from which I grabbed the formula.
    // https://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
    float val = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;

    analogWrite(this->greenPin, val);
}

void LEDController::showSolidRed()
{
    this->solidRed = true;
}

void LEDController::breathe()
{
    this->solidRed = false;
}