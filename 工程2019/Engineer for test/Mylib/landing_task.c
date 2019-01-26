/************************************************************************************
自动登岛模式
***************************************************************************************/

#include "landing_task.h"
#include "main.h"
#include "solenoid_task.h"
#include "chassis_task.h"

void landing_action(void)
{
	switch (g_flag.landing_state) 
	{
		case STATE0:
		{
			static int state0_delay_cnt;
			ASSIST_LEG_ON;                //打开登岛辅助腿
			
			state0_delay_cnt ++;
			if (state0_delay_cnt >= 250)         //等待整个辅助腿完全升起
			{
				state0_delay_cnt = 0;
				g_flag.landing_state = STATE1;
			}
		}break;
		
		case STATE1:
		{
			all_move(0, 1000, 0);         //前进
			
		}break;
		
		case STATE2:
		{
		}break;
		
		case STATE3:
		{
		};
		
		case STATE4:
		{
		}break;
		
		case STATE5:
		{
		}break;
		
		case STATE6:
		{
		};
		
		case STATE7:
		{
		}break;
	}
}
