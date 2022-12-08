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
#include <arpa/inet.h>

#define SEND_PORT 9292
#define BUFF_SIZE 4096
// using will be ./receive_file <ip adress> <file name>


int main(int argc, char** argv)
{
    if(argc != 3)
    {
        printf("./receive_file <ip adress> <file name>\n");
        exit(1);
    }

    char server_ip[16];
    char file_name[16];
    strcpy(server_ip, argv[1]);
    strcpy(file_name, argv[2]);
    int new_file_fd;

    int cs; // client socket
    struct sockaddr_in client_addr, server_addr;
    
    //client sockaddr_in
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   
    //server sockaddr_in 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SEND_PORT);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if((cs = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("failed to create a socket");
        exit(1);
    }

    int opt = 1;
    setsockopt(cs, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if((bind(cs, (struct sockaddr*)&client_addr, sizeof(client_addr))) < 0)
    {
        perror("bind failed");
        exit(1);
    }
       
    if((connect(cs, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0)
    {
        perror("connecting failed");
        exit(1);
    }
    
    unsigned char buffer[BUFF_SIZE];
    //opening new file
    new_file_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(new_file_fd < 0)
    {
        perror("Cant open file for receiving image");
        exit(1);
    }
    
    //getting image from the server
    int bytes;
    while((bytes = read(cs, buffer, BUFF_SIZE)) > 0)
    {
        write(new_file_fd, buffer, bytes);
    }
    
    close(new_file_fd);
    close(cs);
    return 0;
}
