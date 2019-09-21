
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

#include "Cli.h"
#include "MemoryController.h"
#include "CommandsProcessor.h"

Cli cli;
MemoryController memoryController;
CommandsProcessor commandProcessor;

void onCommand(uint8_t argc, char **argv)
{    
    commandProcessor.onCommand(argc, argv);
    cli.stream->print(">");
}

void setup()
{
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);

    Serial.begin(115200);

    cli.begin(&Serial, onCommand);
    commandProcessor.begin(&cli, &memoryController);

    analogReference(INTERNAL);
}

void loop()
{
    cli.loop();

    uint32_t value32 = 0;

    for (int address = 0; address < 4; address++)
    {
        uint8_t value = memoryController.read(address);

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
        memoryController.write(MEM_STATUS, memoryController.read(MEM_STATUS) | 1);

        while (memoryController.read(MEM_STATUS) & 1 != 0)
        {
            cli.loop();
        }

        digitalWrite(RED_LED_PIN, LOW);
    }
}
