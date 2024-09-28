#include "driver.h"

void driver_on()
{
    *DRIVER = DRIVER_ON;
}

void driver_off()
{
    *DRIVER = DRIVER_OFF;
}
