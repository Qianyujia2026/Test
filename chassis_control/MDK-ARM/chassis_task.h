#ifndef _CHASSIS_TASK_H_
#define _CHASSIS_TASK_H_
#include "motor.h" 
void Chassis_Simple_Init(void);
void Chassis_Simple_RC_Map(void);
void Chassis_Speed(void);
	void Chassis_Simple_Control(void);
extern 	Motor_typedef chassis_motors[4];
 
	#endif