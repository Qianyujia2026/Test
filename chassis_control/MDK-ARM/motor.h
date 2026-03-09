#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "main.h"

/**
 * @brief 电机基本数据结构体
 *
 */
struct Motor
{
  int16_t rawEncode;     // 编码器返回的原始值
  int16_t lastRawEncode; // 上一次的编码器原始值
  int32_t round;         // 圈数
  int32_t conEncode;     // 处理后的连续的编码器值
  int32_t lastConEncode; // 上一次的处理后的连续编码器值

  int16_t rawSpeed;     // 返回的转速
  int16_t lastRawSpeed; // 上一次返回的转速

  int16_t Current; // 转矩电流

  int8_t temp; // 温度
};

struct PID
{
  float Kp;
  float Ki;
  float Kd;
  float sum_I;
  float target;
	float target_angle;
  float err;
  float lastErr;
  float P_out;
  float I_out;
  float D_out;
  float I_lit;
  float allIit;
  float allOut;
};

typedef struct
{
  // uint8_t gyroStatus[2];

  struct Motor data;
  struct PID PID_S;
  struct PID PID_A;   
   float  can_id;
}Motor_typedef;
//struct M 
//{
//	struct Motor data;
//	struct PID PID_S;
//	struct PID PID_A;
//};

typedef struct 
{
	float vx;//x轴线速度(前后，前为正)
	float vy;//y轴线速度(左右，左为正)
	float w;//偏航角速度(旋转，逆时针为正)
	
}Chassis_cmd;


void MotorResolve(Motor_typedef *motor, uint8_t *RxMessage);
void MotorRoundResolve(Motor_typedef *motor);
void PID_INITSet(Motor_typedef *motor);
void PID_control(Motor_typedef *motor );
void Target_set(Motor_typedef *motor);
void PID_double_control(Motor_typedef *motor);
void PID_Init(Motor_typedef *motor);
extern Motor_typedef chassis_motor[4];//(前左，前后，后左，后右)
extern Chassis_cmd chassis_cmd;


#endif
