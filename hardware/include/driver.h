#ifndef DRIVER
#define DRIVER ((volatile unsigned char *)0xFFFFFFFF)
#define DRIVER_ON 1
#define DRIVER_OFF 0

void driver_on();
void driver_off();

#endif