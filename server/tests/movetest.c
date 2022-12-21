#include "blinker.h"
#include <wiringPi.h>


int main()
{
    wiringPiSetup();
    move(1);
    delay(1000);

    move(2);
    delay(1000);

    move(3);
    delay(1000);

    move(4);
    delay(1000);

    return 0;
}
