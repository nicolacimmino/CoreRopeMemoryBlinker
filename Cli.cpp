#include "Cli.h"

void Cli::begin(Stream *stream, void (*onCommand)(uint8_t, char **))
{
    this->stream = stream;
    this->onCommand = onCommand;

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

        this->initParser();
    }
}

void Cli::printHexByte(uint8_t value)
{
    this->stream->print("0123456789ABCDEF"[value >> 4]);
    this->stream->print("0123456789ABCDEF"[value & 15]);
}
