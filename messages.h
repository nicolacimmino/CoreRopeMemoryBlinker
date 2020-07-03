#ifndef __ROPEMEM_MESSAGES_H__
#define __ROPEMEM_MESSAGES_H__

const char message0[] PROGMEM =
    "\e[33m"
    " ______                ___  ___                    \r\n"
    " | ___ \\               |  \\/  |                     \e[32mMeory:\e[33m 4-nibbles ROM\r\n"
    " | |_/ /___  _ __   ___| .  . | ___ _ __ ___        \e[32mS/N:\e[33m 202006-003\r\n"
    " |    // _ \\| '_ \\ / _ \\ |\\/| |/ _ \\ '_ ` _ \\       \e[32mAssembled:\e[33m 2020-06-30\r\n"
    " | |\\ \\ (_) | |_) |  __/ |  | |  __/ | | | | |      \e[32mBuilt:\e[33m " __DATE__ " " __TIME__ "\r\n"
    " \\_| \\_\\___/| .__/ \\___\\_|  |_/\\___|_| |_| |_|      \r\n"
    "            | |                \e[32m(c) Nicola 2020\r\n\e[33m"
    "            |_|                                \r\n";

const char *const messages[] PROGMEM = {message0};

void printMessage(uint8_t messageId)
{
    char buffer[612];
    strcpy_P(buffer, (char *)pgm_read_word(&(messages[messageId])));
    Serial.print(buffer);
}

#endif