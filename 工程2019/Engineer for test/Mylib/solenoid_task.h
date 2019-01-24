#ifndef _SOLENOID_TASK_H_
#define _SOLENOID_TASK_H_

#define ASSIST_LEG_ON			PCout(13) = 1;PCout(0) = 1		//�����ȴ�
#define ASSIST_LEG_OFF			PCout(13) = 0;PCout(0) = 0		//�����ȹر�

#define FILL_BULLET_OPEN		PCout(2) = 1		//���������ſ�
#define FILL_BULLET_CLOSE		PCout(2) = 0		//�������׹ر�

#define RIGHT_PAW_OPEN		PCout(14) = 1		//�����ſ�
#define RIGHT_PAW_CLOSE		PCout(14) = 0		//���ֹر�
#define UP_PAW_OPEN         PAout(2) = 1		//ȡ�����ſ�
#define UP_PAW_CLOSE        PAout(2) = 0		//ȡ���ֹر�
#define MAGAZINE_OPEN		PCout(15) = 1
#define MAGAZINE_CLOSE		PCout(15) = 0
#define MAGAZINE_PROTECT_ON		PCout(1) = 0
#define MAGAZINE_PROTECT_OFF	PCout(1) = 1
#define LIGHT_SWITCH_ON		PAout(5) = 1
#define LIGHT_SWITCH_OFF	PAout(5) = 0

void solenoid_config(void);
void solenoid_action(void);
#endif
