#include <syslog.h>
#include "01_id_echo.h"
#include "00_become_deamon.h"

int main(int argc, char* argv[])
{
    int sfd;
    ssize_t numRead;
    socklen_t addrlen, len;
    struct sockaddr_storage claddr;
    char buf[BUF_SIZE];
    char addrStr[IS_ADDR_STR_LEN];

    if(becomeDaemon(0)==-1)
        errExit("becomedeamon");
    
    sfd = inetBind(SERVICE, SOCK_DGRAM, &addrlen);
    if(sfd == -1)
    {
        syslog(LOG_ERR, "could-not create server socket(%s)", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for(;;)
    {
        len = sizeof(struct sockaddr_storage);
        numRead = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr*)&claddr, &len);

        if(numRead == -1)
            errExit("recvfrom");
    
        if(sendto(sfd, buf, numRead, 0, (struct sockaddr*)&claddr, len)!= numRead)  
            syslog(LOG_WARNING, "Error echoing response tp %s (%s)", inetAddressStr((struct sockaddr*)&claddr, len, addrStr, IS_ADDR_STR_LEN), strerror(errno));

    }
}

/*
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation -pedantic -I../tlpi_lib 00_inet_sockets.c 01_id_echo_sv.c ../tlpi_lib/error_functions.c ../tlpi_lib/get_num.c -pthread -o 01_id_echo_sv.out
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation  -pedantic  00_inet_sockets.c 01_id_echo_sv.c  -o 01_id_echo_sv.out -L ../tlpi_lib/ -ltlpi 
*/
