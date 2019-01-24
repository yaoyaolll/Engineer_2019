#include "stm32f4xx.h"
#include "main.h"
#include "led.h"

int led_cnt;

/**
  * @brief  红蓝LED灯配置
  * @param  None
  * @retval None
  */
void LED_Configration(void)
{
    GPIO_InitTypeDef gpio;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &gpio);

    LED_G_ON;
    LED_R_OFF;
}

/**
  * @brief  TIM2初始化，用作断线测试
  * @param  None
  * @retval None
  */
void TIM2_Configration(void)
{
	TIM_TimeBaseInitTypeDef TIM_2;
	NVIC_InitTypeDef NVIC_TIM;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
    
	NVIC_TIM.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_TIM.NVIC_IRQChannelCmd = ENABLE;
	NVIC_TIM.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_TIM.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_TIM);
	
	TIM_2.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_2.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_2.TIM_Period = 999;
	TIM_2.TIM_Prescaler = 15000 - 1;
	TIM_TimeBaseInit(TIM2, &TIM_2);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, DISABLE);
}

/**
  * @brief  TIM2中断函数，根据错误号闪烁不同次数
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
        if(led_cnt < (cur_error_num * 2 - 1))
        {
            LED_R_TOGG;
            led_cnt++;
        }
        else if(led_cnt < (cur_error_num * 2 + 3))
        {
            LED_R_OFF;
            led_cnt++;
        }
        else if(led_cnt == (cur_error_num * 2 + 3))
        {
            led_cnt = 0;
        }
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

