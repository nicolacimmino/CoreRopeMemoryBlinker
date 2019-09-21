#include "CommandsProcessor.h"

void CommandsProcessor::begin(Cli *cli, MemoryController *memoryController)
{
    this->cli = cli;
    this->memoryController = memoryController;
}

void CommandsProcessor::dumpMemoryCommand(uint8_t from, uint8_t to)
{
    for (uint16_t offset = 0; offset <= to - from; offset++)
    {
        if ((offset % 16) == 0)
        {
            this->cli->printHexByte(from + offset);
            this->cli->stream->print(" - ");
        }

        this->cli->printHexByte(memoryController->read(from + offset));

        this->cli->stream->print(((offset % 16) == 15) ? "\n" : ".");
    }
    this->cli->stream->println("");
}

void CommandsProcessor::readMemoryCommand(uint8_t address)
{
    this->cli->printHexByte(memoryController->read(address), true);
}

void CommandsProcessor::writeMemoryCommand(uint8_t address, uint8_t data)
{
    memoryController->write(address, data);
}

void CommandsProcessor::onCommand(uint8_t argc, char **argv)
{
    if (strcmp(argv[0], "exit") == 0)
    {
        this->cli->stream->println("bye");
        return;
    }

    if (strcmp(argv[0], "dump") == 0 || strcmp(argv[0], "d") == 0)
    {
        this->dumpMemoryCommand(this->argToByte(argv[1]), this->argToByte(argv[2]));
        return;
    }

    if (strcmp(argv[0], "read") == 0 || strcmp(argv[0], "r") == 0)
    {
        this->readMemoryCommand(this->argToByte(argv[1]));
        return;
    }

    if (strcmp(argv[0], "write") == 0 || strcmp(argv[0], "w") == 0)
    {
        this->writeMemoryCommand(this->argToByte(argv[1]), this->argToByte(argv[2]));
        return;
    }
}

uint8_t CommandsProcessor::argToByte(char *arg)
{
    if (arg[0] == '$')
    {
        return strtol(arg + 1, NULL, 16);
    }

    return atoi(arg);
}
