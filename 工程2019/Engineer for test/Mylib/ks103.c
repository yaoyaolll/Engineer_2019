/************************************************************************************
KS103读取距离
单位为发送接收间的范围
主要介绍修改ID号方法，在程序中调用一次Change_ID函数，输入原ID和要修改成的新ID
等待运行完成后等待一会，重新上电，观察上电后的LED闪烁情况，快闪两下代表1，慢闪一下代表0
如果ID没有修改成功则重新运行一次
注意每次上电只能运行一次Change_ID函数，不要在while(1)中调用，运行过程中不要断电
***************************************************************************************/

#include "stm32f4xx.h"
#include "main.h"
#include "ks103.h" 
#include "delay.h"
#include "iic.h"

/**
  * @brief  KS103初始化，开关电源降噪
  * @param  address：器件地址
  * @retval None
  */
void KS103_INIT(u8 address)
{
    KS103_WriteOneByte(address, 0X02, 0X73); 
}

/**
  * @brief  读取距离
  * @param  address：器件地址
  * @retval None
  */
u16 KS103_READ(u8 address)
{
    u16 range;	
    
    KS103_WriteOneByte(address, 0X02, 0X0a); 
    delay_ms(5);
    range = KS103_ReadOneByte(address, 0x02);
    range <<= 8;
    range += KS103_ReadOneByte(address, 0x03);
    
    return range;
}

/**
  * @brief  修改ID
  * @param  address：当前器件地址
			new_addr：要修改成的地址
  * @retval None
  */
void Change_ID(u8 address, u8 new_addr)
{
    KS103_WriteOneByte(address, 0X02, 0x9a); 
    delay_ms(5);
    KS103_WriteOneByte(address, 0X02, 0x92); 
    delay_ms(5);
    KS103_WriteOneByte(address, 0X02, 0x9e); 
    delay_ms(5);
    KS103_WriteOneByte(address, 0X02, new_addr); 
    delay_ms(200);
}

u8 KS103_ReadOneByte(u8 address, u8 reg)
{ 
    u8 temp=0;
    IIC_Start();
    IIC_Send_Byte(address); //发送低地址
    IIC_Wait_Ack();
    IIC_Send_Byte(reg); //发送低地址
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(address + 1); //进入接收模式 
    IIC_Wait_Ack();
    delay_us(25);  //增加此代码通信成功！！！
    temp=IIC_Read_Byte(0);  //读寄存器 3
    IIC_Stop();//产生一个停止条件 
    return temp;
}


void KS103_WriteOneByte(u8 address,u8 reg,u8 command)
{ 
    IIC_Start();
    IIC_Send_Byte(address);  //发送写命令
    IIC_Wait_Ack();
    IIC_Send_Byte(reg);//发送高地址
    IIC_Wait_Ack();
    IIC_Send_Byte(command); //发送低地址
    IIC_Wait_Ack();
    IIC_Stop();//产生一个停止条件
}
