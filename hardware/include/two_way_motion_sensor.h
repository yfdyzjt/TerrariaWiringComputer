#ifndef MOTION_SENSOR
#define MOTION_SENSOR

#define MOTION_SENSOR_U ((unsigned char)0b01)
#define MOTION_SENSOR_D ((unsigned char)0b10)

volatile const unsigned char _motion_sensor_buffer __attribute__((section(__FILE_NAME__)));

#endif
