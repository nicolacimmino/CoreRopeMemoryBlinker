#ifndef __ROPE_MEMORY_HEADER__
#define __ROPE_MEMORY_HEADER__

#include "Arduino.h"

#define DRIVE_COMMON_PIN 7

class RopeMemory
{
public:
    uint8_t readNibble(uint8_t address);

private:
    uint8_t drivePins[8] = {2, 4, 5, 6, 8, 9, 10, 11};
    uint8_t sensePins[4] = {A0, A1, A2, A3};

    void dischargeSenseCapacitors();
};

#endif
