#ifndef __TP4336_H__
#define __TP4336_H__


#include "main.h"
#include "FreeRTOS.h"
#include "task.h"


//启动TP4336电源 避免自动关闭
void TP4336_start(void);


#endif
