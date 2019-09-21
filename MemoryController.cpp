#include "MemoryController.h"

bool MemoryController::isCoreRopeOn()
{
    return (EEPROM.read(MEM_CONFIG) & 0x80) == 0;
}

uint8_t MemoryController::readMemory(uint8_t address)
{
    if (this->isCoreRopeOn() && address < 4)
    {
        return this->ropeMemory.readNibble(2 * address) << 4 | this->ropeMemory.readNibble(1 + (2 * address));
    }

    return EEPROM.read(address);
}

void MemoryController::writeMemory(uint8_t address, uint8_t data)
{
    EEPROM.write(address, data);
}
