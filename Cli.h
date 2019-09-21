#ifndef __CLI_HEADER__
#define __CLI_HEADER__

#define CLI_MAX_ARGC 4
#define CLI_MAX_ARGLEN 16
#define CLI_INPUT_BUFFER_SIZE 1 + (CLI_MAX_ARGC * CLI_MAX_ARGLEN)

#include "Arduino.h"
#include "MemoryController.h"

class Cli
{
public:
    void begin(Stream *stream, MemoryController *memoryController);
    void loop();
    
private:
    Stream *stream;
    void parseCommand(String command);
    char *argv[CLI_MAX_ARGC];
    uint8_t argc;
    char inputBuffer[CLI_INPUT_BUFFER_SIZE];
    uint8_t inputBufferIndex = 0;
    void initParser();
    MemoryController *memoryController;
    void onCommand(uint8_t argc, char **argv);
    void dumpMemoryCommand(uint8_t from, uint8_t to);
    void readMemoryCommand(uint8_t address);
    void writeMemoryCommand(uint8_t address, uint8_t data);
    void copyMemoryCommand(uint8_t from, uint8_t to, uint8_t destination);
    void printPrompt();
    void printHexByte(uint8_t value, bool appendNewLine = false);
    uint8_t argToByte(char *arg);
    bool promptNeeded = false;
};

#endif