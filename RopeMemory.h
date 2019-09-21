#ifndef __ROPE_MEMORY_HEADER__
#define __ROPE_MEMORY_HEADER__

#include "Arduino.h"

class RopeMemory
{
public:
    void begin(uint8_t driveCommonPin, uint8_t *drivePins, uint8_t *sensePins);
    uint8_t readNibble(uint8_t address);
        
private:
    uint8_t *drivePins;
    uint8_t *sensePins;
    uint8_t driveCommonPin;

    void dischargeSenseCapacitors();
};

#endif
