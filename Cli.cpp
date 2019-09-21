#include "Cli.h"

void Cli::begin(Stream *stream, MemoryController *memoryController)
{
    this->stream = stream;
    this->memoryController = memoryController;

    this->initParser();
}

void Cli::initParser()
{
    memset(this->inputBuffer, 0, CLI_INPUT_BUFFER_SIZE);

    this->argc = 0;
    this->inputBufferIndex = 0;
    this->argv[0] = this->inputBuffer;
}

void Cli::loop()
{
    if(this->promptNeeded) {
        this->printPrompt();
        return;
    }
    
    while (this->stream->available())
    {
        this->inputBuffer[this->inputBufferIndex] = (char)this->stream->read();
        this->stream->print(this->inputBuffer[this->inputBufferIndex]);

        if (this->inputBuffer[this->inputBufferIndex] == ' ')
        {
            this->argc++;
            this->inputBuffer[this->inputBufferIndex] = 0;
            this->argv[this->argc] = &this->inputBuffer[this->inputBufferIndex + 1];
        }

        this->inputBufferIndex++;

        if (this->inputBufferIndex > CLI_INPUT_BUFFER_SIZE)
        {
            this->inputBufferIndex = 0;
            return;
        }
    }

    if (this->inputBuffer[this->inputBufferIndex - 1] == '\r')
    {
        this->inputBuffer[this->inputBufferIndex - 1] = 0;

        this->stream->println("");
        this->argc++;

        this->onCommand(this->argc, this->argv);
        this->promptNeeded = true;
        this->initParser();
    }
}

void Cli::printHexByte(uint8_t value, bool appendNewLine = false)
{
    this->stream->print("0123456789ABCDEF"[value >> 4]);
    this->stream->print("0123456789ABCDEF"[value & 15]);

    if (appendNewLine)
    {
        this->stream->println("");
    }
}

void Cli::printPrompt()
{
    this->stream->print("RMERR:");
    this->stream->print(this->memoryController->isRopeMemoryOK() ? 0 : 1);
    this->stream->print(" RMON:");
    this->stream->println(this->memoryController->isRopeMemoryOn() ? 1 : 0);
    
    this->stream->print(">");

    this->promptNeeded = false;
}

void Cli::dumpMemoryCommand(uint8_t from, uint8_t to)
{
    for (uint16_t offset = 0; offset <= to - from; offset++)
    {
        if ((offset % 16) == 0)
        {
            this->printHexByte(from + offset);
            this->stream->print(" - ");
        }

        this->printHexByte(memoryController->read(from + offset));

        this->stream->print(((offset % 16) == 15) ? "\n" : ".");
    }
    this->stream->println("");
}

void Cli::readMemoryCommand(uint8_t address)
{
    this->printHexByte(memoryController->read(address), true);
}

void Cli::writeMemoryCommand(uint8_t address, uint8_t data)
{
    memoryController->write(address, data);
}

void Cli::onCommand(uint8_t argc, char **argv)
{
    if (strcmp(argv[0], "exit") == 0)
    {
        this->stream->println("bye");
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

uint8_t Cli::argToByte(char *arg)
{
    if (arg[0] == '$')
    {
        return strtol(arg + 1, NULL, 16);
    }

    return atoi(arg);
}
