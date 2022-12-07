#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define PORT 0

int main(int argc, char *argv[])
{
    int result;
    int cs; // cs - client socket
    if(argc != 2)
    {
        printf("Usage ./client <ip_adress>\n");
        exit(0);
    }
    char ip_adress[16];
    strcpy(ip_adress, argv[1]);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    struct sockaddr_in servadr;
    servadr.sin_family = AF_INET;
    servadr.sin_port = htons(9111);
    servadr.sin_addr.s_addr = inet_addr(ip_adress);

    /*create soket*/

    cs = socket(AF_INET, SOCK_STREAM, 0);
    if(cs == -1)
    {
        perror("Creation of socket failed");
        exit(1);
    }

    /*bind socket*/
    int opt = 1;
    setsockopt(cs, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    result = bind(cs, (struct sockaddr*)&addr, sizeof(addr));
    if(result == -1)
    {
        perror("Socket bind failed");
        exit(1);
    }
    result = connect(cs, (struct sockaddr*)&servadr, sizeof(servadr));
    if(result == -1)
    {
        perror("failed to connect.\n");
        exit(1);
    }
    printf("Connection established!\n");
    
    FILE *received_file = fopen("received_image.jpg", "wb"); 
    int i = 1;
    while(1)
    {
        //size_t buff_size = 1024;
        //void* buff = malloc(buff_size);
        unsigned char buff[512];
        int nbytes = recv(cs, buff, sizeof(buff), 0);
        if(nbytes == 0)
        {
            printf("Disconneted\n");
            exit(1);
        }
        if(nbytes < 0)
        {
            perror("nbytes < 0");
            exit(1);
        }
        //fwrite(buff, 1, sizeof(buff), received_file);
        fwrite(buff, sizeof(unsigned char), sizeof buff, received_file);
       //printf("%d bytes, %d\n", nbytes, i);
        i++;
        //free(buff);
    }
    close(cs);
    return 0;
}
