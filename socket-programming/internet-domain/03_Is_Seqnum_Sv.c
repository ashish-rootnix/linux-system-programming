#define _DEFAULT_SOURCE

#include <netdb.h>
#include "03_Is_Seqnum.h"

#define BACKLOG 50

int main(int argc, char *argv[])
{
    uint32_t seqNum;
    char reqLenStr[INT_LEN];
    char seqNumStr[INT_LEN];
    struct sockaddr_storage claddr;
    int lfd, cfd, optval, reqLen;

    socklen_t addrlen;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    #define ADDSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)

    char addrStr[ADDSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    if(argc > 1 && strcmp(argv[1], "--help")==0)
        usageErr("%s [init-seq-num]\n", argv[0]);
    
    seqNum = (argc > 1) ? getInt(argv[1], 0, "init-seq-num") : 0;

    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        errExit("signal");
    
    // call getaddrinfo() to obtain a list of addresses that we can try binding to

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV; // wildcard IP address, service name is numeric

    if(getaddrinfo(NULL, PORT_NUM, &hints, &result)!=0)
        errExit("getaddrinfo()");
    
    /* walk through returned list until we find an address structure than can be used 
    to successfully create and bind a socket*/
    optval = 1;
    for(rp = result; rp != NULL; rp = rp->ai_next)
    {
        lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(lfd == -1)
            continue;
        
        if(setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))==-1)
            errExit("setsockopt");
            
        if(bind(lfd, rp->ai_addr, rp->ai_addrlen)==0)
            break; // Success
        
        /* bind() failed then close this socket and try next address*/
    }

    if(rp == NULL)
        fatal("could not bind to any address");
    
    if(listen(lfd, BACKLOG)==-1)
        errExit("listen");
    
    freeaddrinfo(result);

    for(;;)     // handle client iteratively
    {
        // Accept a client connection, obtaining clients address
        addrlen = sizeof(struct sockaddr_storage);
        cfd = accept(lfd, (struct sockaddr *)&claddr, &addrlen);
        if(cfd == -1)
        {
            errMsg("msg");
            continue;
        }
        if(getnameinfo((struct sockaddr*)&claddr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
            snprintf(addrStr, ADDSTRLEN, "(%s, %s)", host, service);
        else
            snprintf(addrStr, ADDSTRLEN, "?(unknown)");
        
        printf("Connection from %s\n", addrStr);

        if(readLine(cfd, reqLenStr, INT_LEN)<=0)
        {
            close(cfd);
            continue;       // Failed read
        }       
        reqLen = atoi(reqLenStr);
        if(reqLen <= 0)     // watch misbehaving client
        {
            close(cfd);
            continue;       // bad request skip it
        }

        snprintf(seqNumStr, INT_LEN, "%d\n", seqNum);
        if(write(cfd, &seqNumStr, strlen(seqNumStr)) != (ssize_t)strlen(seqNumStr))
            fprintf(stderr, "Error on Write");
        
        seqNum += reqLen;

        if(close(cfd) == -1)    // clsoe connection
            errMsg("close");
    }
}

/*
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation -pedantic -I../tlpi_lib 03_Is_Seqnum_Sv.c ../tlpi_lib/error_functions.c ../tlpi_lib/get_num.c -pthread -o 03_us_abstract_bind
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation  -pedantic 03_Is_Seqnum_Sv.c  -o 03_Is_Seqnum_Sv.out -L ../tlpi_lib/ -ltlpi 

*/
