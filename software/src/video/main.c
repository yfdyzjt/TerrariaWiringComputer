#include "display_96_64.h"

#include "video.h"

int main()
{
	int y, ptr = 0;

	while (ptr < sizeof(video) / sizeof(unsigned int))
	{
		for (y = 0; y < 48; y++)
		{
			*(unsigned int *)&display_buffer[(y << 4) + 0] = video[ptr++];
			*(unsigned int *)&display_buffer[(y << 4) + 4] = video[ptr++];
		}
		*display_ctrl = DISPLAY_REF;
	}
}
