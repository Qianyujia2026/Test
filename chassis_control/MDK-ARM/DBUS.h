#ifndef _DBUS_H_
#define _DBUS_H_

#include "main.h"

/**
 * @brief 遥控数据结构体
 *
 */
typedef struct remote {
    struct
    {
        int16_t ch0_int16;//横滚(ROll),需要正负值
        int16_t ch0_int16_last;
        int16_t ch1_int16;//俯仰(pitch)
        int16_t ch1_int16_last;
        int16_t ch2_int16;//油门
        int16_t ch2_int16_last;
        int16_t ch3_int16;//偏航(Yaw)
        int16_t ch3_int16_last;
        int16_t dial_int16;
        uint8_t last_s1;
        uint8_t s1_u8;
        uint8_t last_s2;
        uint8_t s2_u8;
    } control;
} remote_typedef;

extern remote_typedef remote_t;

void DBUS_Resolve(uint8_t *data);
void DBUS_offLine(void );

#endif
