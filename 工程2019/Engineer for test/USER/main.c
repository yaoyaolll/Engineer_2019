#include <main.h>
#include "can1.h"
#include "can2.h"
#include "state.h"
#include "led.h"
#include "delay.h"
#include "usart3.h"
#include "uart5.h"
#include "chassis_task.h"
#include "track_task.h"
#include "solenoid_task.h"
#include "landing_task.h"

flag_t g_flag;
int ms50, tick50;						    //分频定时
int cur_error_num, last_error_num;			//错误码                      

 /**
  * @brief  主函数
  * @param  None
  * @retval None
  */
int main()
{
	System_Configration();
while(1)
	{
		
	}
}

/**
  * @brief  系统定时中断
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	if(ms50 < 49)
		ms50++;
	else
	{
		ms50 = 0;
		tick50 = 1;
	}
	
    cur_error_num = offline_detect();					//读取断线信息 

//	if(cur_error_num == 0)								//正常状态开始工作
//    {
        TIM_Cmd(TIM2, DISABLE);
        LED_R_OFF;
        switch_mode();							 		//判断控制模式
        if(g_flag.control_mode == RC_MODE || g_flag.control_mode == KEY_MODE)				//遥控及键鼠模式
        {
            chassis_cal();							//计算底盘移动
			track_cal(); 
			solenoid_action();
//			WOffset(0.2f);							//快速偏置旋转
        }
		else if(g_flag.control_mode == LANDING_ON)		//上岛模式
		{	
			landing_action();
		}
//		else if(Flag.ContralMode == LANDING_OFF)	//下岛模式
//		{	
//			MagTransform(MagDown, 0, 0);
//			LandingOffAction();						//下岛状态机
//		}
//		else if(Flag.ContralMode == GET_BULLET)		//取弹模式
//		{
//			GetBulletAction();			
//		}
//		else if(Flag.ContralMode == FEED_MODE)		//英雄补给模式
//		{
//			MagTransform(MagDown, HandUp, 0);
//			LegExpand(FrontUp, BehindUp, 115, 180, 1);
//			ALLmove(0, 0, 0);   
//			if(RC_Ctl.mouse.press_l == 1)			//打开弹仓
//			{
//				MAGAZINE_OPEN;
//				MAGAZINE_PROTECT_OFF;
//				solenoidState |= 0X01;
//			}
//		}
//		else if(Flag.ContralMode == FEED_MODE1)		//补给站补给模式
//		{
//			MagTransform(MagDown, HandUp, 0);
//			LegExpand(FrontUp, BehindUp, 20, 120, 1);
//			ALLmove(0, 0, 0);   
//			if(RC_Ctl.mouse.press_l == 1)			//打开弹仓
//			{
//				MAGAZINE_OPEN;
//				MAGAZINE_PROTECT_OFF;
//				solenoidState |= 0X3E;
//			}
//		}
		else if(g_flag.control_mode == POWER_OFF)		//断电模式
		{
			chassis_power_off();
		}
//		else if(Flag.ContralMode == GET_SMALL_BULLET)	//取小弹丸		
//		{
//			GetSmallBulletAction();
//		}
//		else if(Flag.ContralMode == RESCUE_UP)		//救援英雄
//		{
//			MagTransform(MagDown, HandUp, 0);
//			LegExpand(FrontUp, BehindUp, 10, 10, 1);
//			if(ExpandReceive[0].cycle <= -10 && ExpandReceive[1].cycle <= -10 && ExpandReceive[2].cycle <= -10 && ExpandReceive[3].cycle <= -10)
//				Flag.ContralMode = KEY_MODE;
//		}
//		else if(Flag.ContralMode == INITAL_MODE)		//恢复所有状态
//		{
//			ALLmove(0, 0, 0);
//			MagTransform(MagDown, HandUp, 0);
//			LegExpand(FrontDown, BehindDown, 0, 0, 0);
//			ASSIST_LEG_OFF;
//			PAW_IN;
//			LEFT_PAW_CLOSE;
//			RIGHT_PAW_CLOSE;
//			if(TransformReceive[1].real_posion < 1500)	//控制取弹手在外时打开
//			{
//				UP_PAW_CLOSE;
//				solenoidState &= 0X3D;
//			}
//			else if(TransformReceive[1].real_posion >= 1500)
//			{
//				UP_PAW_OPEN;
//				solenoidState |= 0X02;
//			}
//			LIGHT_SWITCH_OFF;
//			Flag.gyroFlag = 1;
//			MAGAZINE_CLOSE;
//			if(Flag.ExpandFlagDown[0] == 1 && Flag.ExpandFlagDown[1] == 1 && Flag.ExpandFlagDown[2] == 1 && Flag.ExpandFlagDown[3] == 1 && SWITCH_DOWN == 0 && SWITCH_HAND_IN == 0)
//			{
//				MAGAZINE_PROTECT_ON;
//				Flag.ContralMode = KEY_MODE;
//			}
//		}
		
		key_refresh(); 										//更新按键状态
//    }
//	else if(cur_error_num != last_error_num)				//断线状态改变
//	{
//        TIM_Cmd(TIM2, ENABLE);								//启动定时器，控制红色LED闪烁
//		chassis_power_off();
////		ExpandSend(0, 0, 0, 0);
////		TransformSend(0, 0, 0, 0);
//    }
    last_error_num = cur_error_num;							//改变状态，用于检测断线信息改变
	
	return_state();
}


/**
  * @brief  系统初始化
  * @param  None
  * @retval None
  */

void System_Configration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    TIM2_Configration();
//	TIM5_Configration();
    TIM6_Configration();
    TIM7_Configration();    
    CAN1_Configration();
    CAN2_Configration();
	
	USART3_Configuration();
	UART5_Configuration();
//	UART4_Configuration();
	rc_reset();
	
    delay_ms(5000);

    solenoid_config();
//	switch_config();
//	ServoConfiguration();
//    
//	Adc_Init();
	
//    ChassisPidInit();
//	FollowPidInit();
//    ExpandPidInit();
//	TransformPidInit();
	
//    IIC_Init();
//	KS103_INIT(KS103_DEVICE_ADDRESS1);				//初始化超声波模块
//    KS103_INIT(KS103_DEVICE_ADDRESS2);
//	KS103_INIT(KS103_DEVICE_ADDRESS3);
//	Change_ID(KS103_DEVICE_ADDRESS1, KS103_DEVICE_ADDRESS2);		//更改ID号函数，需要时取消注释，具体见KS103.h
	chassis_vel_pid_init(7.5f, 2.5f, 10.0f, 100.0f);
	track_vel_pid_init(17.5f, 5.0f, 0, 100.0f);
    LED_Configration();								//配置LED，当蓝色LED亮起，进入工作状态

    SysTick_Config(168000);
}

