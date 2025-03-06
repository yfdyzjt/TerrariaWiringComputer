#include "keyboard_input.h"
#include "display_output.h"

#include <errno.h>
#undef errno
extern int errno;

int _read(int file, char *ptr, int len)
{
    if (file == 0)
    {
        int received = 0;
        while (received < len)
        {
            char c = keyboard_input_scan();
            if (c == '\b')
            {
                if (received > 0)
                {
                    received--;
                    display_output_print(c);
                }
            }
            else
            {
                ptr[received++] = c;
                display_output_print(c);
                if (c == '\n')
                    break;
            }
        }
        return received;
    }

    errno = EBADF;
    return -1;
}
