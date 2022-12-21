#include <stdio.h>
#include <ncurses.h>
#include "functions.h"


void help_message()
{
    printw("press \"q\" for exit.\n");
    printw("press \"arrow up\" for moving forward.\n");
    printw("press \"arrow left\" for turn left.\n");
    printw("press \"arrow down\" for moving back.\n");
    printw("press \"arrow right\" for turn right.\n");
}
