
//  Copyright (C) 2019 Nicola Cimmino
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see http://www.gnu.org/licenses/.
//

#define GREEN_LED_PIN 3
#define RED_LED_PIN 12

#define MEM_CONFIG 10
#define MEM_STATUS 11

#include <EEPROM.h>
#include "Cli.h"
#include "RopeMemory.h"

Cli *cli;
RopeMemory ropeMemory;

bool isCoreRopeOn()
{
    return (EEPROM.read(MEM_CONFIG) & 0x80) == 0;
}

uint8_t readMemory(uint8_t address)
{
    if (isCoreRopeOn() && address < 4)
    {
        return ropeMemory.readNibble(2 * address) << 4 | ropeMemory.readNibble(1 + (2 * address));
    }

    return EEPROM.read(address);
}

void writeMemory(uint8_t address, uint8_t data)
{
    EEPROM.write(address, data);
}

void dumpMemoryCommand(uint8_t from, uint8_t to)
{
    for (uint16_t address = from; address <= to; address++)
    {
        if ((address % 16) == 0)
        {
            cli->printHexByte(address + 1);
            Serial.print(" - ");
            continue;
        }

        cli->printHexByte(readMemory(address));

        Serial.print(((address % 16) == 15) ? "\n" : ".");
    }
    Serial.println("");
}

void readMemoryCommand(uint8_t address)
{
    cli->printHexByte(readMemory(address));
    Serial.println("");
}

void writeMemoryCommand(uint8_t address, uint8_t data)
{
    writeMemory(address, data);
}

void onCommand(uint8_t argc, char **argv)
{
    if (strcmp(argv[0], "exit") == 0)
    {
        Serial.println("bye");
        return;
    }

    if (strcmp(argv[0], "dump") == 0 || strcmp(argv[0], "d") == 0)
    {
        dumpMemoryCommand(atoi(argv[1]), atoi(argv[2]));
        return;
    }

    if (strcmp(argv[0], "read") == 0 || strcmp(argv[0], "r") == 0)
    {
        readMemoryCommand(atoi(argv[1]));
        return;
    }

    if (strcmp(argv[0], "write") == 0 || strcmp(argv[0], "w") == 0)
    {
        writeMemoryCommand(atoi(argv[1]), atoi(argv[2]));
        return;
    }
}

void setup()
{
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);

    Serial.begin(115200);

    cli = new Cli();
    cli->begin(&Serial, onCommand);

    analogReference(INTERNAL);
}

void loop()
{
    cli->loop();

    uint32_t value32 = 0;

    for (int address = 0; address < 4; address++)
    {
        uint8_t value = readMemory(address);

        value32 = (value32 << 8) | value;

        // Keep breathing! See Sean Voisen great post from which I grabbed the formula.
        // https://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
        float val = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;

        analogWrite(GREEN_LED_PIN, val);
    }

    if (value32 != 0xF05A4E43)
    {
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, HIGH);
        EEPROM.write(MEM_STATUS, EEPROM.read(MEM_STATUS) | 1);

        while (EEPROM.read(MEM_STATUS) & 1 != 0)
        {
            cli->loop();
        }

        digitalWrite(RED_LED_PIN, LOW);
    }
}
