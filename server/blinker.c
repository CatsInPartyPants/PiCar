#include <wiringPi.h>
#include "blinker.h"
#include <stdio.h>

void blinker(int *is_on)
{
<<<<<<< HEAD
    if(*(is_on) == 0)
=======
    pinMode(0, OUTPUT);
    for(int i = 0; i < 10; i++)
>>>>>>> 7c247e244268cba531209d6e29b2dd664f32cbb4
    {
        pinMode(0, OUTPUT);
        digitalWrite(0, HIGH);
        *(is_on) = 1;
    }
    if(*(is_on) == 1)
    {
        digitalWrite(0, LOW);
        pinMode(0, INPUT);
        *(is_on) = 0;
    }
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

            delay(MILISEK_TO_TURN);

            pinMode(1, INPUT);
            pinMode(4, INPUT);
            
            break;

        case right:
            pinMode(5, OUTPUT);
            pinMode(6, OUTPUT);

            digitalWrite(5, LOW);
            digitalWrite(6, HIGH);

            delay(MILISEK_TO_TURN);

            pinMode(5, INPUT);
            pinMode(6, INPUT);

            break;

        default:
            printf("WRONG INPUT\n");
            break;
    }
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
