
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
#include "LEDController.h"

Cli cli;
MemoryController memoryController;
CommandsProcessor commandProcessor;
LEDController ledController;

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

    ledController.begin(RED_LED_PIN, GREEN_LED_PIN);

    analogReference(INTERNAL);
}

void loop()
{
    cli.loop();
    ledController.loop();

    if (memoryController.read(MEM_STATUS) & 1 != 0)
    {
        return;
    }

    ledController.breathe();

    uint32_t value32 = 0;
    for (int address = 0; address < 4; address++)
    {
        uint8_t value = memoryController.read(address);
        value32 = (value32 << 8) | value;
    }

    if (value32 != 0xF05A4E43)
    {
        ledController.showSolidRed();
        memoryController.write(MEM_STATUS, memoryController.read(MEM_STATUS) | 1);
    }
}
