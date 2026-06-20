#ifndef __FREERTOS_TASK_H__
#define __FREERTOS_TASK_H__

#include "FreeRTOS.h"
#include "task.h"
#include "com_debug.h"
#include "Com_config.h"
#include "TP4336.h"
#include "motor.h"
#include "led.h"
#include "SI24R1.h"
#include "receive_data.h"

void freeRTOS_start(void);


#endif /* __FREERTOS_TASK_H__ */
