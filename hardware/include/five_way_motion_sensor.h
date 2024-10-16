#ifndef MOTION_SENSOR
#define MOTION_SENSOR

#define MOTION_SENSOR_U ((unsigned char)(1 << 3))
#define MOTION_SENSOR_D ((unsigned char)(1 << 1))
#define MOTION_SENSOR_R ((unsigned char)(1 << 4))
#define MOTION_SENSOR_L ((unsigned char)(1 << 0))
#define MOTION_SENSOR_J ((unsigned char)(1 << 2))

extern volatile const unsigned char _motion_sensor_buffer;
extern unsigned char _motion_sensor_cache;

void motion_sensor_refresh();

char motion_sensor_u();
char motion_sensor_d();
char motion_sensor_r();
char motion_sensor_l();
char motion_sensor_j();

#endif
