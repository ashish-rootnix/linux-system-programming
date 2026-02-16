#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include "../tlpi_lib/tlpi_hdr.h"

#define BUF_SIZE 10 /*Maximum size of message exchanged between client and server*/

#define PORT_NUM 50002  /*Server port number*/
