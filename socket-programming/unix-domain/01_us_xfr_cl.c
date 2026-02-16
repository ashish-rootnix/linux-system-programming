#include <stdio.h>
#include "us_xfr.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0); // Create client socket
    if(sfd == -1)
        errExit("socket");
    
    /* Connect server address, and make connection */
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path)-1);

    if(connect(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_un)-1))
        errExit("connect");

    /*Copy stdin to socket*/
    while((numRead = read(STDIN_FILENO, buf, BUF_SIZE))>0)
        if(write(sfd, buf, numRead) != numRead)
            fatal("partial/failed write");
        
    if(numRead == -1)
        errExit("read");
    
    exit(EXIT_SUCCESS);
}

/*
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation -pedantic -I../tlpi_lib 01_us_xfr_cl.c ../tlpi_lib/error_functions.c ../tlpi_lib/get_num.c -pthread -o 01_us_xfr_cl
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation  -pedantic 01_us_xfr_cl.c  -o 01_us_xfr_cl.out -L ../tlpi_lib/ -ltlpi 
$ ./01_us_xfr_sv > b &
$ cat *.c > a
$ ./01_us_xfr_cl < a
$ kill %1
$ ls -lF /tmp/us_xfr
$ diff a b
*/
