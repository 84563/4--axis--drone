#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__



#include "adc.h"

typedef struct 
{
    uint16_t thr;
    uint16_t yaw;
    uint16_t pit;
    uint16_t rol;
}joystick_struct;


/**
 * @brief 初始化ADC 打开ADC
 */
void joystick_init(void);
/**
 * @brief 读取ADC数据 保存到结构体中
 * @param joystick 摇杆数据结构体指针
 */
void joystick_get(joystick_struct *joystick);




#endif
