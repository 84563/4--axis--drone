#ifndef __MOTOR_H__
#define __MOTOR_H__


#include "tim.h"
#include "com_debug.h"


typedef struct
{
    TIM_HandleTypeDef *tim;
    uint16_t channel;
    uint16_t speed;
}motor_struct;


/**
 * 传入比较值 初始200 最大1000
 */
void motor_set_speed(motor_struct *motor);

/**
 * 启动电机 传入具体电机的结构体
 */
void motor_start(motor_struct *motor);



#endif
