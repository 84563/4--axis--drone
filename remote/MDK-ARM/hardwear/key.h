#ifndef __KEY_H__
#define __KEY_H__


#include "main.h"
#include "FreeRTOS.h"
#include "task.h"


typedef enum
{
    KEY_NONE = 0,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_LEFT_X,
    KEY_RIGHT_X,
    KEY_RIGHT_X_LONG,
} key_type;


/**
 * @brief 获取按键是否按下 0按下 1抬起
 * @return key_type KEY_NONE:没有按键按下
 */
key_type key_get(void);


#endif
