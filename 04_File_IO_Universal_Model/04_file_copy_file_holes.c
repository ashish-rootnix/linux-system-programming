#include <sys/stat.h>
#include <fcntl.h>
#include "../tlpi_lib/tlpi_hdr.h"

#ifndef     BUF_SIZE        /* Allow "cc -D" to override definition */
#define     BUF_SIZE    4096
#endif

int main(int argc, char* argv[])
{
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];
    int isAllZeros;
    
    if(argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s old-file new-file\n", argv[0]);
    
    /* Open input and output files */
    inputFd = open(argv[1], O_RDONLY);
    if(inputFd == -1)
        errExit("Opening file %s", argv[1]);
    
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; /* rw-rw-rw-*/

    outputFd = open(argv[2], openFlags, filePerms);
    if(outputFd == -1)
        errExit("Opening file %s", argv[2]);
    
    /* Transfer data until we encounter end of input or an error */
    while((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
    {   isAllZeros = 0;
        static const char zeros[BUF_SIZE] = {0};
        isAllZeros = (memcmp(buf, zeros, numRead) == 0);
        if(isAllZeros)
            lseek(outputFd, numRead, SEEK_CUR);
        else
        {
            if(write(outputFd, buf, numRead)!= numRead)
                fatal("write() returned error or partial write occured");
        }
    }
    
    if(numRead == -1)
        errExit("read()");

    if(isAllZeros)
    {
           if(write(outputFd, "/0", 1)!= 1)
                fatal("write() returned error or partial write occured");        
    }

    if(close(inputFd) == -1)
        errExit("Close input");
    if(close(outputFd) == -1)
        errExit("Close output");
        
    exit(EXIT_SUCCESS);
}

/*
    gcc 04_file_copy_file_holes.c ../tlpi_lib/error_functions.c -Wno-attributes
    ./a.out File_Universal_IO_Model.txt Copied.txt
*/
