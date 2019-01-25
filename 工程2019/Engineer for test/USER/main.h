#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdint.h>

#include <stm32f4xx.h>	 
#include <stm32f4xx_conf.h>

/*******************�궨��*************************/
#define ABS(x) ((x)>0? (x):(-(x))) 
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))

/*******************λ������*************************/
//IO??????
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO?????
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n) 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n) 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n) 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n) 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n) 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n) 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n) 

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n) 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  

/*******************??????*************************/
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */

/*******************?????*************************/
typedef struct
{
    s16 angle;					//�Ƕȷ���ֵ
	s16 last_angle;				//ǰһ�νǶȷ���ֵ
	s16 cycle;					//Ȧ��
    s16 speed;					//ʵʱ�ٶ�
}rm820r_t;

typedef struct
{
    s16 angle;					//�Ƕȷ���ֵ
	s16 last_angle;				//ǰһ�νǶȷ���ֵ
	s16 cycle;					//Ȧ��
	s32 real_posion;			//ʵ��λ��
    s16 speed;					//ʵʱ�ٶ�
	s16 flow;					//ʵʱ����
	s16 temperature;			//ʵʱ�¶�
}rmc620_t;

typedef struct
{
	uint8_t robot_level;
	uint8_t bullet_freq;
	uint16_t remain_HP;
	uint16_t shooter_heat17;
	uint16_t shooter_heat42;
	float bullet_speed;
	float real_chassis_outV;
	float real_chassis_outA;
	float real_chassis_power;
	float remain_energy;       //ʣ������
}game_data_t;

typedef enum
{
	STATE0 = 0, STATE1, STATE2, STATE3, STATE4, STATE5, STATE6, STATE7
}landing_state_e;

typedef struct
{
	u8 control_mode;           //����ģʽ
	u8 control_target;         //����Ŀ��
	
	u8 can1_connect;
	u32 can1_cnt;
	
	u8 can2_connect;
	u32 can2_cnt;
	
	u8 rc_connect;
	u32 rc_cnt;
	
	u8 gyro_connect;
	u32 gyro_cnt;
	u8 gyro_use_flag;         //�Ƿ�����������
	
	u8 initial_flag;
	
	landing_state_e landing_state;         //�Զ��ǵ�״̬��־
}flag_t;

typedef union
{
	float fdata;
	unsigned long idata;
}
float_long_t;

typedef struct
{
	uint8_t robot_level;
	uint8_t bullet_freq;
	uint16_t remain_HP;
	uint16_t shooter_heat17;
	uint16_t shooter_heat42;
	float bullet_speed;
	float real_chassis_outV;
	float real_chassis_outA;
	float real_chassis_power;
	float remain_energy;       //ʣ������
}game_inf_t;

#define RC_MODE     		0X01		//ң��ģʽ
#define KEY_MODE    		0X02		//����ģʽ
#define GET_BULLET 			0X03		//�Զ�ȡ��ģʽ
#define LANDING_ON 			0X04		//�Զ��ǵ�ģʽ
#define POWER_OFF 			0X05		//����ģʽ

#define MAGAZINE_MODE		0X06		//�����ϲ�
#define CHASSIS_MODE 		0X07		//���Ƶ����ƶ� 
#define EXPAND_MODE 		0x08		//���Ƶ�������
#define RESCUE_MODE 		0X09		//���ƾ�Ԯ��е��
#define MANUAL_LAND_MODE    0X10        //�ֶ����Ƶǵ�
#define INITIALIZE_MODE		0X11        //����װ�û�λ

extern flag_t g_flag;
extern int cur_error_num;				//���ߴ���� 

void System_Configration(void);

#endif

