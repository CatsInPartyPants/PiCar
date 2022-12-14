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
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <wiringPi.h>

#include "blinker.h"
#include "makephoto.h"


#define PORT 9111

int main()
{
    wiringPiSetup();
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
    else
    {
        printf("[+]Creation of socket.\n");
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
    else
    {
    
       printf("[+]Socket bind succesfull.\n");
    }
    
    /*listening...*/
    printf("[+]Listening...\n");
    result = listen(fs, 5);   

   /*accept the connection*/ 
    cs = accept(fs, (struct sockaddr*)&client_addr, &addrlen); 
    if(cs == -1)
    {
        perror("Accepting failed\n");
        exit(1);
    }
    else
    {
       printf("[+]Accepting the connection succesfully.\n");
    }
    
    
    /*making child process*/
    
    pid = fork();
    if(pid == 0)
    {
        close(fs);//close listen socket from parent process
        printf("[+] Child process working...\n");
        for(;;)
        {
            char buff[3] = {0};
            int count_of_bytes;
            count_of_bytes = read(cs, &buff, sizeof buff);
            printf("%d received.\n", count_of_bytes);
            if(count_of_bytes == 0)
                break;

            printf("%s\n", buff);

            if(0 == strcmp(buff, "q"))
                break;
            if(0 == strcmp(buff, "w"))
            {
                printf("Moving forward\n");
                move(1);
            }

            if(0 == strcmp(buff, "a"))
            {
                printf("Turning left\n");
                move(3);
            }

            if(0 == strcmp(buff, "s"))
            {
                printf("Moving back\n");
                move(2);
            }

            if(0 == strcmp(buff, "d"))
            {
                printf("Turnin right\n");
                move(4);
            }
            
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
                //make photo
                printf("[+]I'm got the p letter!\n");
                make_single_photo();
                printf("[+]photo ready!\n");
                sleep(3);
                //trying to send file
                
                int _size_of_file = 0;
                int in_fd = open("image.jpg", O_RDONLY);
                int bytes_sended = 0;    
                if(in_fd == -1)
                {
                    perror("Cant open file for send");
                    exit(1);
                }
                else
                {
                    printf("[+]File for send opened.\n");
                } 

                struct stat fileStatBuff;
                if(fstat(in_fd, &fileStatBuff) < 0)
                {
                    perror("failed to get stats of file");
                    exit(1);
                }
                else
                {
                    printf("[+]Got stats (size) of the file.\n");
                }

                _size_of_file = fileStatBuff.st_size;
                printf("[+]Size of the file is %d\n", _size_of_file);
                printf("[+]Sending.\n");
                bytes_sended = sendfile(cs, in_fd, NULL, _size_of_file);
                printf("[+]%d bytes sended succesfully.\n", bytes_sended);
                close(in_fd);
                printf("[+]File descriptor closed\n");
            }
        }
    }

    printf("got the connection! Parent...\n");
    close(cs); //close accepted socked from established connection
    wait(NULL);
    close(fs);
}
