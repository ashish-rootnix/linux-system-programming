#include <signal.h>
#include <syslog.h>
#include <sys/wait.h>
#include "00_become_deamon.h"
#include "00_inet_sockets.h"
#include "../tlpi_lib/tlpi_hdr.h"

#define SERVICE "echo"
#define BUF_SIZE 4096

static void grimReaper(int sig)
{
    int savedErrno;
    savedErrno = errno;
    while(waitpid(-1, NULL, WNOHANG)>0)
        continue;
    errno = savedErrno;
}

static void handleRequest(int cfd)
{
    char buf[BUF_SIZE];
    ssize_t numRead;

    while((numRead=read(cfd, buf, BUF_SIZE)>0))
    {
        if(write(cfd, buf, numRead)!=numRead)
        {
            syslog(LOG_ERR, "write() failed: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    if(numRead == -1)
    {
        syslog(LOG_ERR, "Error from read(): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[])
{
    int lfd, cfd;
    struct sigaction sa;

    if(becomeDaemon(0)==-1)
        errExit("beacomDeamon");
    
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
}