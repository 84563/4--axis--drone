#include "freeRTOS_Task.h"

//stm32f103c8t6 20k --> 分配12k给操作系统


void Task1(void *pvParameters);
//最小填128 -> 128*4 = 512字节
#define Task1_STACK_SIZE 128
//优先级0~4 数值越小优先级越小
#define Task1_PRIORITY  1
TaskHandle_t Task1_handle;



void freeRTOS_start(void)
{
    //1.创建任务
    xTaskCreate( Task1, "Task1", 128, NULL, Task1_PRIORITY, &Task1_handle);

    //2.启动调度器
    vTaskStartScheduler();
}

void Task1(void *pvParameters)
{
    while(1)
    {
        debug_printf("Task1\r\n");
        vTaskDelay(1000); //延时1秒
    }
}
