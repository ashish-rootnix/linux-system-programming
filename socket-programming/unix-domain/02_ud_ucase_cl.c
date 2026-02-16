#include <stdio.h>
#include "ud_ucase.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un svaddr, claddr;
    int sfd, j;
    size_t msglen;
    ssize_t numBytes;
    char resp[BUF_SIZE];

    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s msg...\n", argv[0]);

    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);   // Create server socket
    if(sfd == -1)
        errExit("socket");
    
    memset(&claddr, 0, sizeof(struct sockaddr_un));
    claddr.sun_family = AF_UNIX;
    snprintf(claddr.sun_path, sizeof(claddr.sun_path), "/tmp/ud_ucase_cl.%ld", (long)getpid());
    
    if(bind(sfd, (struct sockaddr*)&claddr, sizeof(struct sockaddr_un)) == -1)
        errExit("bind");
    
    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);
    
    for(j = 1; j < argc; j++)
    {
        msglen = strlen(argv[j]);
        if(sendto(sfd, argv[j], msglen, 0, (struct sockaddr*)&svaddr, sizeof(struct sockaddr_un)) != (ssize_t)msglen)
            fatal("sendto");
        
        numBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
        if(numBytes == -1)
            errExit("recvfrom");
        
        printf("response %d: %.*s\n", j, (int)numBytes, resp);
    }
    remove(claddr.sun_path);
    exit(EXIT_SUCCESS);
}

/*
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation -pedantic -I../tlpi_lib 02_ud_ucase_cl.c ../tlpi_lib/error_functions.c ../tlpi_lib/get_num.c -pthread -o 02_ud_ucase_cl
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation  -pedantic 02_ud_ucase_cl.c  -o 02_ud_ucase_cl.out -L ../tlpi_lib/ -ltlpi 
./02_ud_ucase_sv  &
./02_ud_ucase_cl hello world

server received 5 bytes from /tmp/ud_ucase_cl.113292
response 1: HELLO
server received 5 bytes from /tmp/ud_ucase_cl.113292
response 2: WORLD

./02_ud_ucase_cl 'long message'
server received 10 bytes from /tmp/ud_ucase_cl.113295
response 1: LONG MESSA

kill %1

*/