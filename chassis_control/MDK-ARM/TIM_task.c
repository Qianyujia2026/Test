#include "tim.h"
#include "TIM_task.h"
#include "CAN_task.h"
#include "DBUS.h"
#include "VOFA.h"
#include "Motor.h"
#include "chassis_task.h" 

int16_t AA          = 0;
uint8_t DBUS_status = 0;
uint8_t DBUS_Time   = 100;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) { 
			// 云台任务
			//Chassis_Simple_Control();
			 Chassis_Simple_RC_Map();
       Chassis_Speed();
//			 PID_control(&chassis_motors[0]); 
//	 PID_control(&chassis_motors[1]);
//   PID_control(&chassis_motors[2]); 	
//	 PID_control(&chassis_motors[3]); 
			/* PID_control(&chassis_motors[0]); 
			 PID_control(&chassis_motors[1]); 
    for(int i=0; i<4; i++){
			 // PID_Init(&chassis_motors[i]);
        PID_control(&chassis_motors[i]); 
    } */
     /* for(int i=0; i<4; i++)
			{
        CAN_send(&hcan,chassis_motors[i].can_id, (int16_t)chassis_motors[i].PID_S.allOut,0,0,0);
      }*/
	CAN_send(&hcan, 0x200,chassis_motors[0].PID_S.allOut, chassis_motors[1].PID_S.allOut,chassis_motors[2].PID_S.allOut, chassis_motors[3].PID_S.allOut);
       
       /* VOFA_justfloat(remote_t.control.ch0_int16,
                       remote_t.control.ch1_int16,
                       remote_t.control.ch2_int16,
                       remote_t.control.ch3_int16,
                       GM6020_demo.data.rawEncode,
                       GM6020_demo.data.rawSpeed,
                       0,
                       0,
                       0,
                       0);*/
		/*	VOFA_justfloat(GM6020_demo .PID_S .I_out ,
                       GM6020_demo .PID_S .err ,
                       GM6020_demo .PID_S .target ,
                       GM6020_demo .PID_S .P_out  ,
                       GM6020_demo.PID_S .allOut ,
                       GM6020_demo.data.rawEncode ,
                       GM6020_demo.PID_S.sum_I,
                       0,
                       0,
                       0);*/
			
    }
//    if (htim->Instance == TIM4) { // 离线监测任务
//        DBUS_Time++;
//        if (DBUS_Time >= 50)
//            DBUS_status = off_line;
//        else
//            DBUS_status = on_Line;
//        if (DBUS_status == off_line) {
//            DBUS_offLine();
//        }
//    }
}
