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

#include "blinker.h"
#include "makephoto.h"
#include "sendfile.h"

#define PORT 9111

int main()
{
    int result, fs; //fs - listening socket
    int cs; // cs - client socket
    int pid;
    static int is_lamp_on = 0; 
    struct sockaddr_in addr, client_addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    //addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    socklen_t addrlen = sizeof(client_addr);

    /*create soket*/

    fs = socket(AF_INET, SOCK_STREAM, 0);
    if(fs == -1)
    {
        perror("Creation of socket failed\n");
        exit(1);
    }

    /*bind socket*/
    int opt = 1;
    setsockopt(fs, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    result = bind(fs, (struct sockaddr*)&addr, sizeof(addr));
    if(result == -1)
    {
        perror("Socket bind failed\n");
        exit(1);
    }
    
    /*listening...*/
    result = listen(fs, 5);   

   /*accept the connection*/ 
    cs = accept(fs, (struct sockaddr*)&client_addr, &addrlen); 
    if(cs == -1)
    {
        perror("Accepting failed\n");
        exit(1);
    }
    /*making child process*/
    pid = fork();
    if(pid == 0)
    {
        close(fs);//close listen socket from parent process
        for(;;)
        {
            char buff[3] = {0};
            int count_of_bytes;
            count_of_bytes = read(cs, &buff, 3);
            printf("%d received.\n", count_of_bytes);
            if(count_of_bytes == 0)
                break;

            printf("%s\n", buff);

            if(0 == strcmp(buff, "q"))
                break;
            if(0 == strcmp(buff, "w"))
                printf("Moving forward\n");
            if(0 == strcmp(buff, "a"))
                printf("Turning left\n");
            if(0 == strcmp(buff, "s"))
                printf("Moving back\n");
            if(0 == strcmp(buff, "d"))
                printf("Turnin right\n");
            if(0 == strcmp(buff, "b") && is_lamp_on == 0)
            {
               pid = fork();
               if(pid == 0)
               {
                   is_lamp_on = 1;
                   blinker();
                   is_lamp_on = 0;
               }
               //wait(NULL);
               //is_lamp_on = 0;
            }
            if(0 == strcmp(buff, "p"))
            {
                pid = fork();
                if(pid == 0)
                {
                    int send_sock = dup(cs);
                    make_single_photo();
                    sleep(5);
                    filesender("image.jpg", send_sock);
                    close(send_sock); 
                }
                //wait(NULL);
                sleep(10);
                kill(pid, SIGKILL);
                wait(NULL);

            }
        } 
        exit(0);
    }

    printf("got the connection!\n");
    close(cs); //close accepted socked from established connection
    wait(NULL);
    close(fs);

}
