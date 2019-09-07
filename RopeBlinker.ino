#include <CRC32.h>
CRC32 crc;

uint8_t rowPins[] = {3, 4, 5, 6};
uint8_t colPins[] = {7, 8, 9, 10};
uint8_t sensePins[] = {A0, A1, A2, A3};

void setup()
{
    for (uint8_t ix = 0; ix < 4; ix++)
    {
        pinMode(sensePins[ix], INPUT);
    }

    Serial.begin(9600);
    analogReference(INTERNAL);
}

uint8_t readNibble(uint8_t address)
{
    uint8_t A = colPins[address & 0b11];
    uint8_t B = rowPins[(address >> 2) & 0b11];
    uint8_t values[] = {0, 0, 0, 0};

    for (uint8_t ix = 0; ix < 4; ix++)
    {
        pinMode(sensePins[ix], OUTPUT);
        digitalWrite(sensePins[ix], LOW);
        delay(1);
        pinMode(sensePins[ix], INPUT);
    }

    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);

    for (uint8_t phase = 0; phase < 2; phase++)
    {
        digitalWrite(A, phase % 2);
        digitalWrite(B, (phase + 1) % 2);
        for (uint8_t ix = 0; ix < 4; ix++)
        {
            values[ix] += analogRead(sensePins[ix]);
        }
    }
  
    pinMode(A, INPUT);
    pinMode(B, INPUT);

    uint8_t result = 0;
    for (uint8_t ix = 0; ix < 4; ix++)
    {
        result = (result << 1) | ((values[ix] > 1) ? 1 : 0);
    }

    return result;
}

void loop()
{
    crc.reset();

    for (int address = 0; address < 16; address++)
    {
        if (address > 0 && address % 2 == 0)
        {
            Serial.print(".");
        }

        uint8_t nibble = readNibble(address);

        crc.update(nibble);

        Serial.print(nibble, HEX);
    }

    uint32_t crcValue = crc.finalize();

    Serial.print(" - ");
    Serial.println(crcValue, HEX);

    if (crcValue != 3702334500)
    {
        Serial.println("CRC Error!");
        while (true)
        {
        };
    }
}