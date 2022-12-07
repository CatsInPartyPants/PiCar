#include <wiringPi.h>
#include "blinker.h"

void blinker()
{
    wiringPiSetup();
    pinMode(0, OUTPUT);
    for(int i = 0; i < 10; i++)
    {
        digitalWrite(0, HIGH);
        delay(500);
        digitalWrite(0, LOW);
        delay(500);
    }
    pinMode(0, INPUT);
}
