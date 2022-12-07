#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "receivefile.h"

void filereceiver(char* filename, int socketdescr)
{
    FILE *received_file = fopen(filename, "wb");
    while(1)
    {
        unsigned char buff[512];
        int nbytes = recv(socketdescr, buff, sizeof(buff), 0);
        if(nbytes == 0)
        {
            printf("Disconnected\n");
            break;
        }
        if(nbytes < 0)
        {
            perror("nbytes < 0");
            break;
        }
        int res = fwrite(buff, sizeof(unsigned char), sizeof buff, received_file);
        if(res <=0)
            break;
        
    }
}
