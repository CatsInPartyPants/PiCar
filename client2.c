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
#include <ncurses.h>

#include "functions.h"
#include "receivefile.h"

#define PHOTO_BUFFER 400000
#define PORT 0

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Using: ./client2 <ip adress>\n");
        exit(1);
    }
    //ncurses
    int key;
    initscr();
    cbreak();
    keypad(stdscr,1);
    noecho();
    curs_set(0);

    int result;
    int cs; // cs - client socket
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
        endwin();
        exit(1);
    }

    /*bind socket*/
    int opt = 1;
    setsockopt(cs, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    result = bind(cs, (struct sockaddr*)&addr, sizeof(addr));
    if(result == -1)
    {
        perror("Socket bind failed");
        endwin();
        exit(1);
    }
    result = connect(cs, (struct sockaddr*)&servadr, sizeof(servadr));
    if(result == -1)
    {
        perror("failed to connect.\n");
        endwin();
        exit(1);
    }
    printf("Connection established!\n");
    help_message();
    
    //for(;;)
    {
       //sending messages
       char buff[3] = {0};
      // scanf("%s", buff);
      // write(cs, buff, sizeof(buff));
       //if(0 == strcmp(buff, "exit"))
        //   break;
       while((key = getch()) != 'q')
       {
           switch(key)
           {
              case KEY_UP:
                  buff[0] = 'w';
                  printw("FORWARD\n");
                  write(cs, buff, sizeof(buff));
                  break;
              case KEY_LEFT:
                  buff[0] = 'a';
                  write(cs, buff, sizeof(buff));
                  printw("LEFT\n");
                  break;
              case KEY_DOWN:
                  buff[0] = 's';
                  write(cs, buff, sizeof(buff));
                  printw("BACK\n");
                  break;
              case KEY_RIGHT:
                  buff[0] = 'd';
                  write(cs,buff, sizeof(buff));
                  printw("RIGHT\n");
                  break;
              case 'q':
                  buff[0] = 'q';
                  write(cs, buff, sizeof(buff));
                  printw("quit\n");
                  break;
              case 'b':
                  buff[0] = 'b';
                  printw("BLINK\n");
                  write(cs,buff, sizeof(buff));
                  break;
              case 'p':
                  buff[0] = 'p';
                  printw("PHOTO REQUEST\n");
                  int pid;
                  pid = fork();
                  if(pid == 0)
                  {  
                     write(cs, buff, sizeof(buff));
                     /*get file and save it*/
                     filereceiver("get_img.jpg", cs);
                     printf("function filereceiver done!");
                  }
                  sleep(10);
                  kill(pid, SIGKILL);
                  wait(NULL);       
           }
       }      
    }
    close(cs);
    endwin();
}
