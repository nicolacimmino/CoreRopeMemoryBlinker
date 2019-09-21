
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

#include "Hardware.h"
#include "Cli.h"
#include "MemoryController.h"
#include "CommandsProcessor.h"
#include "LEDController.h"
#include "RopeMemory.h"

Cli cli;
MemoryController memoryController;
CommandsProcessor commandProcessor;
LEDController ledController;
RopeMemory ropeMemory;

uint8_t drivePins[] = {PIN_DRIVE_0, PIN_DRIVE_1, PIN_DRIVE_2, PIN_DRIVE_3, PIN_DRIVE_4, PIN_DRIVE_5, PIN_DRIVE_6, PIN_DRIVE_7};
uint8_t sensePins[] = {PIN_SENSE_0, PIN_SENSE_1, PIN_SENSE_2, PIN_SENSE_3};

void onCommand(uint8_t argc, char **argv)
{
    commandProcessor.onCommand(argc, argv);
    cli.printPrompt();
}

void setup()
{
    pinMode(PIN_RED_LED, OUTPUT);
    pinMode(PIN_RED_LED, OUTPUT);

    Serial.begin(115200);

    ropeMemory.begin(DRIVE_COMMON_PIN, drivePins, sensePins);
    memoryController.begin(&ropeMemory);

    cli.begin(&Serial, onCommand);
    commandProcessor.begin(&cli, &memoryController);

    ledController.begin(PIN_RED_LED, PIN_GREEN_LED);

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
