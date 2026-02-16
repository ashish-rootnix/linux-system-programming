#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include "01_Read_Line.h"
#include "../tlpi_lib/tlpi_hdr.h"

#define PORT_NUM "50000"        // Port number for the server
#define INT_LEN 30              // Size of string able to hold largest integar including terminating '\n'
