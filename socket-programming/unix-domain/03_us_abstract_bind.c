#include <sys/un.h>
#include <sys/socket.h>

#include "../tlpi_lib/tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_un addr;
    char *str;

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;

    str = "xyz";
    strncpy(&addr.sun_path[1], str, sizeof(addr.sun_path)-2);

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd == -1)
        errExit("socket");
    
    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)) == -1)
        errExit("bind");

    sleep(30);

    exit(EXIT_SUCCESS);
}

/*
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation -pedantic -I../tlpi_lib 03_us_abstract_bind.c ../tlpi_lib/error_functions.c ../tlpi_lib/get_num.c -pthread -o 03_us_abstract_bind
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation  -pedantic 03_us_abstract_bind.c  -o 03_us_abstract_bind.out -L ../tlpi_lib/ -ltlpi 
*/