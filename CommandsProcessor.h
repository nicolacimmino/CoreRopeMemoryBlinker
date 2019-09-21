
#ifndef __COMMANDS_PROCESSOR_HEADER__
#define __COMMANDS_PROCESSOR_HEADER__

#include "Cli.h"
#include "MemoryController.h"

class CommandsProcessor
{
public:
    void begin(Cli *cli, MemoryController *memoryController);
    void onCommand(uint8_t argc, char **argv);

private:
    Cli *cli;
    MemoryController *memoryController;
    void dumpMemoryCommand(uint8_t from, uint8_t to);
    void readMemoryCommand(uint8_t address);
    void writeMemoryCommand(uint8_t address, uint8_t data);    
    uint8_t argToByte(char *arg);
};

#endif