#ifndef TIPL_HDR_H
#define TIPL_HDR_H //Prevent accidental double inclusion

#include <sys/types.h>  // Types definitions used by many programs
#include <stdio.h>      // Standard I/O functions
#include <stdlib.h>     // Prototypes of commonly used library functions + EXIT_SUCCESS and EXIT_FAILURE constants
#include <unistd.h>     // Prototypes for many system calls
#include <errno.h>      // Decalres errno and defines error constants
#include <string.h>     // Commonly used string handling functions

#include "get_num.h"    // Decalres our functions handling numeric arguments (getInt(), getLong())
#include "error_functions.h" // Declare out error-handling functions

typedef enum{FALSE, TRUE} Boolean;

#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))

#endif
