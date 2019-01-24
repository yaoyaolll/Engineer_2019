#ifndef __LED_H__
#define __LED_H__

#define LED_G_OFF GPIO_SetBits(GPIOA, GPIO_Pin_9)
#define LED_G_ON GPIO_ResetBits(GPIOA, GPIO_Pin_9)
#define LED_G_TOGG GPIO_ToggleBits(GPIOA, GPIO_Pin_9)

#define LED_R_OFF GPIO_SetBits(GPIOA, GPIO_Pin_10)
#define LED_R_ON GPIO_ResetBits(GPIOA, GPIO_Pin_10)
#define LED_R_TOGG GPIO_ToggleBits(GPIOA, GPIO_Pin_10)
void LED_Configration(void);
void TIM2_Configration(void);


#endif
