#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

// Prints message on standard error
void errMsg(const char *format, ...);

#ifdef __GNUC__

/*  This micro stops 'gcc -wall' complaining that "function reaches end of non-void function"
    If we use the following functions to terminate main() or some other non-void function. */
#define NORETURN __attribute__((__noreturn))
#else
#define NORETURN
#endif

// Opearates like errMsg(), also terminates the program either by calling exit() or abort()
void errExit(const char *format, ...) NORETURN;

// Opeartes like errMsg, aslo termiantes program with _exit()
void err_exit(const char *format, ...) NORETURN;

void errExitEN(int errnum, const char *format, ...) NORETURN;
void fatal(const char *format, ...) NORETURN;
void usageErr(const char *format, ...) NORETURN;
void cmdLineErr(const char *format, ...)NORETURN;

#endif
