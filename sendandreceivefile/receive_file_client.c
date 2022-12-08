#include <sys/socket.h>
#include <netinet/io.h>


int main()
{
    int cs;
    struct sockaddr_in

    cs = socket(AF_INET, SOCK_STREAM, 0);

    bind(cs, &client_addr, sizeof(client_addr));
}
