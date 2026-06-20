#ifndef __RECEIVE_DATA_H__
#define __RECEIVE_DATA_H__


#include "SI24R1.h"
#include "Com_config.h"
#include "com_debug.h"
#include "FreeRTOS.h"
#include "task.h"

//帧头校验
#define FRAME_HEAD_CHECK_1 's'
#define FRAME_HEAD_CHECK_2 'g'
#define FRAME_HEAD_CHECK_3 'g'

//最大重装次数
#define MAX_RETRY_TIMES 10


/**
 * @brief 接收数据
 * 
 * @return 0: 接收成功 1: 接收失败
 */
uint8_t receive_data(void);

/**
 * @brief 处理连接状态
 * 
 * @param res 接收到的数据
 */
void process_connect_state(uint8_t res);

/**
 * @brief 处理飞行状态
 * 
 */
void process_flight_state(void);

/**
 * @brief 处理解锁逻辑
 * 
 */
static uint8_t process_unlock(void);

#endif
