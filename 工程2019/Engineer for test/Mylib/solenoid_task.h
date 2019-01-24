#ifndef _SOLENOID_TASK_H_
#define _SOLENOID_TASK_H_

#define ASSIST_LEG_ON			PCout(13) = 1;PCout(0) = 1		//辅助腿打开
#define ASSIST_LEG_OFF			PCout(13) = 0;PCout(0) = 0		//辅助腿关闭

#define FILL_BULLET_OPEN		PCout(2) = 1		//补弹气缸张开
#define FILL_BULLET_CLOSE		PCout(2) = 0		//补弹气缸关闭

#define RIGHT_PAW_OPEN		PCout(14) = 1		//右手张开
#define RIGHT_PAW_CLOSE		PCout(14) = 0		//右手关闭
#define UP_PAW_OPEN         PAout(2) = 1		//取弹手张开
#define UP_PAW_CLOSE        PAout(2) = 0		//取弹手关闭
#define MAGAZINE_OPEN		PCout(15) = 1
#define MAGAZINE_CLOSE		PCout(15) = 0
#define MAGAZINE_PROTECT_ON		PCout(1) = 0
#define MAGAZINE_PROTECT_OFF	PCout(1) = 1
#define LIGHT_SWITCH_ON		PAout(5) = 1
#define LIGHT_SWITCH_OFF	PAout(5) = 0

void solenoid_config(void);
void solenoid_action(void);
#endif
