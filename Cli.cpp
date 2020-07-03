#include "Cli.h"

void Cli::begin(Stream *stream, MemoryController *memoryController)
{
    this->stream = stream;
    this->memoryController = memoryController;

    this->initParser();
    this->printStatusBar();
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
    if (this->promptNeeded)
    {
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
    VT100.setCursor(12, 2);
    this->stream->print("RMERR:");
    this->stream->print(this->memoryController->isRopeMemoryOK() ? 0 : 1);
    this->stream->print(" RMON:");
    this->stream->print(this->memoryController->isRopeMemoryOn() ? 1 : 0);
    this->stream->print(" EEON:");
    this->stream->println(this->memoryController->isEEPROMOn() ? 1 : 0);

    this->stream->print(" >");
    VT100.clearLineAfter();
    VT100.setCursor(13, 3);

    this->promptNeeded = false;
}

void Cli::dumpMemoryCommand(uint8_t from, uint8_t to)
{
    from = min(from, 64);
    to = min(to, 64);

    for (uint16_t offset = 0; offset <= to - from; offset++)
    {
        if ((offset % 16) == 0)
        {
            this->stream->print(" ");
            this->printHexByte(from + offset);
            this->stream->print(" - ");
        }

        this->printHexByte(memoryController->read(from + offset));

        this->stream->print(((offset % 16) == 15) ? "\r\n" : ".");
    }
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
    for(uint8_t line=13; line < 20; line++) {
        VT100.setCursor(line, 1);
        VT100.clearLineAfter();
    }
    VT100.setCursor(15, 1);

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

void Cli::printStatusBar()
{
    VT100.setBackgroundColor(VT_BLACK);
    VT100.clearScreen();

    VT100.setCursor(2, 1);
    printMessage(0);

    VT100.setCursor(1, 1);
    VT100.setBackgroundColor(VT_YELLOW);
    VT100.setTextColor(VT_BLACK);
    Serial.print(" CoreRopeMemory V1.0 ");

    VT100.clearLineAfter();
    VT100.setCursor(1, TERMINAL_WIDTH + 1);
    VT100.setBackgroundColor(VT_BLACK);
    VT100.setTextColor(VT_WHITE);
    VT100.clearLineAfter();

    VT100.cursorOn();
}
