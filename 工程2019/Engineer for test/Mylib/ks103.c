/************************************************************************************
KS103��ȡ����
��λΪ���ͽ��ռ�ķ�Χ
��Ҫ�����޸�ID�ŷ������ڳ����е���һ��Change_ID����������ԭID��Ҫ�޸ĳɵ���ID
�ȴ�������ɺ�ȴ�һ�ᣬ�����ϵ磬�۲��ϵ���LED��˸������������´���1������һ�´���0
���IDû���޸ĳɹ�����������һ��
ע��ÿ���ϵ�ֻ������һ��Change_ID��������Ҫ��while(1)�е��ã����й����в�Ҫ�ϵ�
***************************************************************************************/

#include "stm32f4xx.h"
#include "main.h"
#include "ks103.h" 
#include "delay.h"
#include "iic.h"

/**
  * @brief  KS103��ʼ�������ص�Դ����
  * @param  address��������ַ
  * @retval None
  */
void KS103_INIT(u8 address)
{
    KS103_WriteOneByte(address, 0X02, 0X73); 
}

/**
  * @brief  ��ȡ����
  * @param  address��������ַ
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
  * @brief  �޸�ID
  * @param  address����ǰ������ַ
			new_addr��Ҫ�޸ĳɵĵ�ַ
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
    IIC_Send_Byte(address); //���͵͵�ַ
    IIC_Wait_Ack();
    IIC_Send_Byte(reg); //���͵͵�ַ
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(address + 1); //�������ģʽ 
    IIC_Wait_Ack();
    delay_us(25);  //���Ӵ˴���ͨ�ųɹ�������
    temp=IIC_Read_Byte(0);  //���Ĵ��� 3
    IIC_Stop();//����һ��ֹͣ���� 
    return temp;
}


void KS103_WriteOneByte(u8 address,u8 reg,u8 command)
{ 
    IIC_Start();
    IIC_Send_Byte(address);  //����д����
    IIC_Wait_Ack();
    IIC_Send_Byte(reg);//���͸ߵ�ַ
    IIC_Wait_Ack();
    IIC_Send_Byte(command); //���͵͵�ַ
    IIC_Wait_Ack();
    IIC_Stop();//����һ��ֹͣ����
}
