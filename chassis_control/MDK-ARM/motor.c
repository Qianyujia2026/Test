#include "Motor.h"

/**
 * @brief 解算电机反馈来的数据
 *
 * @param motor 电机结构体指针，
 * @param RxMessage CAN报文
 */
void MotorResolve(Motor_typedef *motor, uint8_t *RxMessage)
{
    /*编码器值解算*/
    motor->data.lastRawEncode = motor->data.rawEncode;
    motor->data.rawEncode     = ((uint16_t)RxMessage[0] << 8 | (uint16_t)RxMessage[1]);
    /*速度解算*/
    motor->data.lastRawSpeed = motor->data.rawSpeed;
    motor->data.rawSpeed     = ((uint16_t)RxMessage[2] << 8 | (uint16_t)RxMessage[3]);
    /*转矩电流*/
    motor->data.Current = ((uint16_t)RxMessage[4] << 8 | (uint16_t)RxMessage[5]);
    /*电机温度*/
    motor->data.temp = RxMessage[6];
	
}
//void Motor_Resolve(struct M *motor)
//{
//	motor ->data 
//}

/**
 * @brief 根据电机编码器值计算运转圈数
 *
 * @param motor 电机结构体指针
 */
void MotorRoundResolve(Motor_typedef *motor)//quanshu
{
	//这里不要初始化总编码值，定时器有一定同步性
	
    if (motor->data.rawEncode - motor->data.lastRawEncode < -4096) {
        motor->data.round--;
    }
    if (motor->data.rawEncode - motor->data.lastRawEncode > 4096) {
        motor->data.round++;
    }
		
    motor->data.conEncode=motor->data.round*8192+motor->data.rawEncode;
		motor->data .lastRawEncode =motor->data.rawEncode ;//必要加
}


void PID_Init(Motor_typedef *motor)
{
  motor->PID_S .Kp=0;
	motor ->PID_S .Ki =0;
	motor ->PID_S .Kd=0 ;
	motor->PID_S .err =0;
	motor->PID_S .lastErr =0;
	motor->PID_S .target =0;
	motor->PID_S .sum_I=0; 
	motor->PID_S .I_out =0;
	motor->PID_S .allOut =0;
	motor->PID_S .D_out =0;
	motor ->PID_S .P_out =0;
	motor->data .rawSpeed =0;
	motor->data .lastRawSpeed =0;
	
}

void PID_control(Motor_typedef *motor )
{
	motor->PID_S .err =motor->PID_S .target -motor ->data .rawSpeed ;
	if((motor->PID_S .target ==0)&&((motor->data .rawSpeed  <5)||(motor->data .rawSpeed  >-5)))
	{
    motor ->PID_S .sum_I =0;//很重要啊
		motor->PID_S.I_out =0;
	}
	
	/*if((motor->PID_S .err >=0&&motor->PID_S .err <=10)||(motor->PID_S .err <=0&&motor->PID_S .err >=-10))
	{
		motor->PID_S.P_out =0;
		//motor->PID_S .allOut =0;
		//motor->PID_S .I_out =0;
	}
	else 
	{*/
		motor->PID_S .P_out =motor->PID_S .err * (motor->PID_S .Kp );
		motor ->PID_S .sum_I +=motor ->PID_S .err ;
		motor->PID_S .I_out =motor->PID_S .sum_I * (motor->PID_S .Ki) ;
		motor ->PID_S .D_out =((motor ->PID_S.err - motor->PID_S .lastErr )/0.001)*motor->PID_S .Kd ;
	
	
	motor->PID_S .lastErr =motor ->PID_S .err ;
	//限制
	/*if (motor->PID_S .P_out <-5000)
	{
		motor->PID_S .P_out =-5000;
	}
	if(motor ->PID_S .P_out >5000)
	{
		motor->PID_S .P_out =5000;
	}*/
	if(motor->PID_S .I_out >5000)
	{
		motor->PID_S.I_out =5000;
	}
	if(motor ->PID_S .I_out <-5000)
	{
		motor->PID_S .I_out =-5000;
	}
	if(motor->PID_S .sum_I >5000)
	{
		motor->PID_S .sum_I =5000;
	}
	if(motor->PID_S .sum_I <-5000)
	{
		motor->PID_S .sum_I =-5000;
	}
	
	motor ->PID_S .allOut =motor->PID_S .P_out +motor->PID_S .I_out +motor->PID_S .D_out ;
	if(motor->PID_S .allOut >8000)
	{
		motor ->PID_S .allOut =8000;
	}
	if(motor->PID_S.allOut <-8000)
	{
		motor->PID_S .allOut =-8000;
	}
	
}