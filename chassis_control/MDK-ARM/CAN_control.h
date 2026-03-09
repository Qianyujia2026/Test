#ifndef _CAN_TASK_H_
#define _CAN_TASK_H_

#include "can.h"
#include "Motor.h"

#define GM6020_D 0x205//反馈报文标识符--在电机协议中易查

extern Motor_typedef GM6020_demo; 

void CAN_send(CAN_HandleTypeDef *_hcan, int16_t stdid,int16_t num1,int16_t num2,int16_t num3,int16_t num4);
void CAN_Filter_Init(CAN_HandleTypeDef *hcan);
#endif
