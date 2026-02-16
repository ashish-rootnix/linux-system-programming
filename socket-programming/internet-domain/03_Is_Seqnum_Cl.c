#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <netdb.h>
#include "03_Is_Seqnum.h"

int main(int argc, char *argv[])
{
    char *reqLenStr;
    char seqNumStr[INT_LEN];
    int cfd;
    ssize_t numRead;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    if(argc < 2 || strcmp(argv[1], "--help")==0)
        usageErr("%s server-host [sequence-len]\n", argv[0]);
    
    // Call getaddrinfo() to obtain a list of addresses that we can try conecting to

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    if(getaddrinfo(argv[1], PORT_NUM, &hints, &result) != 0)
        errExit("getaddrinfo");
    
    // walk through returned list until we find address structure whta can be used to succesfully connect a socket
    for(rp = result; rp != NULL; rp = rp -> ai_next)
    {
        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(cfd == -1)
            continue;   // On error, try next address
        
        if(connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;  // Success
        
        close(cfd);
    }

    if(rp == NULL)
        fatal("could not connect socket to any address");
    
    freeaddrinfo(result);

    // Send requested sequence length, with terminating new line

    reqLenStr = (argc > 2)? argv[2] : "1";
    if(write(cfd, reqLenStr, strlen(reqLenStr)) != (ssize_t)strlen(reqLenStr))
        fatal("Pratial/failed write(reqLenStr)");
    if(write(cfd, "\n", 1)!= 1)
        fatal("Pratial/failed write(newline)");
    
    numRead = readLine(cfd, seqNumStr, INT_LEN);
    if(numRead == -1)
        errExit("readLine");
    if(numRead == 0)
        fatal("Unexpected EOF from server");

    printf("Sequence number: %s", seqNumStr);
    exit(EXIT_SUCCESS);
}

/*
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation -pedantic -I../tlpi_lib 01_Read_Line.c 03_Is_Seqnum_Cl.c ../tlpi_lib/error_functions.c ../tlpi_lib/get_num.c -pthread -o 03_Is_Seqnum_Cl
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation  -pedantic 01_Read_Line.c 03_Is_Seqnum_Cl.c  -o 03_Is_Seqnum_Cl.out -L ../tlpi_lib/ -ltlpi 
*/