#include "main.h"
#include "can1.h"
#include "pid.h"
#include "uart5.h"

pid_Typedef track_vel_pid[2];
int track_current[2];
u8 track_on_flag;

extern rmc620_t track_motor[2];

/**
  * @brief  履带速度环计算
  * @param  None
  * @retval None
  */
void track_cal(void)
{	
	if (g_flag.control_mode == RC_MODE && g_flag.control_target == MANUAL_LAND_MODE)        //处于遥控模式，控制底盘及补弹气缸         
	{
		if (rc_ctrl.rc.ch3 > 1500)
			track_on_flag = 1;
		else if (rc_ctrl.rc.ch3 < 500)
			track_on_flag = 0;
	}
	else
		track_on_flag = 0;
	
	if (track_on_flag)                    //开启履带
	{
		track_vel_pid[0].SetPoint = -5000;
		track_vel_pid[1].SetPoint = 5000;
	}		
	else                                  //关闭履带
		track_vel_pid[0].SetPoint = track_vel_pid[1].SetPoint = 0;
	
	//PID计算
	track_current[0] = PID_Calc(&track_vel_pid[0], track_motor[0].speed);
	track_current[1] = PID_Calc(&track_vel_pid[1], track_motor[1].speed);
	
	track_current_send(track_current[0], track_current[1]);
}

/**
  * @brief  履带断电
  * @param  None
  * @retval None
  */
void track_power_off(void)
{
	track_current_send(0, 0);
}

/**
  * @brief  履带速度环参数初始化
  * @param  PID各参数
  * @retval None
  */
void track_vel_pid_init(float p, float i, float d, float i_max)
{
	int j;
	
	for (j = 0; j < 2; j ++)
	{
		track_vel_pid[j].P = p;
		track_vel_pid[j].I = i;
		track_vel_pid[j].D = d;
        track_vel_pid[j].IMax = i_max;
		track_vel_pid[j].SetPoint = 0.0f;
	}
}
