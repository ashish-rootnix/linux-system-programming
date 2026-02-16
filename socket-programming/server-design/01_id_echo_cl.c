#include "01_id_echo.h"

int main(int argc, char* argv[])
{
    int sfd, j;
    size_t len;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if(argc < 2 || strcmp(argv[1], "--help")== 0)
        usageErr("%s: host msg...\n", argv[0]);

    sfd = inetConnect(argv[1], SERVICE, SOCK_DGRAM);
    if(sfd == -1)
        fatal("Could not conect to server socket");

    for(j = 2; j < argc; ++j)
    {
        len = strlen(argv[j]);
        if(write(sfd, argv[j], len)!= (ssize_t)len)
            fatal("Partial failed write");
        
        numRead = read(sfd, buf, BUF_SIZE);
        if(numRead == -1)
            errExit("read");
        
        printf("[%ld bytes] %.*s\n", (long)numRead, (int)numRead, buf);
    }

    exit(EXIT_SUCCESS);
}

/*
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation -pedantic -I../tlpi_lib 00_inet_sockets.c 01_id_echo_cl.c ../tlpi_lib/error_functions.c ../tlpi_lib/get_num.c -pthread -o 01_id_echo_cl.out
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation  -pedantic  00_inet_sockets.c 01_id_echo_cl.c  -o 01_id_echo_cl.out -L ../tlpi_lib/ -ltlpi 
*/
