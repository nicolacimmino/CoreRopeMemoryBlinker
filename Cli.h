#ifndef __CLI_HEADER__
#define  __CLI_HEADER__

#define CLI_MAX_ARGC 3
#define CLI_MAX_ARGLEN 16
#define CLI_INPUT_BUFFER_SIZE 1+(CLI_MAX_ARGC*CLI_MAX_ARGLEN)

class Cli
{
public:
    void begin(Stream *stream, void (*onCommand)(uint8_t, char**));
    void loop();

private:
    Stream *stream;
    void (*onCommand)(uint8_t argc, char **argv);
    void parseCommand(String command);
    char *argv[CLI_MAX_ARGC];
    uint8_t argc;    
    char inputBuffer[CLI_INPUT_BUFFER_SIZE];
    uint8_t inputBufferIndex = 0;
    void initParser();
};

#endif