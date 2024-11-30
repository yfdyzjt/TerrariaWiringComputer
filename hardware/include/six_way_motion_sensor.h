#ifndef MOTION_SENSOR
#define MOTION_SENSOR

#define SIX_MOTION_SENSOR_U ((unsigned char)(1 << 3))
#define SIX_MOTION_SENSOR_D ((unsigned char)(1 << 1))
#define SIX_MOTION_SENSOR_R ((unsigned char)(1 << 4))
#define SIX_MOTION_SENSOR_L ((unsigned char)(1 << 0))
#define SIX_MOTION_SENSOR_J ((unsigned char)(1 << 2))
#define SIX_MOTION_SENSOR_S ((unsigned char)(1 << 5))

extern volatile const unsigned char _six_motion_sensor_buffer;
extern unsigned char _six_motion_sensor_cache;

void six_motion_sensor_refresh();

char six_motion_sensor_u();
char six_motion_sensor_d();
char six_motion_sensor_r();
char six_motion_sensor_l();
char six_motion_sensor_j();
char six_motion_sensor_s();

#endif
