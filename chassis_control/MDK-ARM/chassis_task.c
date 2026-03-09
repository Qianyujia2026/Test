#include "chassis_task.h"
#include "Motor.h"
#include "DBUS.h"
#include "CAN_task.h" 

Motor_typedef chassis_motors[4];
  Chassis_cmd chassis_cmd = {0};

// 遥控缩放系数
#define RC_SCALE 1
// 电机转速限幅
#define MOTOR_SPEED_MAX 500
#define rotate_radius 0.25//旋转半径
#define WHEEL_RADIUS  0.075

// 底盘电机初始化
void Chassis_Simple_Init(void)
{
    for(int i=0; i<4; i++){
        PID_Init(&chassis_motors[i]);
		
        // 配置电机CAN ID
        chassis_motors[0].can_id = 0x201; // 前左
        chassis_motors[1].can_id = 0x202; // 前右
        chassis_motors[2].can_id = 0x203; // 后左
        chassis_motors[3].can_id = 0x204; // 后右
        // 速度环PID参数
        chassis_motors[i].PID_S.Kp = 10;
        chassis_motors[i].PID_S.Ki = 0.1;
        chassis_motors[i].PID_S.Kd = 0;

    }
}

//  遥控通道映射为运动指令
void Chassis_Simple_RC_Map(void)
{
	float vx_max=0.5;
	float vy_max=0.5;
	float w_max=3;
    // 直接取遥控通道值，乘缩放系数
    chassis_cmd.vx = remote_t.control.ch1_int16*(vx_max/660);
    chassis_cmd.vy = remote_t.control.ch0_int16*(vy_max/660) ;
    chassis_cmd.w = remote_t.control.ch3_int16 *(w_max/660);
	

    // 限幅
//	if(chassis_cmd.vx > MOTOR_SPEED_MAX)
//	{
//		 chassis_cmd.vx=MOTOR_SPEED_MAX ;
//	}
////	if(chassis_cmd.vx < -MOTOR_SPEED_MAX)
////	{
////		chassis_cmd.vx=-MOTOR_SPEED_MAX ;
////	}
//	if(chassis_cmd.vy > MOTOR_SPEED_MAX)
//	{
//		 chassis_cmd.vy=MOTOR_SPEED_MAX ;
//	}
////	if(chassis_cmd.vy < -MOTOR_SPEED_MAX)
////	{
////		chassis_cmd.vy=-MOTOR_SPEED_MAX ;
////	}
//    if(chassis_cmd.w > MOTOR_SPEED_MAX)
//	{
//		 chassis_cmd.w=MOTOR_SPEED_MAX ;
//	}
////	if(chassis_cmd.w < -MOTOR_SPEED_MAX)
////	{
//		chassis_cmd.w=-MOTOR_SPEED_MAX ;
//	}
}

//  转速分配
void Chassis_Speed(void)
{
    int16_t motor_speed[4];
	//转化成rpm(每分钟的转的圈数=(轮子的目标角速度[每秒转过的度数]/2Π)*60
    motor_speed[0] = (chassis_cmd.vx + chassis_cmd.vy + chassis_cmd.w*0.25)*(30/3.14)/0.075 *19;// 前左
    motor_speed[1] = (-chassis_cmd.vx + chassis_cmd.vy + chassis_cmd.w*0.25)*(30/3.14)/0.075*19; // 前右
    motor_speed[2] = (-chassis_cmd.vx - chassis_cmd.vy + chassis_cmd.w*0.25)*(30/3.14)/0.075*19; // 后左
    motor_speed[3] = (chassis_cmd.vx - chassis_cmd.vy + chassis_cmd.w*0.25)*(30/3.14)/0.075*19; // 后右

//    // 限幅，赋值给电机速度环目标
   for(int i=0; i<4; i++){
//      
//			if(motor_speed[i] > MOTOR_SPEED_MAX)
//	   {
//		   motor_speed[i] =MOTOR_SPEED_MAX ;
//	   }
//	   if(motor_speed[i]  < -MOTOR_SPEED_MAX)
//	   {
//	  	motor_speed[i] =-MOTOR_SPEED_MAX ;
//	   }
        chassis_motors[i].PID_S.target = motor_speed[i];
   }
}

// 底盘
/*void Chassis_Simple_Control(void)
{
    // 遥控离线保护
    if(remote_t.control.s1_u8 == 3 && remote_t.control.s2_u8 == 3){
        chassis_cmd.vx = 0;
        chassis_cmd.vy = 0;
        chassis_cmd.w= 0;
    }
    else{
        Chassis_Simple_RC_Map();       // 遥控映射
        Chassis_Speed();   // 转速分配
    }
	}*/

   /*
    for(int i=0; i<4; i++){
			 // PID_Init(&chassis_motors[i]);
        PID_control(&chassis_motors[i]);
    }

    // CAN发送电机指令
    for(int i=0; i<4; i++){
        CAN_send(&hcan,chassis_motors[i].can_id, (int16_t)chassis_motors[i].PID_S.allOut,0,0,0);
    }
}*/