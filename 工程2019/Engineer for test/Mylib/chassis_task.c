#include "main.h"
#include "pid.h"
#include "can1.h"
#include "can2.h"
#include "uart5.h"

int vx, vy, vw;
pid_Typedef chassis_vel_pid[4];
pid_Typedef chassis_pos_follow_pid = {
	.P = 0.0f,
	.I = 0.0f,
	.D = 0.0f,
	.IMax = 0.0f,
	.SetPoint = 0.0f
};
pid_Typedef chassis_vel_follow_pid = {
	.P = 0.0f,
	.I = 0.0f,
	.D = 0.0f,
	.IMax = 0.0f,
	.SetPoint = 0.0f
};

extern rm820r_t chassis_motor[4];     

/**
  * @brief  底盘速度环参数初始化
  * @param  PID各参数
  * @retval None
  */
void chassis_vel_pid_init(float p, float i, float d, float i_max)
{
	int j = 0;
	
	for (j = 0; j < 4; j ++)
	{
		chassis_vel_pid[j].P = p;
		chassis_vel_pid[j].I = i;
		chassis_vel_pid[j].D = d;
        chassis_vel_pid[j].IMax = i_max;
		chassis_vel_pid[j].SetPoint = 0.0f;
	}
}

/**
  * @brief  底盘断电
  * @param  None
  * @retval None
  */
void chassis_power_off(void)
{
	chassis_current_send(0, 0, 0, 0);
}

/**
  * @brief  底盘速度分解及PID计算
  * @param  vx：左右速度
			vy：前后速度
			vw：旋转速度
  * @retval None
  */
int test_chassis_current[4];                 //only for debug to watch
void chassis_task(int vx, int vy, int vw)
{
	int i = 0;
	
	chassis_vel_pid[0].SetPoint = LIMIT_MAX_MIN(-(vx + vy + vw), 7000, -7000);
	chassis_vel_pid[1].SetPoint = LIMIT_MAX_MIN((vx - vy - vw), 7000, -7000);
	chassis_vel_pid[2].SetPoint = LIMIT_MAX_MIN(-(vx + vy + vw), 7000, -7000);
	chassis_vel_pid[3].SetPoint = LIMIT_MAX_MIN(-(vx + vy + vw), 7000, -7000);
	
	for (i = 0; i < 4; i ++)
		test_chassis_current[i] = PID_Calc(&chassis_vel_pid[i], chassis_motor[i].speed);
	
	chassis_current_send(test_chassis_current[0], test_chassis_current[1], test_chassis_current[2], test_chassis_current[3]);
}

/**
  * @brief  底盘跟随陀螺仪YAW轴角度
  * @param  vx：左右速度
			vy：前后速度
			vw：旋转速度
  * @retval None
  */
void chassis_cal(void)
{  
	if (g_flag.initial_flag == 1)
	{
		if (g_flag.control_mode == RC_MODE && g_flag.control_target == CHASSIS_MODE)         //处于遥控模式，控制底盘及补弹气缸
		{
			if (rc_ctrl.rc.ch0 > 1044 || rc_ctrl.rc.ch0 < 1004)
				vx = (1024 - rc_ctrl.rc.ch0) * 10.0f;
			else 
				vx = 0;
			
			if (rc_ctrl.rc.ch1 > 1044 || rc_ctrl.rc.ch1 < 1004)
				vy = (1024 - rc_ctrl.rc.ch1) * 10.0f;
			else 
				vy = 0;
			
			if (rc_ctrl.rc.ch2 > 1044 || rc_ctrl.rc.ch2 < 1004)
			{	
				if (g_flag.gyro_use_flag)	//使用陀螺仪数据
				{
					chassis_pos_follow_pid.SetPoint += (1024 - rc_ctrl.rc.ch2) / 4000.0f;
					chassis_vel_follow_pid.SetPoint = LIMIT_MAX_MIN(PID_Calc(&chassis_pos_follow_pid, yaw_angle), 5.7f, -5.7f);
					vw = PID_Calc(&chassis_vel_follow_pid, gz);
				}
				else						//不使用陀螺仪数据
					vw = (1024 - rc_ctrl.rc.ch2) * 10.0f;
			}
		}
		else if (g_flag.control_mode == RC_MODE && g_flag.control_target == MANUAL_LAND_MODE)  //处于遥控模式，控制手动登岛
		{
			vx = 0;
			
			if (rc_ctrl.rc.ch1 > 1044 || rc_ctrl.rc.ch1 < 1004)
				vy = (1024 - rc_ctrl.rc.ch1) * 10.0f;
			else 
				vy = 0;
			
			if (rc_ctrl_last.rc.s2 == 3)              //进入此模式时记录陀螺仪位置
				chassis_pos_follow_pid.SetPoint = yaw_angle;
			if (g_flag.gyro_use_flag)	//使用陀螺仪数据
			{
				chassis_vel_follow_pid.SetPoint = LIMIT_MAX_MIN(PID_Calc(&chassis_pos_follow_pid, yaw_angle), 5.7f, -5.7f);
				vw = PID_Calc(&chassis_vel_follow_pid, gz);
			}
			else						//不使用陀螺仪数据
				vw = 0;
		}
		else if (g_flag.control_mode == RC_MODE && g_flag.control_target == MAGAZINE_MODE)    //处于遥控模式，控制上层
		{
			vx = vy = vw = 0;
		}
		else if (g_flag.control_mode == KEY_MODE)
		{
			vx = vy = vw = 0;
		}
		else 
			vx = vy = vw = 0;
		
		chassis_task(vx, vy, vw);	
	}
	
}
