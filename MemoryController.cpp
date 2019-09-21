#include "MemoryController.h"

MemoryController::MemoryController()
{
    
}

void MemoryController::begin(RopeMemory *ropeMemory)
{
    memset(this->ram, 0, 48);
    this->ropeMemory = ropeMemory;
}

bool MemoryController::isRopeMemoryOn()
{
    return (EEPROM.read(MEM_CONFIG) & 0x80) == 0;
}

bool MemoryController::isEEPROMOn()
{
    return (EEPROM.read(MEM_CONFIG) & 0x40) == 0;
}

uint8_t MemoryController::read(uint8_t address)
{
    if (this->isRopeMemoryOn() && address < 4)
    {
        return this->ropeMemory->readNibble(2 * address) << 4 | this->ropeMemory->readNibble(1 + (2 * address));
    }

    if (address <= 0x1F && this->isEEPROMOn())
    {
        return EEPROM.read(address);
    }

    if (address <= 0x2F)
    {
        return this->ram[address];
    }

    return 0;
}

void MemoryController::write(uint8_t address, uint8_t data)
{
    if (address <= 0x2F)
    {
        this->ram[address] = data;    
    }

    if (address == MEM_CONFIG ||  (address <= 0x1F && this->isEEPROMOn()))
    {
        EEPROM.write(address, data);        
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
