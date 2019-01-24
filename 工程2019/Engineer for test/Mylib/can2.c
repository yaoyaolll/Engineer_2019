/************************************************************************************
CAN2�����ϲ�������ȡ���ֲ����ƶ��Լ����������ݽ��գ�ID�����£�
					ͷ
					|
					|         0x203(ȡ���ֲ����ƶ�)
					|		  
	0x201(����)		|		  0x202(����)
					|
				    β

���������ݽ��գ�0x301��0x302
***************************************************************************************/

#include "stm32f4xx.h"
#include "can1.h"
#include "can2.h"
#include "main.h"
#include "pid.h"
#include "float2byte.h"

rmc620_t lift_motor[2];       //�ϲ��������
rmc620_t lateral_motor;       //ȡ���ֲ����ƶ����
float yaw_angle;
float gz;

extern pid_Typedef chassis_pos_follow_pid;

/**
  * @brief  ����CAN2
  * @param  None
  * @retval None
  */
void CAN2_Configration(void)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2); 

    gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12 ;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &gpio);

    nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    nvic.NVIC_IRQChannel = CAN2_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);  

    CAN_DeInit(CAN2);
    CAN_StructInit(&can);

    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = ENABLE;    
    can.CAN_AWUM = DISABLE;    
    can.CAN_NART = ENABLE;    
    can.CAN_RFLM = DISABLE;    
    can.CAN_TXFP = ENABLE;     
    can.CAN_Mode = CAN_Mode_Normal;
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1 = CAN_BS1_9tq;
    can.CAN_BS2 = CAN_BS2_4tq;
    can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN2, &can);

    can_filter.CAN_FilterNumber = 15; 
    can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
    can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
    can_filter.CAN_FilterIdHigh = 0x0000;
    can_filter.CAN_FilterIdLow = 0x0000;
    can_filter.CAN_FilterMaskIdHigh = 0x0000;
    can_filter.CAN_FilterMaskIdLow = 0x0000;
    can_filter.CAN_FilterFIFOAssignment = 0;//fifo0
    can_filter.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&can_filter);

    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
    CAN_ITConfig(CAN2, CAN_IT_TME, ENABLE); 
}

/**
  * @brief  CAN2�����ж�
  * @param  None
  * @retval None
  */
int can2tx;
void CAN2_TX_IRQHandler(void)
{
    can2tx++;
    if (CAN_GetITStatus(CAN2,CAN_IT_TME) != RESET) 
    {
        CAN_ClearITPendingBit(CAN2, CAN_IT_TME);
    }
}


/**
  * @brief  CAN2�����ж�
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0) != RESET)
    {
        CAN_Receive(CAN2, CAN_FIFO0, &rx_message);
        switch(rx_message.StdId)
        {
            case 0x201:                                                          
                lift_motor[0].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
                lift_motor[0].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
				if ((lift_motor[0].angle - lift_motor[0].last_angle) > 5000)
				{
					lift_motor[0].cycle++;
				}
				if ((lift_motor[0].angle - lift_motor[0].last_angle) < -5000)
				{
					lift_motor[0].cycle--;
				}
				lift_motor[0].last_angle = lift_motor[0].angle;
            break;
            
            case 0x202:
                lift_motor[1].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
                lift_motor[1].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
				if ((lift_motor[1].angle - lift_motor[1].last_angle) > 5000)
				{
					lift_motor[1].cycle++;
				}
				if ((lift_motor[1].angle - lift_motor[1].last_angle) < -5000)
				{
					lift_motor[1].cycle--; 
				}
				lift_motor[1].real_posion = (-lift_motor[1].cycle * 8192.0f + lift_motor[1].angle) / 20.0f;
				lift_motor[1].last_angle = lift_motor[1].angle;
            break;
            
            case 0x203:
                lateral_motor.angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
                lateral_motor.speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
				if ((lateral_motor.angle - lateral_motor.last_angle) > 5000)
				{
					lateral_motor.cycle++;
				}
				if ((lateral_motor.angle - lateral_motor.last_angle) < -5000)
				{
					lateral_motor.cycle--;
				}
				lateral_motor.last_angle = lateral_motor.angle;
            break;
            
            case 0x301:
				byte2float(rx_message.Data, 4, &yaw_angle);
				g_flag.gyro_cnt = 0;
				g_flag.gyro_connect = 0;
				if(g_flag.initial_flag == 0)
				{
					g_flag.initial_flag = 1;
					chassis_pos_follow_pid.SetPoint = yaw_angle;
				}
			break;
			
			case 0x302:
				byte2float(rx_message.Data, 4, &gz);
			break;
			
			default:
			break;
		}
		
		g_flag.can2_cnt = 0;
		g_flag.can2_connect = 0;
        
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);   
    }
	
}

/**
  * @brief  CAN2�������� 0x200������������ƶ����
  * @param  a��0x201��������
            b��0x202��������
            c��0x203��������
            d��0x204��������
  * @retval None
  */
void can2_current_send(int a, int b, int c, int d)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = RM820R_0x204_1;
	
    a = LIMIT_MAX_MIN(a, 15000, -15000);
    b = LIMIT_MAX_MIN(b, 15000, -15000);
    c = LIMIT_MAX_MIN(c, 15000, -15000);
    d = LIMIT_MAX_MIN(d, 15000, -15000);
	  
	
    tx_message.Data[0] = (unsigned char)((a >> 8) & 0xff);
    tx_message.Data[1] = (unsigned char)(a & 0xff);  
    tx_message.Data[2] = (unsigned char)((b >> 8) & 0xff);
    tx_message.Data[3] = (unsigned char)(b & 0xff);
    tx_message.Data[4] = (unsigned char)((c >> 8) & 0xff);
    tx_message.Data[5] = (unsigned char)(c & 0xff);
    tx_message.Data[6] = (unsigned char)((d >> 8) & 0xff); 
    tx_message.Data[7] = (unsigned char)(d & 0xff);

    CAN_Transmit(CAN2, &tx_message);
}

