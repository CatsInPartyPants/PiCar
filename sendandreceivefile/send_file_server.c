#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define SEND_PORT 9292
#define LENGHT_OF_FILE_NAME 50

// using will be ./send_file <file name>


int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Argc is not 3\nUsing is ./send_file <file name>\n");
        exit(1);
    }
    int sf; //server sock
    int cs; //client sock
    int pid;
    char filename[LENGHT_OF_FILE_NAME];
    strcpy(filename, argv[1]);
    
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SEND_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    struct sockaddr_in client_addr;
    socklen_t len_cl_addr = sizeof(client_addr); 
    

    if((sf = socket(AF_INET, SOCK_STREAM,0)) < 0)
    {
        perror("creation of socket failed");
        exit(1);
    }

    int opt;
    setsockopt(sf, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    if(bind(sf, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind failed");
        exit(1);
    }

    if(listen(sf, 5) < 0)
    {
        perror("listen failed");
        exit(1);
    }
    if((cs = accept(sf, (struct sockaddr*)&client_addr, &len_cl_addr)) < 0)
    {
        perror("accept failed");
        exit(1);
    }
    
    pid = fork();
    if(pid == 0)
    {
        close(sf);
        int _size_of_file = 0;
        int in_fd = open(filename, O_RDONLY);
        
        if(in_fd == -1)
        {
            perror("failed to open file for send");
            exit(1);
        }
        
        struct stat fileStatBuff;
        
        if(fstat(in_fd, &fileStatBuff) < 0)
        {
            perror("failed to get stats of the file");
            exit(1);
        }
        
        _size_of_file = fileStatBuff.st_size;
        sendfile(cs, in_fd, NULL, _size_of_file);
    }
    close(cs);
    printf("Got the connection & client socket is closed(its main process speaking)\n");
    wait(NULL);
    close(sf);
    return 0;
}
