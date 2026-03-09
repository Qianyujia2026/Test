
#include "CAN_task.h"
#include "can.h"
#include "Motor.h"
#include "VOFA.h"
#include "chassis_task.h" 

uint8_t CAN1RXmessage[8];
CAN_RxHeaderTypeDef rx1;
void CAN_Calc(CAN_HandleTypeDef *hcan, uint32_t ID);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)//CAN的接收回调函数
{
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx1, CAN1RXmessage);

   CAN_Calc(hcan, rx1.StdId);
		/*for(int i=0; i<4; i++){
			 // PID_Init(&chassis_motors[i]);
        PID_control(&chassis_motors[i]); 
    } */
	 PID_control(&chassis_motors[0]); 
	 PID_control(&chassis_motors[1]);
   PID_control(&chassis_motors[2]); 	
	 PID_control(&chassis_motors[3]); 
	
}

void CAN_Calc(CAN_HandleTypeDef *hcan, uint32_t ID)
{

    if (hcan->Instance == CAN1) {
        switch (ID) {
       
					case 0x201:
						   MotorResolve(&chassis_motors[0], CAN1RXmessage);
					break ;
						
					case M3508_2:
						   MotorResolve(&chassis_motors[1], CAN1RXmessage);
					break ;
					
					case M3508_3:
						   MotorResolve(&chassis_motors[2], CAN1RXmessage);
				
					break ;
					
					case M3508_4:
					
						   MotorResolve(&chassis_motors[3], CAN1RXmessage);
							
					break ;
					
        }
    }
}

/**
 * @brief CAN发送函数
 *
 * @param _hcan CANx
 * @param stdid 标识符(ID)
 * @param num1 
 * @param num2
 * @param num3
 * @param num4
 * @return void
 */
void CAN_send(CAN_HandleTypeDef *_hcan, int16_t stdid, int16_t num1, int16_t num2, int16_t num3, int16_t num4)
{
    CAN_TxHeaderTypeDef tx;
    uint8_t Data[8];
    uint32_t mailbox = 0;
    tx.DLC           = 0x08;//数据长度
	  tx.IDE           = CAN_ID_STD;//帧类型标识：分为标准帧和扩展帧
    tx.RTR           = CAN_RTR_DATA;//帧类型：数据帧和远程帧
    tx.StdId         = stdid;
    tx.ExtId         = 0x000;
    Data[0]          = ((num1) >> 8);
    Data[1]          = (num1);
    Data[2]          = ((num2) >> 8);
    Data[3]          = (num2);
    Data[4]          = ((num3) >> 8);
    Data[5]          = (num3);
    Data[6]          = ((num4) >> 8);
    Data[7]          = (num4);
	HAL_CAN_AddTxMessage(&hcan, &tx, Data, &mailbox);//报文发送函数(将配置好的tx和Data写入CAN发送邮箱，触发报文向CAN总线发送，给电机发送控制指令）
}

/**
 * @brief CAN滤波器初始化
 *
 * @param hcan CANx
 */
void CAN_Filter_Init(CAN_HandleTypeDef *hcan)
{
    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterBank           = 0;//滤波器组
    sFilterConfig.FilterMode           = CAN_FILTERMODE_IDMASK;//掩码模式
	sFilterConfig.FilterScale          = CAN_FILTERSCALE_32BIT;//滤波器尺度
    sFilterConfig.FilterIdHigh         = 0x0000;
    sFilterConfig.FilterIdLow          = 0x0000;
    sFilterConfig.FilterMaskIdHigh     = 0x0000;
    sFilterConfig.FilterMaskIdLow      = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation     = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK) {
        /* Filter configuration Error */
        Error_Handler();
    }
    /* Start the CAN peripheral */
   if (HAL_CAN_Start(hcan) != HAL_OK) {
        /* Start Error */
        Error_Handler();
    }
    /* Activate CAN RX notification */
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        /* Notification Error */
        Error_Handler();
    }
    /* Activate CAN TX notification */
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
        /* Notification Error */
        Error_Handler();
    }
}