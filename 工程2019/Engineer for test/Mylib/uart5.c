#include "stm32f4xx.h"
#include "main.h"
#include "uart5.h"

volatile unsigned char sbus_rx_buffer[18];
rc_ctrl_t rc_ctrl, rc_ctrl_last; 

/**
  * @brief  UART5配置
  * @param  None
  * @retval None
  */
void UART5_Configuration(void)
{
    USART_InitTypeDef usart5;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOC,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);

    GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); 

    gpio.GPIO_Pin = GPIO_Pin_2;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD,&gpio);
	
	gpio.GPIO_Pin = GPIO_Pin_12;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC,&gpio);
	
	PCout(12) = 1;	//PC12 wifi断电重启 0：关 1：开

    usart5.USART_BaudRate = 100000;
    usart5.USART_WordLength = USART_WordLength_8b;
    usart5.USART_StopBits = USART_StopBits_1;
    usart5.USART_Parity = USART_Parity_Even;
    usart5.USART_Mode = USART_Mode_Rx;
    usart5.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(UART5,&usart5);

    USART_ITConfig(UART5,USART_IT_IDLE,ENABLE);
    USART_Cmd(UART5,ENABLE);
    USART_DMACmd(UART5,USART_DMAReq_Rx,ENABLE);

    nvic.NVIC_IRQChannel = UART5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 3;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
    {
        DMA_InitTypeDef   dma;
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
        DMA_DeInit(DMA1_Stream0);
        dma.DMA_Channel= DMA_Channel_4;
        dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART5->DR);
        dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
        dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
        dma.DMA_BufferSize = RX_UART5_BUFFER;
        dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
        dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
        dma.DMA_Mode = DMA_Mode_Circular;
        dma.DMA_Priority = DMA_Priority_VeryHigh;
        dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
        dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
        dma.DMA_MemoryBurst = DMA_Mode_Normal;
        dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        DMA_Init(DMA1_Stream0, &dma);
        DMA_ITConfig(DMA1_Stream0,DMA_IT_TC,ENABLE);
        DMA_Cmd(DMA1_Stream0, ENABLE);
    }
}

/**
  * @brief  UART5中断服务函数
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void)
{
	static int DATA_LENGTH=0;
	if (USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)
	{
        (void)UART5->SR;
        (void)UART5->DR;	
        DMA_Cmd(DMA1_Stream0,DISABLE);
        DATA_LENGTH=RX_UART5_BUFFER-DMA_GetCurrDataCounter(DMA1_Stream0);
        if(DATA_LENGTH==18)
        {
            remote_receive(sbus_rx_buffer);
            g_flag.rc_cnt = 0;
            g_flag.rc_connect = 1;
        }
        DMA_SetCurrDataCounter(DMA1_Stream0,RX_UART5_BUFFER);	
        DMA_Cmd(DMA1_Stream0,ENABLE);
    }
}

/**
  * @brief  遥控器数据清零
  * @param  None
  * @retval None
  */
void rc_reset(void)
{
    rc_ctrl.rc.ch0 = 1024;
    rc_ctrl.rc.ch1 = 1024;
    rc_ctrl.rc.ch2 = 1024;
    rc_ctrl.rc.ch3 = 1024;
    rc_ctrl.mouse.x = 0;
    rc_ctrl.mouse.y = 0;
    rc_ctrl.mouse.z = 0;
    rc_ctrl.mouse.press_l = 0;                                                
    rc_ctrl.mouse.press_r = 0;

    rc_ctrl.key.w = 0;
    rc_ctrl.key.s = 0;                            
    rc_ctrl.key.a = 0;
    rc_ctrl.key.d = 0;
    rc_ctrl.key.q = 0;
    rc_ctrl.key.e = 0;
    rc_ctrl.key.r = 0;
    rc_ctrl.key.f = 0;
    rc_ctrl.key.shift = 0;
    rc_ctrl.key.ctrl = 0;

    rc_ctrl.rc.s1 = 2;
    rc_ctrl.rc.s2 = 2;
}   

/**
  * @brief  遥控器数据解码
  * @param  None
  * @retval None
  */
void remote_receive(volatile unsigned char rx_buffer[])
{
	rc_ctrl.rc.ch0 = (rx_buffer[0]| (rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0
	rc_ctrl.rc.ch1 = ((rx_buffer[1] >> 3) | (rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
	rc_ctrl.rc.ch2 = ((rx_buffer[2] >> 6) | (rx_buffer[3] << 2) | (rx_buffer[4] << 10)) & 0x07ff;//!< Channel 2
	rc_ctrl.rc.ch3 = ((rx_buffer[4] >> 1) | (rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
	rc_ctrl.rc.s1 = ((rx_buffer[5] >> 4)& 0x0003); //!< Switch left
	rc_ctrl.rc.s2 = ((rx_buffer[5] >> 6)& 0x0003);

	rc_ctrl.mouse.x = rx_buffer[6] | (rx_buffer[7] << 8); //!< Mouse X axis
	rc_ctrl.mouse.y = rx_buffer[8] | (rx_buffer[9] << 8); //!< Mouse Y axis
	rc_ctrl.mouse.z = rx_buffer[10] | (rx_buffer[11] << 8); //!< Mouse Z axis
	rc_ctrl.mouse.press_l = rx_buffer[12]; //!< Mouse Left Is Press ?
	rc_ctrl.mouse.press_r = rx_buffer[13]; //!< Mouse Right Is Press ?

	rc_ctrl.key.w = rx_buffer[14]&0x01; // KeyBoard value
	rc_ctrl.key.s = (rx_buffer[14]>>1)&0x01;
	rc_ctrl.key.a = (rx_buffer[14]>>2)&0x01;
	rc_ctrl.key.d = (rx_buffer[14]>>3)&0x01;
	rc_ctrl.key.shift = (rx_buffer[14]>>4)&0x01;
	rc_ctrl.key.ctrl = (rx_buffer[14]>>5)&0x01;
	rc_ctrl.key.q = (rx_buffer[14]>>6)&0x01;
	rc_ctrl.key.e = (rx_buffer[14]>>7)&0x01;	
	rc_ctrl.key.r = (rx_buffer[15])&0x01;
	rc_ctrl.key.f = (rx_buffer[15]>>1)&0x01;
	rc_ctrl.key.g = (rx_buffer[15]>>2)&0x01; 
	rc_ctrl.key.z = (rx_buffer[15]>>3)&0x01;
	rc_ctrl.key.x = (rx_buffer[15]>>4)&0x01;
	rc_ctrl.key.c = (rx_buffer[15]>>5)&0x01;
	rc_ctrl.key.v = (rx_buffer[15]>>6)&0x01;
	rc_ctrl.key.b = (rx_buffer[15]>>7)&0x01;
}

/**
  * @brief  改变状态，用于检测拨杆，更新状态机初始状态
  * @param  None
  * @retval None
  */
void key_refresh(void)
{
	rc_ctrl_last.rc.s1 = rc_ctrl.rc.s1;				
	rc_ctrl_last.rc.s2 = rc_ctrl.rc.s2;
	rc_ctrl_last.key.g = rc_ctrl.key.g;
	rc_ctrl_last.key.b = rc_ctrl.key.b;
	rc_ctrl_last.key.z = rc_ctrl.key.z;
	rc_ctrl_last.key.x = rc_ctrl.key.x;
	rc_ctrl_last.key.c = rc_ctrl.key.c;
	rc_ctrl_last.key.d = rc_ctrl.key.d;
	rc_ctrl_last.key.s = rc_ctrl.key.s;
	rc_ctrl_last.key.f = rc_ctrl.key.f;
	rc_ctrl_last.key.a = rc_ctrl.key.a;
	rc_ctrl_last.key.q = rc_ctrl.key.q;
	rc_ctrl_last.key.e = rc_ctrl.key.e;
	rc_ctrl_last.key.r = rc_ctrl.key.r;
	rc_ctrl_last.key.w = rc_ctrl.key.w;
	rc_ctrl_last.key.v = rc_ctrl.key.v;
}

