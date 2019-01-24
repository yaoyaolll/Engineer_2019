#ifndef _TRACK_TASK_H_
#define _TRACK_TASK_H_
#include "main.h"

void track_cal(u8 flag);
void track_power_off(void);
void track_vel_pid_init(float p, float i, float d, float i_max);

#endif
