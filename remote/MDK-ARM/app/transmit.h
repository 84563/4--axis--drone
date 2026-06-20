#ifndef __TRANSMIT_H__
#define __TRANSMIT_H__


#include "SI24R1.h"
#include "process_data.h"
#include "com_debug.h"
#include "FreeRTOS.h"
#include "task.h"

//定义帧头校验的值
#define FRAME_HEAD_CHECK1 's'
#define FRAME_HEAD_CHECK2 'g'
#define FRAME_HEAD_CHECK3 'g'

/**
 * @brief 自动切换SI24R1的模式 => 将采集的ADC数据发送给飞控
 */
void transmit_data(void);

#endif
