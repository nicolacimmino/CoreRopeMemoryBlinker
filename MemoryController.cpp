#include "MemoryController.h"

void MemoryController::begin(RopeMemory *ropeMemory)
{
    this->ropeMemory = ropeMemory;
}

bool MemoryController::isCoreRopeOn()
{
    return (EEPROM.read(MEM_CONFIG) & 0x80) == 0;
}

uint8_t MemoryController::read(uint8_t address)
{
    if (this->isCoreRopeOn() && address < 4)
    {
        return this->ropeMemory->readNibble(2 * address) << 4 | this->ropeMemory->readNibble(1 + (2 * address));
    }

    return EEPROM.read(address);
}

void MemoryController::write(uint8_t address, uint8_t data)
{
    EEPROM.write(address, data);
}

bool MemoryController::isRopeMemoryOK()
{
    return (this->read(MEM_STATUS) & 1) == 0;
}

void MemoryController::setRopeMemoryNOK()
{
    EEPROM.write(MEM_STATUS, EEPROM.read(MEM_STATUS) | 1);    
}
