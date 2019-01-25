/************************************************************************************
ģʽ�ж�
�ж��߼�����Ҫ��Ϊ�����֣���һ�����ǿ���ģʽ����Ϊң��ģʽ������ģʽ������ģʽ��
�Զ��ǵ�ģʽ�Լ��Զ�ȡ��ģʽ
�ڶ�����Ϊ����Ŀ�꣬���ң��ģʽʱң�صĲ��֣���������ƶ��������������ϲ�������
�ϲ���������Ԯ��е�ֵȵȣ�ң����״̬���ޣ���ʱ�޸�
***************************************************************************************/

#include "stm32f4xx.h"
#include "main.h"
#include "uart5.h"
#include "pid.h"
#include "can2.h"

extern pid_Typedef chassis_pos_follow_pid;

/**
  * @brief  ��·���߼��
  * @param  None
  * @retval ���ض���id
  */
u8 offline_detect(void)
{
/**************CAN1���߼��*******************/
	if (g_flag.can1_connect == 0)
		g_flag.can1_cnt ++;
	
	if (g_flag.can1_cnt > 100)
		return 1;
	
/**************CAN2���߼��*******************/    
    if (g_flag.can2_connect == 0)
        g_flag.can2_cnt ++;
    
    if (g_flag.can2_cnt > 100)
        return 2;	
	
/**************ң�������߼��*******************/
    if (g_flag.rc_connect == 0)
        g_flag.rc_cnt ++;
    
    if (g_flag.rc_cnt > 200)
	{
        UART5_Configuration();        //����ң��������
		return 3;
	}

/**************�����Ƕ��߼��*******************/
	if (g_flag.gyro_connect == 0)
		g_flag.gyro_cnt ++;
	
	if (g_flag.gyro_cnt > 50)
	{
		g_flag.gyro_use_flag = 0;    //����
		return 4;
	}
	else
		g_flag.gyro_use_flag = 1;    //��������
	
/**************��������*******************/
	return 0;
}

/**
  * @brief  ģʽ�ж�
  * @param  None
* @retval s1: 1:����ģʽ
			  2:�ϵ�ģʽ
			  3:ң��ģʽ
  */
void switch_mode(void)
{
	if (rc_ctrl.rc.s1 == 3)            //ң��ģʽ
	{
		g_flag.control_mode = RC_MODE;
		
		if (rc_ctrl_last.rc.s1 == 2)
			chassis_pos_follow_pid.SetPoint = yaw_angle;
		
		if (rc_ctrl.rc.s2 == 3)                           //��������
			g_flag.control_target = CHASSIS_MODE;         //���Ƶ���
		else if (rc_ctrl.rc.s2 == 2)                      //��������
			g_flag.control_target = MANUAL_LAND_MODE;     //�ֶ����Ƶǵ�
		else if (rc_ctrl.rc.s2 == 1) 
			g_flag.control_target = MAGAZINE_MODE;        //�����ϲ�
	}
	else if (rc_ctrl.rc.s1 == 2)      //�ϵ�ģʽ
	{
		g_flag.control_mode = POWER_OFF;
	}
	else if (rc_ctrl.rc.s1 == 1)      //����ģʽ
	{
		if (rc_ctrl.rc.s2 == 2)       //�����Զ��ǵ�
		{
			if (rc_ctrl_last.rc.s1 != 1 || (rc_ctrl_last.key.d == 0 && rc_ctrl.key.d == 1 && rc_ctrl.key.ctrl == 1))
				g_flag.control_mode = KEY_MODE;
			
			if (rc_ctrl_last.key.b == 0 && rc_ctrl.key.b == 1 && rc_ctrl.key.ctrl == 1)
				g_flag.control_mode = INITIALIZE_MODE;
		}
		else if (rc_ctrl.rc.s2 == 3)  //�����Զ��ǵ�
		{
			if (rc_ctrl_last.key.b == 0 && rc_ctrl.key.b == 1 && rc_ctrl.key.ctrl == 1)
				g_flag.control_mode = INITIALIZE_MODE;
			if (rc_ctrl_last.key.r == 0 && rc_ctrl.key.r == 1 && rc_ctrl.key.ctrl == 1)
			{
				g_flag.control_mode = LANDING_ON;
				g_flag.landing_state = STATE0;
			}
		}
		else if (rc_ctrl.rc.s2 == 1)
		{
		}
	}
}
