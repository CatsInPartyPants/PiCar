#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include "makephoto.h"

#define QUALITY "20"
#define WIDTH "1366" //width of photo
#define HEIGHT "798" //height of photo
#define FILENAME "image.jpg" //filename

void make_single_photo()
{
    char filename[] = FILENAME;
    int pid;
    
    pid = fork();
    if(pid == 0)
    {
        execlp("raspistill", "raspistill", "-rot", "180","-q", QUALITY, "-w", WIDTH, "-h", HEIGHT, "-o", filename, NULL);                
    }
    wait(NULL);
}
