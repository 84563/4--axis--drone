#include "freeRTOS_Task.h"

//stm32f103c8t6 20k --> 分配12k给操作系统


//电源管理任务
void power_task(void *pvParameters);
//最小填128 -> 128*4 = 512字节
#define POWER_TASK_STACK_SIZE 128
//优先级0~4 数值越小优先级越小
#define POWER_TASK_PRIORITY  4
TaskHandle_t power_task_handle;
#define POWER_TASK_PERIOD 10000

//通信任务
void com_task(void *pvParameters);
#define COM_TASK_STACK_SIZE 128
#define COM_TASK_PRIORITY 3
TaskHandle_t com_task_handle;
#define COM_TASK_PERIOD 6

//按键任务
void key_task(void *pvParameters);
#define KEY_TASK_STACK_SIZE 128
#define KEY_TASK_PRIORITY 2
TaskHandle_t key_task_handle;
#define KEY_TASK_PERIOD 20

//摇杆任务
void joystick_task(void *pvParameters);
#define JOYSTICK_TASK_STACK_SIZE 128
#define JOYSTICK_TASK_PRIORITY 2
TaskHandle_t joystick_task_handle;
#define JOYSTICK_TASK_PERIOD 20




void freeRTOS_start(void)
{
     //1.创建电源管理任务
    xTaskCreate( power_task, "power_task", POWER_TASK_STACK_SIZE, NULL, POWER_TASK_PRIORITY, &power_task_handle);

    //创建通信任务
    xTaskCreate( com_task, "com_task", COM_TASK_STACK_SIZE, NULL, COM_TASK_PRIORITY, &com_task_handle);

    //创建按键任务
    xTaskCreate( key_task, "key_task", KEY_TASK_STACK_SIZE, NULL, KEY_TASK_PRIORITY, &key_task_handle);

    //创建摇杆任务
    xTaskCreate( joystick_task, "joystick_task", JOYSTICK_TASK_STACK_SIZE, NULL, JOYSTICK_TASK_PRIORITY, &joystick_task_handle);

    //2.启动调度器
    vTaskStartScheduler();
}

void power_task(void *pvParameters)
{
    //获取当前的基准时间
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        vTaskDelayUntil(&xLastWakeTime, POWER_TASK_PERIOD);
        TP4336_start();
    }
}

void com_task(void *pvParameters)
{
    //获取当前的基准时间
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    
    while (1)
    {
        //打包数据发送给飞控
        transmit_data();
        vTaskDelayUntil(&xLastWakeTime, COM_TASK_PERIOD);
    }
}

void key_task(void *pvParameters)
{
    //获取当前的基准时间
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        process_key_data();
        vTaskDelayUntil(&xLastWakeTime, KEY_TASK_PERIOD);
    }
}

void joystick_task(void *pvParameters)
{
    //获取当前的基准时间
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    joystick_init();
    while (1)
    {
        process_joystick_data();
        vTaskDelayUntil(&xLastWakeTime, JOYSTICK_TASK_PERIOD);
    }
}
