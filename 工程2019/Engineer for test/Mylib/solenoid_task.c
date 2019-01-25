#include "main.h"
#include "uart5.h"
#include "solenoid_task.h"

/**
  * @brief  电磁阀配置
  * @param  None
  * @retval None
  */
void solenoid_config()
{
    GPIO_InitTypeDef gpio;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &gpio);

    gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &gpio);
	
	gpio.GPIO_Pin = GPIO_Pin_5;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &gpio);
	
	LIGHT_SWITCH_ON;
}

/**
  * @brief  电磁阀控制
  * @param  None
  * @retval None
  */
void solenoid_action(void)
{
	if(g_flag.control_mode == RC_MODE && g_flag.control_target == CHASSIS_MODE)					//处于遥控模式，控制底盘及补弹气缸
	{
		if (rc_ctrl.rc.ch3 > 1500)
			FILL_BULLET_OPEN;
		else if (rc_ctrl.rc.ch3 < 500)
			FILL_BULLET_CLOSE;
	}
	else if (g_flag.control_mode == RC_MODE && g_flag.control_target == MANUAL_LAND_MODE)    	//处于遥控模式，控制手动登岛
	{
		if (rc_ctrl.rc.ch2 > 1500)
		{
			ASSIST_LEG_ON;													//打开登岛辅助腿
		}
		else if (rc_ctrl.rc.ch2 < 500)
		{
			ASSIST_LEG_OFF;													//关闭登岛辅助腿
		}
	} 
	else if (g_flag.control_mode == RC_MODE && g_flag.control_target == MAGAZINE_MODE)          //处于遥控模式，控制上层
	{
		if (rc_ctrl.rc.ch2 > 1500)
			UP_PAW_OPEN;													//取弹手张开
		else if (rc_ctrl.rc.ch2 < 500)
			UP_PAW_CLOSE;													//取弹手关闭
		
		if (rc_ctrl.rc.ch3 > 1500)											//旋转气缸张开
			ROTARY_CYLINDER_OPEN;
		else if (rc_ctrl.rc.ch3 < 500)
			ROTARY_CYLINDER_CLOSE;											//旋转气缸关闭
	}
}
