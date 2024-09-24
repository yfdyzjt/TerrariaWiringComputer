#include "five_way_motion_sensor.h"
#include "display_96_64.h"
#include "random_number_generator.h"

void setPixel(int posX, int posY, unsigned char pixel)
{
	unsigned char *addr = &_display_buffer[posX / 8 + posY * 16];
	int offsetX = posX % 8;
	*addr &= ~(1 << offsetX);
	*addr ^= pixel << offsetX;
}

int main()
{
	int x = -1;
	// DISPLAY_SIZE_X / 2;
	int y = -1;
	// DISPLAY_SIZE_Y / 2;

	while (1)
	{
		// setPixel(_random_number % DISPLAY_SIZE_X, _random_number % DISPLAY_SIZE_Y, 1);
		setPixel(x, y, 0);

		unsigned char buffer = _motion_sensor_buffer;
		if (buffer & MOTION_SENSOR_U)
			y--;
		if (buffer & MOTION_SENSOR_D)
			y++;
		if (buffer & MOTION_SENSOR_R)
			x--;
		if (buffer & MOTION_SENSOR_L)
			x++;
		if (buffer & MOTION_SENSOR_J)
		{
			x = DISPLAY_SIZE_X / 2;
			y = DISPLAY_SIZE_Y / 2;
		}

		x %= DISPLAY_SIZE_X;
		y %= DISPLAY_SIZE_Y;
		setPixel(x, y, 1);
		*_display_ctrl = DISPLAY_REF;
	}
}
