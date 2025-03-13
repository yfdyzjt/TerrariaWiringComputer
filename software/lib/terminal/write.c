#ifdef TERMINAL

#include "display_output.h"

#include <errno.h>
#undef errno
extern int errno;

int _write(int file, char *ptr, int len)
{
    if (file == 1 || file == 2)
    {
        for (int i = 0; i < len; i++)
            display_output_print(ptr[i]);
        return len;
    }

    errno = EBADF;
    return -1;
}

#endif
