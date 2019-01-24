#ifndef _CHASSIS_TASK_H_
#define _CHASSIS_TASK_H_

void chassis_vel_pid_init(float p, float i, float d, float i_max);
void chassis_cal(void);
void chassis_task(int vx, int vy, int vw);
void chassis_power_off(void);

#endif
