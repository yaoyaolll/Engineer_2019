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
int ms50, tick50;						    //��Ƶ��ʱ
int cur_error_num, last_error_num;			//������                      

 /**
  * @brief  ������
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
  * @brief  ϵͳ��ʱ�ж�
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
	
    cur_error_num = offline_detect();					//��ȡ������Ϣ 

//	if(cur_error_num == 0)								//����״̬��ʼ����
//    {
        TIM_Cmd(TIM2, DISABLE);
        LED_R_OFF;
        switch_mode();							 		//�жϿ���ģʽ
        if(g_flag.control_mode == RC_MODE || g_flag.control_mode == KEY_MODE)				//ң�ؼ�����ģʽ
        {
            chassis_cal();							//��������ƶ�
			track_cal(); 
			solenoid_action();
//			WOffset(0.2f);							//����ƫ����ת
        }
		else if(g_flag.control_mode == LANDING_ON)		//�ϵ�ģʽ
		{	
			landing_action();
		}
//		else if(Flag.ContralMode == LANDING_OFF)	//�µ�ģʽ
//		{	
//			MagTransform(MagDown, 0, 0);
//			LandingOffAction();						//�µ�״̬��
//		}
//		else if(Flag.ContralMode == GET_BULLET)		//ȡ��ģʽ
//		{
//			GetBulletAction();			
//		}
//		else if(Flag.ContralMode == FEED_MODE)		//Ӣ�۲���ģʽ
//		{
//			MagTransform(MagDown, HandUp, 0);
//			LegExpand(FrontUp, BehindUp, 115, 180, 1);
//			ALLmove(0, 0, 0);   
//			if(RC_Ctl.mouse.press_l == 1)			//�򿪵���
//			{
//				MAGAZINE_OPEN;
//				MAGAZINE_PROTECT_OFF;
//				solenoidState |= 0X01;
//			}
//		}
//		else if(Flag.ContralMode == FEED_MODE1)		//����վ����ģʽ
//		{
//			MagTransform(MagDown, HandUp, 0);
//			LegExpand(FrontUp, BehindUp, 20, 120, 1);
//			ALLmove(0, 0, 0);   
//			if(RC_Ctl.mouse.press_l == 1)			//�򿪵���
//			{
//				MAGAZINE_OPEN;
//				MAGAZINE_PROTECT_OFF;
//				solenoidState |= 0X3E;
//			}
//		}
		else if(g_flag.control_mode == POWER_OFF)		//�ϵ�ģʽ
		{
			chassis_power_off();
		}
//		else if(Flag.ContralMode == GET_SMALL_BULLET)	//ȡС����		
//		{
//			GetSmallBulletAction();
//		}
//		else if(Flag.ContralMode == RESCUE_UP)		//��ԮӢ��
//		{
//			MagTransform(MagDown, HandUp, 0);
//			LegExpand(FrontUp, BehindUp, 10, 10, 1);
//			if(ExpandReceive[0].cycle <= -10 && ExpandReceive[1].cycle <= -10 && ExpandReceive[2].cycle <= -10 && ExpandReceive[3].cycle <= -10)
//				Flag.ContralMode = KEY_MODE;
//		}
//		else if(Flag.ContralMode == INITAL_MODE)		//�ָ�����״̬
//		{
//			ALLmove(0, 0, 0);
//			MagTransform(MagDown, HandUp, 0);
//			LegExpand(FrontDown, BehindDown, 0, 0, 0);
//			ASSIST_LEG_OFF;
//			PAW_IN;
//			LEFT_PAW_CLOSE;
//			RIGHT_PAW_CLOSE;
//			if(TransformReceive[1].real_posion < 1500)	//����ȡ��������ʱ��
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
		
		key_refresh(); 										//���°���״̬
//    }
//	else if(cur_error_num != last_error_num)				//����״̬�ı�
//	{
//        TIM_Cmd(TIM2, ENABLE);								//������ʱ�������ƺ�ɫLED��˸
//		chassis_power_off();
////		ExpandSend(0, 0, 0, 0);
////		TransformSend(0, 0, 0, 0);
//    }
    last_error_num = cur_error_num;							//�ı�״̬�����ڼ�������Ϣ�ı�
	
	return_state();
}


/**
  * @brief  ϵͳ��ʼ��
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
//	KS103_INIT(KS103_DEVICE_ADDRESS1);				//��ʼ��������ģ��
//    KS103_INIT(KS103_DEVICE_ADDRESS2);
//	KS103_INIT(KS103_DEVICE_ADDRESS3);
//	Change_ID(KS103_DEVICE_ADDRESS1, KS103_DEVICE_ADDRESS2);		//����ID�ź�������Ҫʱȡ��ע�ͣ������KS103.h
	chassis_vel_pid_init(7.5f, 2.5f, 10.0f, 100.0f);
	track_vel_pid_init(17.5f, 5.0f, 0, 100.0f);
    LED_Configration();								//����LED������ɫLED���𣬽��빤��״̬

    SysTick_Config(168000);
}

