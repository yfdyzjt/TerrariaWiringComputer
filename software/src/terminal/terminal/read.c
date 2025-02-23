#include "keyboard_input.h"
#include "display_output.h"

#include <errno.h>
#undef errno
extern int errno;

int _read(int file, char *ptr, int len)
{
    if (file == 0)
    {
        int received;
        for (received = 0; received < len; received++)
        {
            ptr[received] = keyboard_input_scan();
            if (ptr[received] == '\n')
            {
                received++;
                break;
            }
            else if (ptr[received] == '\b')
            {
                if (received >= 1)
                {
                    display_output_print(ptr[received]);
                    received -= 2;
                }
            }
            else
            {
                display_output_print(ptr[received]);
            }
        }
        return received;
    }

    errno = EBADF;
    return -1;
}
