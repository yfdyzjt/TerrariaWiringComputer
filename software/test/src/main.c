#define SCREEN ((volatile unsigned char *)_display_96_64_origin)
#define SCREEN_CTRL ((volatile unsigned char *)(_display_96_64_origin + _display_96_64_length - 1))
#define DIVER ((volatile unsigned char *)0xFFFFFFFF)

#define SCREEN_CLC ((unsigned char)0)
#define SCREEN_REF ((unsigned char)1)

#define SCREEN_SIZE_X 96
#define SCREEN_SIZE_Y 64

#include "ASCII_5_7.h"

void drawByteGrid(int posX, int posY, int sizeX, int sizeY, unsigned char *grid)
{
	unsigned char *addr = (unsigned char *)(SCREEN + posX / 8 + posY * 16);

	int i;

	int high, low;
	unsigned short mask, date;

	low = posX % 8;
	high = low + sizeX;
	mask = (unsigned short)(((1 << high) - 1) ^ ((1 << low) - 1));

	for (i = 0; i < sizeY; i++)
	{
		if (i + posY < 0 && i + posY >= SCREEN_SIZE_Y)
			continue;
		date = (unsigned short)((grid[i] << low) & mask);

		if (posX >= 0 && posX < SCREEN_SIZE_X)
			*addr = (*addr & ~*((unsigned char *)&mask)) ^ *((unsigned char *)&date);
		if (high >= 8 && posX + 8 >= 0 && posX + 8 < SCREEN_SIZE_X)
			*(addr + 1) = (*(addr + 1) & ~*((unsigned char *)&mask + 1)) ^ *((unsigned char *)&date + 1);
		addr += 16;
	}
}

int x = SCREEN_SIZE_X;
int y = 0;

void drawChar(char c)
{
	if (c == '\n')
	{
		x = SCREEN_SIZE_X;
		y += 8;
	}
	else if (c >= 32 && c < 127)
	{
		x -= 6;
		drawByteGrid(x, y, 5, 7, (unsigned char *)g57Ascii[c - 32]);
	}
}

void drawString(char *s)
{
	int i = 0;
	while (s[i] != '\0')
	{
		drawChar(s[i++]);
	}
}

int main()
{
	drawString("Hello World!\n");
	*SCREEN_CTRL = SCREEN_REF;

	*DIVER = 0;
}
