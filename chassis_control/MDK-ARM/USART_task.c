#include "USART_task.h"
#include "TIM_task.h"
#include "usart.h"
#include "DBUS.h"

uint8_t DBUS[18] = {0};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3) {
        DBUS_Resolve(DBUS);
        HAL_UART_Receive_DMA(huart, DBUS, 18);
        DBUS_Time = 0;
    }
}
