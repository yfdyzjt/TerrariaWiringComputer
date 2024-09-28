#include "display_96_64.h"

int main()
{
	display_clear();
	draw_string(DISPLAY_SIZE_X, 0, "Hello World!");
	display_refresh();
}
