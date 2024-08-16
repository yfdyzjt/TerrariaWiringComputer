#include "display_96_64.h"

#include "video.h"

int main()
{
	int y, ptr = 0;

	while (ptr < sizeof(video) / sizeof(unsigned int))
	{
		for (y = 0; y < SCREEN_SIZE_Y; y++)
		{
			*(unsigned int *)(SCREEN + (y << 4) + 0) = video[ptr++];
			*(unsigned int *)(SCREEN + (y << 4) + 4) = video[ptr++];
			*(unsigned int *)(SCREEN + (y << 4) + 8) = video[ptr++];
		}
		*SCREEN_CTRL = SCREEN_REF;
	}
}
