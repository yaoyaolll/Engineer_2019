#ifndef __USART5_H__
#define __USART5_H__

typedef struct{
	struct
	{
		unsigned short ch0;
		unsigned short ch1;
		unsigned short ch2;
		unsigned short ch3;
		unsigned char s1;
		unsigned char s2;
	}rc;
	struct
	{
		short x;
		short y;
		short z;
		unsigned char press_l;
		unsigned char press_r;
	}mouse;
	struct
	{
		unsigned short w,s,a,d,q,e,r,f,g,z,x,c,v,b,shift,ctrl;
	}key;
    
}rc_ctrl_t;

void UART5_Configuration(void);
void rc_reset(void);
void remote_receive(volatile unsigned char rx_buffer[]);
void key_refresh(void);

#define RX_UART5_BUFFER 30

extern rc_ctrl_t rc_ctrl, rc_ctrl_last;

#endif

