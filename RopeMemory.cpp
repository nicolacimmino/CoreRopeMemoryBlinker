#include "RopeMemory.h"

void RopeMemory::begin(uint8_t driveCommonPin, uint8_t *drivePins, uint8_t *sensePins)
{
    this->driveCommonPin = driveCommonPin;
    this->drivePins = drivePins;
    this->sensePins = sensePins;
}

uint8_t RopeMemory::readNibble(uint8_t address)
{
    uint8_t values[] = {0, 0, 0, 0};

    this->dischargeSenseCapacitors();

    pinMode(this->drivePins[address], OUTPUT);
    pinMode(this->driveCommonPin, OUTPUT);

    // Give a couple of whacks to the drive line so we induce
    // current in the sense coils of the set bits.
    // Here we energize the drive line in both polarizations
    // alternatively. This showed to give the best results
    // regardless of orientation of the sense coils versus
    // the drive lines.
    for (uint8_t phase = 0; phase < 4; phase++)
    {
        digitalWrite(this->drivePins[address], phase % 2);
        digitalWrite(this->driveCommonPin, (phase + 1) % 2);
        for (uint8_t ix = 0; ix < 4; ix++)
        {
            values[ix] += analogRead(this->sensePins[ix]);
        }
    }

    digitalWrite(this->drivePins[address], LOW);
    digitalWrite(this->driveCommonPin, LOW);

    pinMode(this->drivePins[address], INPUT);
    pinMode(this->driveCommonPin, INPUT);

    // We consider a bit set if we got anything on the sense line.
    uint8_t result = 0;
    for (uint8_t ix = 0; ix < 4; ix++)
    {
        result = (result << 1) | ((values[ix] > 1) ? 1 : 0);
    }

    return result;
}

void RopeMemory::dischargeSenseCapacitors()
{
    for (uint8_t ix = 0; ix < 4; ix++)
    {
        pinMode(this->sensePins[ix], OUTPUT);
        digitalWrite(this->sensePins[ix], LOW);
        delay(1);
        pinMode(this->sensePins[ix], INPUT);
    }
}