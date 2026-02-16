#include <gnu/libc-version.h>
#include <stdio.h>
/*
const char* gnu_get_libc_version(void);

Returns pointer to null-termninated, statically allocated string containing GNU C library version number

*/

int main(void)
{
    const char *version = gnu_get_libc_version();

    if (version == NULL)
        return 1;

    printf("GNU glibc Version ==> %s\n", version);

    return 0;
}