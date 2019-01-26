#ifndef __KS103_H__
#define __KS103_H__

#include <main.h>

#define KS103_DEVICE_ADDRESS1 0XE8
#define KS103_DEVICE_ADDRESS2 0XD0
#define KS103_DEVICE_ADDRESS3 0XD2

void Change_ID(u8 address, u8 new_addr);
u16 KS103_READ(u8 address);
void KS103_INIT(u8 address);
u8 KS103_ReadOneByte(u8 address, u8 reg);
void KS103_WriteOneByte(u8 address,u8 reg,u8 command);

#endif
