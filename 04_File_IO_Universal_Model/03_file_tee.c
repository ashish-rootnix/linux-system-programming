#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "../tlpi_lib/tlpi_hdr.h"

#ifndef BUF_SIZE 
#define BUF_SIZE 1024  /* Allow "cc -D" to override definition */
#endif

int main(int argc, char* argv[])
{
    int outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead, numWritten_stdout, numWritten_file;
    char buf[BUF_SIZE];

    if(argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file \n", argv[0]);

    openFlags = O_CREAT | O_WRONLY;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    if(argc == 3 || strcmp(argv[1], "-a") == 0)
        openFlags |= O_APPEND;
    else if(argc == 2)
        openFlags |= O_TRUNC;

    outputFd = open(argv[argc-1], openFlags, filePerms);
    if(outputFd == -1)
        errExit("opening file %s", argv[2]);
    
    while(1)
    {
        numRead = read(STDIN_FILENO, buf, BUF_SIZE);
        if(numRead == -1)
        {
            errExit("read()");
            break;
        }
        if(numRead == 0)
            printf("EOD file reached");
        else
        {
            numWritten_stdout = write(STDOUT_FILENO, buf, numRead);
            if(numWritten_stdout != numRead)
                fatal("write() returned error or partial write occured on STDOUT");
            
            numWritten_file = write(outputFd, buf, numRead);
            if(numWritten_file != numRead)
                fatal("write() returned error or partial write occured on file %s", argv[argc-1]);            
        }
    }
    if(close(outputFd)==-1)
        errExit("close output");
    
    exit(EXIT_SUCCESS);
}
