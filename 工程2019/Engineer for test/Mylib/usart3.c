#include "stm32f4xx.h"
#include "main.h"
#include "usart3.h"
#include "algorithmOfCRC.h"

unsigned char judge_receive_buf[JudgeBufBiggestSize];
unsigned char judge_send[22];
unsigned char SaveBuffer[68];
float_long_t chassis_a,chassis_v,chassis_energy;
float_long_t UserMes;
game_inf_t game_inf;
short data_len;
unsigned char judge_send[22];

/**
  * @brief  ??3??,???????
  * @param  None
  * @retval None
  */
void USART3_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);


	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); 

	gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&gpio);

	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3,&usart);

	//USART_ITConfig(USART3, USART_IT_IDLE ,ENABLE);
	USART_Cmd(USART3,ENABLE);

	nvic.NVIC_IRQChannel = DMA1_Stream1_IRQn;
	//		nvic.NVIC_IRQChannel =USART3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 3;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	{
		DMA_InitTypeDef 	dma;
		DMA_DeInit(DMA1_Stream1);
		dma.DMA_Channel= DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)judge_receive_buf;
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
		dma.DMA_BufferSize =JudgeBufBiggestSize;
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
		DMA_Init(DMA1_Stream1,&dma);
		DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Stream1,ENABLE);
	}	

	nvic.NVIC_IRQChannel = DMA1_Stream3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 3;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	{
		DMA_InitTypeDef 	dma;
		DMA_DeInit(DMA1_Stream3);
		dma.DMA_Channel= DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)judge_send;
		dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		dma.DMA_BufferSize = 22;
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
		DMA_Init(DMA1_Stream3,&dma);
		DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Stream3,DISABLE);
	}		
}

/**
  * @brief  USART3中断
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{   
	if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		(void)USART3->SR;
		(void)USART3->DR;
		DMA_Cmd(DMA1_Stream1,DISABLE);
		data_len = JudgeBufBiggestSize - DMA_GetCurrDataCounter(DMA1_Stream1);
		judge_buf_receive(judge_receive_buf,0);

		DMA_SetCurrDataCounter(DMA1_Stream1,JudgeBufBiggestSize);
		USART_ClearITPendingBit(USART3,USART_IT_IDLE);
		USART_ClearFlag(USART3,USART_FLAG_IDLE);
		DMA_Cmd(DMA1_Stream1,ENABLE);
	}	
}

/**
  * @brief  DMA中断
  * @param  None
  * @retval None
  */
void DMA1_Stream1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream1, DMA_IT_TCIF1))
	{
		DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);
		DMA_ClearITPendingBit(DMA1_Stream1, DMA_IT_TCIF1);
		judge_buf_receive(judge_receive_buf,0);
	}
}
void DMA1_Stream3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3)!=RESET)
	{
		DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);
		DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
		DMA_Cmd(DMA1_Stream3, DISABLE);
	}	
}

/**
  * @brief  裁判系统接收数据，解码冗余
  * @param  None
  * @retval None
  */
void judge_buf_receive(unsigned char receive_buf[], uint16_t data_len)
{
	uint8_t cmd_id;
	short PackPoint;
	memcpy(&SaveBuffer[JudgeBufBiggestSize],&receive_buf[0],JudgeBufBiggestSize);
	for(PackPoint=0;PackPoint<JudgeBufBiggestSize;PackPoint++)
	{
		if(SaveBuffer[PackPoint]==0xA5)
		{	
			if((Verify_CRC8_Check_Sum(&SaveBuffer[PackPoint],5)==1))//&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],data_len)==1))
			{
				cmd_id=(SaveBuffer[PackPoint+6])&0xff;
				cmd_id=(cmd_id<<8)|SaveBuffer[PackPoint+5];
				data_len=SaveBuffer[PackPoint+2]&0xff;
				data_len=(data_len<<8)|SaveBuffer[PackPoint+1];
				if((cmd_id==0x0001)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],data_len+9)))
				{
					game_inf.robot_level=SaveBuffer[PackPoint+10];//这句强制转换有不确定
					game_inf.remain_HP=(SaveBuffer[PackPoint+12]<<8)|SaveBuffer[PackPoint+11];
					
				}
				if((cmd_id==0x0003)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],data_len+9)))
				{
					game_inf.bullet_freq=SaveBuffer[PackPoint+8];
					chassis_a.idata=SaveBuffer[PackPoint+12];
					chassis_a.idata=(chassis_a.idata<<8)|SaveBuffer[PackPoint+11];
					chassis_a.idata=(chassis_a.idata<<8)|SaveBuffer[PackPoint+10];
					chassis_a.idata=(chassis_a.idata<<8)|SaveBuffer[PackPoint+9];
					game_inf.bullet_speed=chassis_a.fdata;
				}
				if((cmd_id==0x0004)&&(Verify_CRC16_Check_Sum(&SaveBuffer[PackPoint],data_len+9)))
				{
					chassis_v.idata=SaveBuffer[PackPoint+10];
					chassis_v.idata=(chassis_v.idata<<8)|SaveBuffer[PackPoint+9];
					chassis_v.idata=(chassis_v.idata<<8)|SaveBuffer[PackPoint+8];
					chassis_v.idata=(chassis_v.idata<<8)|SaveBuffer[PackPoint+7];
					game_inf.real_chassis_outV=chassis_v.fdata;
					
					chassis_a.idata=SaveBuffer[PackPoint+14];
					chassis_a.idata=(chassis_a.idata<<8)|SaveBuffer[PackPoint+13];
					chassis_a.idata=(chassis_a.idata<<8)|SaveBuffer[PackPoint+12];
					chassis_a.idata=(chassis_a.idata<<8)|SaveBuffer[PackPoint+11];
					game_inf.real_chassis_outA=chassis_a.fdata;
					
					chassis_energy.idata=SaveBuffer[PackPoint+18];
					chassis_energy.idata=(chassis_energy.idata<<8)|SaveBuffer[PackPoint+17];
					chassis_energy.idata=(chassis_energy.idata<<8)|SaveBuffer[PackPoint+16];
					chassis_energy.idata=(chassis_energy.idata<<8)|SaveBuffer[PackPoint+15]; 
					game_inf.real_chassis_power=chassis_energy.fdata;
					
					chassis_energy.idata=SaveBuffer[PackPoint+22];
					chassis_energy.idata=(chassis_energy.idata<<8)|SaveBuffer[PackPoint+21];
					chassis_energy.idata=(chassis_energy.idata<<8)|SaveBuffer[PackPoint+20];
					chassis_energy.idata=(chassis_energy.idata<<8)|SaveBuffer[PackPoint+19]; 
					game_inf.remain_energy=chassis_energy.fdata;//剩余能量
					
					game_inf.shooter_heat17=SaveBuffer[PackPoint+24]&0xff;
					game_inf.shooter_heat17=(game_inf.shooter_heat17<<8)|(SaveBuffer[PackPoint+23]);
					game_inf.shooter_heat42=(SaveBuffer[PackPoint+26]<<8)|SaveBuffer[PackPoint+25];
				}
			}

		}
	}
	memcpy(&SaveBuffer[0],&SaveBuffer[JudgeBufBiggestSize],JudgeBufBiggestSize);
}

/**
  * @brief  裁判系统数据填入
  * @param  None
  * @retval None
  */
void judge_send_fill(float data1,float data2,float data3 ,unsigned char mask)
{	
	/*FrameHeader(5 Bytes)*/
//	unsigned char SOF = 0xA5;
	unsigned short DataLength = 13;
	static unsigned char Seq = 0;
	/*CmdID(2 Bytes)*/
	unsigned short UserID = 0x0100;   //The User's ID is 0x0100.
	judge_send[0] = 0xA5;
	judge_send[1] = (unsigned char)(DataLength&0xff);
	judge_send[2] = (unsigned char)((DataLength >> 8)&0xff);
	judge_send[3] =  Seq;  //包序号 0~255
	Append_CRC8_Check_Sum(judge_send, 5);
	//CmdID
	judge_send[5] = (unsigned char)((UserID&0xff)&0xff);
	judge_send[6] = (unsigned char)((UserID >> 8)&0xff);
	/*Data(13 Bytes)*/
	/*FrameTail(2 Bytes, CRC16)*/
	
	UserMes.fdata=data1;
	judge_send[10] = (UserMes.idata>>24)&0xff;
	judge_send[9] = (UserMes.idata>>16)&0xff;
	judge_send[8] = (UserMes.idata>>8)&0xff;
	judge_send[7] = UserMes.idata&0xff;
		
	UserMes.fdata=data2;
	judge_send[14] = (UserMes.idata>>24)&0xff;
	judge_send[13] = (UserMes.idata>>16)&0xff;
	judge_send[12] = (UserMes.idata>>8)&0xff;
	judge_send[11] = UserMes.idata&0xff;
		
	UserMes.fdata=data3;
	judge_send[18] = (UserMes.idata>>24)&0xff;
	judge_send[17] = (UserMes.idata>>16)&0xff;
	judge_send[16] = (UserMes.idata>>8)&0xff;
	judge_send[15] = UserMes.idata&0xff;

	judge_send[19] = mask;
	
	Append_CRC16_Check_Sum(judge_send, 22);
	DMA_Cmd(DMA1_Stream3,ENABLE);
}

/**
  * @brief  裁判系统数据发送，内容为机器人当前状态
  * @param  None
  * @retval None
  */
//int legState, magState, handState;
//char solenoidState;
void return_state(void)
{
//	int i = 0;
//	legState = 0;
//	for(i = 0; i < 4; i++)
//	{
//		if(Flag.ExpandFlagDown[i] == 1)
//			legState += pow(10, i);
//		else if(Flag.ExpandFlagUp[i] == 1)
//			legState += 3 * pow(10, i);
//		else 
//			legState += 2 * pow(10, i);
//	}
//	if(SWITCH_UP == 0)
//		magState = 3;
//	else if(SWITCH_DOWN == 0)
//		magState = 1;
//	else 
//		magState = 2;
//		
//	
//	if(SWITCH_HAND_OUT == 0)
//		handState = 0x03;
//	else if(SWITCH_HAND_IN == 0)
//		handState = 0x01;
//	else 
//		handState = 0x02;
//	
//	judge_send_fill(legState, magState, handState, solenoidState);
}
