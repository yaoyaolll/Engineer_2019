#ifndef __USART3_H__
#define __USART3_H__
#include "main.h"
#define JudgeBufBiggestSize 34

void USART3_Configuration(void);
void judge_buf_receive(unsigned char receive_buf[], uint16_t data_len);
void return_state(void);
void judge_send_fill(float data1,float data2,float data3 ,unsigned char mask);
#endif
