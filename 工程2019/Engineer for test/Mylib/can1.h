#ifndef __CAN1_H__
#define __CAN1_H__

#define RM820R_0x204_1 0x200
#define RM820R_0x207_5 0x1FF

void CAN1_Configration(void);
void chassis_current_send(int a, int b, int c, int d);
void track_current_send(int left, int right);

#endif 
