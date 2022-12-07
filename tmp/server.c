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

#define PORT 9111

int main()
{
    int result, fs; //fs - listening socket
    int cs; // cs - client socket
    int pid; 
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
        //size_t buff_size = 1024;
        //void *buff = malloc(buff_size);
        unsigned char buff[512];
        size_t b;
        close(fs);//close listen socket from parent process
        /*
        for(;;)
        {
            scanf("%s", buff);
            write(cs, buff, sizeof(buff));
            if(0 == strcmp(buff, "exit"))
                break;
        } 
        */
        FILE *file = fopen("image.jpg", "rb");
        int i = 0;
        while(!feof(file))
        {
            b = fread(buff, sizeof(unsigned char), sizeof buff, file);
            long size = ftell(file);
            //printf("bytes read: %d, part %d, pos %ld \n", b, i, size);
            if(b!=0)
                send(cs, buff, b, 0);
            i++;
        }
        printf("[+] file %s sended successfully\n", "image.jpg");
        //free(buff);
    }
    printf("got the connection!\n");
    close(cs); //close accepted socked from established connection
    wait(NULL);
    close(fs);

}
