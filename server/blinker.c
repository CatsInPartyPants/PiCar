#include <wiringPi.h>
#include "blinker.h"
#include <stdio.h>

void blinker()
{
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

void move(int destination)
{
    //1 - forward
    //2 - back
    //3 - left
    //4 - right
    enum dest{
        forward = 1, 
        back, 
        left, 
        right
    };

    switch(destination)
    {
        case forward:
            pinMode(1, OUTPUT);
            pinMode(4, OUTPUT);
            pinMode(5, OUTPUT);
            pinMode(6, OUTPUT);

            digitalWrite(1, HIGH);
            digitalWrite(4, LOW);
            digitalWrite(5, LOW);
            digitalWrite(6, HIGH);
            
            delay(MILISEK_TO_MOVE);
            
            pinMode(1, INPUT);
            pinMode(4, INPUT);
            pinMode(5, INPUT);
            pinMode(6, INPUT);

            break;
        
        case back:
            pinMode(1, OUTPUT);
            pinMode(4, OUTPUT);
            pinMode(5, OUTPUT);
            pinMode(6, OUTPUT);

            digitalWrite(1, LOW);
            digitalWrite(4, HIGH);
            digitalWrite(5, HIGH);
            digitalWrite(6, LOW);

            delay(MILISEK_TO_MOVE);

            pinMode(1, INPUT);
            pinMode(4, INPUT);
            pinMode(5, INPUT);
            pinMode(6, INPUT);

            break;

        case left:
            pinMode(1, OUTPUT);
            pinMode(4, OUTPUT);
            
            digitalWrite(1, HIGH);
            digitalWrite(4, LOW);

            delay(MILISEK_TO_MOVE);

            pinMode(1, INPUT);
            pinMode(4, INPUT);
            
            break;

        case right:
            pinMode(5, OUTPUT);
            pinMode(6, OUTPUT);

            digitalWrite(5, LOW);
            digitalWrite(6, HIGH);

            delay(MILISEK_TO_MOVE);

            pinMode(5, INPUT);
            pinMode(6, INPUT);

            break;

        default:
            printf("WRONG INPUT\n");
    }
}
