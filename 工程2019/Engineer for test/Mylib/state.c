/************************************************************************************
模式判断
判断逻辑：主要分为两部分，第一部分是控制模式，分为遥控模式，键盘模式，掉电模式，
自动登岛模式以及自动取弹模式
第二部分为控制目标，针对遥控模式时遥控的部分，比如底盘移动，底盘升降，上层升降，
上层伸缩，救援机械手等等，遥控器状态有限，随时修改
***************************************************************************************/

#include "stm32f4xx.h"
#include "main.h"
#include "uart5.h"
#include "pid.h"
#include "can2.h"

extern pid_Typedef chassis_pos_follow_pid;

/**
  * @brief  线路断线检测
  * @param  None
  * @retval 返回断线id
  */
u8 offline_detect(void)
{
/**************CAN1断线检测*******************/
	if (g_flag.can1_connect == 0)
		g_flag.can1_cnt ++;
	
	if (g_flag.can1_cnt > 100)
		return 1;
	
/**************CAN2断线检测*******************/    
    if (g_flag.can2_connect == 0)
        g_flag.can2_cnt ++;
    
    if (g_flag.can2_cnt > 100)
        return 2;	
	
/**************遥控器断线检测*******************/
    if (g_flag.rc_connect == 0)
        g_flag.rc_cnt ++;
    
    if (g_flag.rc_cnt > 200)
	{
        UART5_Configuration();        //重启遥控器串口
		return 3;
	}

/**************陀螺仪断线检测*******************/
	if (g_flag.gyro_connect == 0)
		g_flag.gyro_cnt ++;
	
	if (g_flag.gyro_cnt > 50)
	{
		g_flag.gyro_use_flag = 0;    //掉线
		return 4;
	}
	else
		g_flag.gyro_use_flag = 1;    //数据正常
	
/**************数据正常*******************/
	return 0;
}

/**
  * @brief  模式判断
  * @param  None
* @retval s1: 1:键鼠模式
			  2:断电模式
			  3:遥控模式
  */
void switch_mode(void)
{
	if (rc_ctrl.rc.s1 == 3)            //遥控模式
	{
		g_flag.control_mode = RC_MODE;
		
		if (rc_ctrl_last.rc.s1 == 2)
			chassis_pos_follow_pid.SetPoint = yaw_angle;
		
		if (rc_ctrl.rc.s2 == 3)
			g_flag.control_target = CHASSIS_MODE;
		else if (rc_ctrl.rc.s2 == 1)
			g_flag.control_target = MANUAL_LAND_MODE;     
		else if (rc_ctrl.rc.s2 == 2)
			g_flag.control_target = MAGAZINE_MODE;
	}
	else if (rc_ctrl.rc.s1 == 2)      //断电模式
	{
		g_flag.control_mode = POWER_OFF;
	}
	else if (rc_ctrl.rc.s1 == 1)      //键鼠模式
	{
		if (rc_ctrl.rc.s2 == 2)
		{
		}
		else if (rc_ctrl.rc.s2 == 3)
		{
		}
		else if (rc_ctrl.rc.s2 == 1)
		{
		}
	}
}
