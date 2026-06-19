#include "motor.h"


/**
 * 传入比较值 初始200 最大1000
 * speed
 */
void motor_set_speed(motor_struct *motor)
{
    if(motor->speed > 1000)
    {
        debug_printf("motor speed is too big!!!");
        return;
    }
    __HAL_TIM_SET_COMPARE(motor->tim, motor->channel, motor->speed);
}

/**
 * 启动电机 传入具体电机的结构体
 * motor
 */
void motor_start(motor_struct *motor)
{

    HAL_TIM_PWM_Start(motor->tim, motor->channel);

}
