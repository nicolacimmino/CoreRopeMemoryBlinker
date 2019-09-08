
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
#define DRIVE_COMMON_PIN 7

uint8_t drivePins[] = {2, 4, 5, 6, 8, 9, 10, 11};
uint8_t sensePins[] = {A0, A1, A2, A3};

void setup()
{
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);

    Serial.begin(115200);
    analogReference(INTERNAL);
}


uint8_t readNibble(uint8_t address)
{
    uint8_t A = drivePins[address];
    uint8_t B = DRIVE_COMMON_PIN;
    uint8_t values[] = {0, 0, 0, 0};

    // Discharge the sense capacitors.
    for (uint8_t ix = 0; ix < 4; ix++)
    {
        pinMode(sensePins[ix], OUTPUT);
        digitalWrite(sensePins[ix], LOW);
        delay(1);
        pinMode(sensePins[ix], INPUT);
    }

    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);

    // Give a couple of whacks to the drive line so we induce 
    // current in the sense coils of the set bits. 
    // Here we energize the drive line in both polarizations
    // alternatively. This showed to give the best results
    // regardless of orientation of the sense coils versus
    // the drive lines.
    for (uint8_t phase = 0; phase < 4; phase++)
    {
        digitalWrite(A, phase % 2);
        digitalWrite(B, (phase + 1) % 2);
        for (uint8_t ix = 0; ix < 4; ix++)
        {
            values[ix] += analogRead(sensePins[ix]);
        }
    }

    digitalWrite(A, LOW);
    digitalWrite(B, LOW);

    pinMode(A, INPUT);
    pinMode(B, INPUT);

    // We consider a bit set if we got anything on the sense line.
    uint8_t result = 0;
    for (uint8_t ix = 0; ix < 4; ix++)
    {
        result = (result << 1) | ((values[ix] > 1) ? 1 : 0);
    }

    return result;
}

void loop()
{
    uint32_t value32 = 0;

    for (int address = 0; address < 8; address++)
    {
        if (address > 0 && address % 2 == 0)
        {
            Serial.print(".");
        }

        uint8_t nibble = readNibble(address);

        value32 = (value32 << 4) | nibble;

        Serial.print(nibble, HEX);

        // Keep breathing! See Sean Voisen great post from which I grabbed the formula.
        // https://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
        float val = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;
        analogWrite(GREEN_LED_PIN, val);
    }

    Serial.println("");

    if (value32 != 0xF05A4E43)
    {
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, HIGH);
        Serial.println("Memory read error! I'm dead.");
        while (true)
        {
        }
    }
}