#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "sendfile.h"

void filesender(char* filename, int socketdescr)
{
    FILE *file = fopen(filename, "rb");
    size_t b;
    unsigned char buff[512];
    while(!feof(file))
    {
        b = fread(buff, sizeof(unsigned char), sizeof buff, file);
        if(b!=0)
            send(socketdescr, buff, b, 0);
        if(b == 0)
            break;
    }
}
