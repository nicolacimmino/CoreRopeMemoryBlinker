#include "RopeMemory.h"

uint8_t RopeMemory::readNibble(uint8_t address)
{
    uint8_t values[] = {0, 0, 0, 0};

    this->dischargeSenseCapacitors();

    pinMode(this->drivePins[address], OUTPUT);
    pinMode(DRIVE_COMMON_PIN, OUTPUT);

    // Give a couple of whacks to the drive line so we induce
    // current in the sense coils of the set bits.
    // Here we energize the drive line in both polarizations
    // alternatively. This showed to give the best results
    // regardless of orientation of the sense coils versus
    // the drive lines.
    for (uint8_t phase = 0; phase < 4; phase++)
    {
        digitalWrite(this->drivePins[address], phase % 2);
        digitalWrite(DRIVE_COMMON_PIN, (phase + 1) % 2);
        for (uint8_t ix = 0; ix < 4; ix++)
        {
            values[ix] += analogRead(this->sensePins[ix]);
        }
    }

    digitalWrite(this->drivePins[address], LOW);
    digitalWrite(DRIVE_COMMON_PIN, LOW);

    pinMode(this->drivePins[address], INPUT);
    pinMode(DRIVE_COMMON_PIN, INPUT);

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