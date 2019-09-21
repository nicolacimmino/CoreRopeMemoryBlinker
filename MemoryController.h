#ifndef __MEMORY_CONTROLLER_HEADER__
#define __MEMORY_CONTROLLER_HEADER__

#include <EEPROM.h>
#include "RopeMemory.h"

#define MEM_CONFIG 10
#define MEM_STATUS 11

class MemoryController
{
public:
    uint8_t readMemory(uint8_t address);
    void writeMemory(uint8_t address, uint8_t data);

private:
    bool isCoreRopeOn();
    RopeMemory ropeMemory;
};

#endif
