#include "joystick.h"


uint16_t adc_buff[4] = {0};

/**
 * @brief 初始化ADC 打开ADC
 */
void joystick_init(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buff, 4);
}
/**
 * @brief 读取ADC数据 保存到结构体中
 * @param joystick 摇杆数据结构体指针
 */
void joystick_get(joystick_struct *joystick)
{
    joystick->thr = adc_buff[0];
	joystick->yaw = adc_buff[1];    //偏航角
	joystick->pit = adc_buff[2];    //俯仰角
	joystick->rol = adc_buff[3];    //横滚角
}
