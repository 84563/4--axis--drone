#ifndef __PROCESS_DATA_H__
#define __PROCESS_DATA_H__

#include "joystick.h"
#include "key.h"
#include "com_debug.h"
#include "com_tool.h"

typedef struct
{
    int16_t thr;
    int16_t yaw;
    int16_t pit;
    int16_t rol;

    int8_t shutdown;    //1:关机 0:开机
    int8_t fix_height;  //1:固定高度 0:不固定高度
}Remote_Data;

/**
 * @brief 处理按键数据
 */
void process_key_data(void);
/**
 * @brief 处理摇杆数据
 */
void process_joystick_data(void);

#endif
