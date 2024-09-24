#ifndef MOTION_SENSOR
#define MOTION_SENSOR

#define MOTION_SENSOR_U ((unsigned char)0b01000)
#define MOTION_SENSOR_D ((unsigned char)0b00010)
#define MOTION_SENSOR_R ((unsigned char)0b10000)
#define MOTION_SENSOR_L ((unsigned char)0b00001)
#define MOTION_SENSOR_J ((unsigned char)0b00100)

volatile const unsigned char _motion_sensor_buffer __attribute__((section(__FILE_NAME__)));

#endif
