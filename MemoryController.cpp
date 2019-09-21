#include "MemoryController.h"

void MemoryController::begin(RopeMemory *ropeMemory)
{
    this->ropeMemory = ropeMemory;
}

bool MemoryController::isRopeMemoryOn()
{
    return (EEPROM.read(MEM_CONFIG) & 0x80) == 0;
}

uint8_t MemoryController::read(uint8_t address)
{
    if (this->isRopeMemoryOn() && address < 4)
    {
        return this->ropeMemory->readNibble(2 * address) << 4 | this->ropeMemory->readNibble(1 + (2 * address));
    }

    if (address <= 0x1F)
    {
        return EEPROM.read(address);
    }

    if (address <= 0x2F)
    {
        return this->ram[address - 0x20];
    }

    return 0;
}

void MemoryController::write(uint8_t address, uint8_t data)
{
    if (address <= 0x1F)
    {
        EEPROM.write(address, data);
    }

    if (address <= 0x2F)
    {
        this->ram[address - 0x20] = data;
    }
}

bool MemoryController::isRopeMemoryOK()
{
    return (this->read(MEM_STATUS) & 1) == 0;
}

void MemoryController::setRopeMemoryNOK()
{
    EEPROM.write(MEM_STATUS, EEPROM.read(MEM_STATUS) | 1);
}
