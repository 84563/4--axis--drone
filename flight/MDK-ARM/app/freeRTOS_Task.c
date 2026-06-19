#include "freeRTOS_Task.h"

//stm32f103c8t6 20k --> 分配12k给操作系统

//电机结构体
motor_struct left_top_motor = {.tim = &htim3, .channel = TIM_CHANNEL_1, .speed = 200};
motor_struct left_bottom_motor = {.tim = &htim4, .channel = TIM_CHANNEL_4, .speed = 200};
motor_struct right_top_motor = {.tim = &htim2, .channel = TIM_CHANNEL_2, .speed = 200};
motor_struct right_bottom_motor = {.tim = &htim1, .channel = TIM_CHANNEL_3, .speed = 200};



//LED结构体
LED_Struct left_top_led = {.port = LED1_GPIO_Port, .pin = LED1_Pin};
LED_Struct right_top_led = {.port = LED2_GPIO_Port, .pin = LED2_Pin};
LED_Struct left_bottom_led = {.port = LED3_GPIO_Port, .pin = LED3_Pin};
LED_Struct right_bottom_led = {.port = LED4_GPIO_Port, .pin = LED4_Pin};

//当前的连接状态
Remote_State remote_state = REMOTE_DISCONNECTED;


//当前的飞行状态
Flight_State flight_state = NORMAL;



//电源管理任务
void power_task(void *pvParameters);
//最小填128 -> 128*4 = 512字节
#define POWER_TASK_STACK_SIZE 128
//优先级0~4 数值越小优先级越小
#define POWER_TASK_PRIORITY  4
TaskHandle_t power_task_handle;
#define POWER_TASK_PERIOD 10000

//飞行任务
void flight_task(void *pvParameters);
#define FLIGHT_TASK_STACK_SIZE 128
#define FLIGHT_TASK_PRIORITY 4
TaskHandle_t flight_control_task_handle;
#define FLIGHT_TASK_PERIOD 6

//LED任务
void led_task(void *pvParameters);
#define LED_TASK_STACK_SIZE 128
#define LED_TASK_PRIORITY 4
TaskHandle_t led_task_handle;
#define LED_TASK_PERIOD 100

//通信任务
void com_task(void *pvParameters);
#define COM_TASK_STACK_SIZE 128
#define COM_TASK_PRIORITY 4
TaskHandle_t com_task_handle;
#define COM_TASK_PERIOD 6


void freeRTOS_start(void)
{
    //1.创建电源管理任务
    xTaskCreate( power_task, "power_task", POWER_TASK_STACK_SIZE, NULL, POWER_TASK_PRIORITY, &power_task_handle);

    //创建飞行控制任务
    xTaskCreate( flight_task, "flight_task", FLIGHT_TASK_STACK_SIZE, NULL, FLIGHT_TASK_PRIORITY, &flight_control_task_handle);

    //创建LED任务
    xTaskCreate( led_task, "led_task", LED_TASK_STACK_SIZE, NULL, LED_TASK_PRIORITY, &led_task_handle);

    //创建通信任务
    xTaskCreate( com_task, "com_task", COM_TASK_STACK_SIZE, NULL, COM_TASK_PRIORITY, &com_task_handle);

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

void flight_task(void *pvParameters)
{
    //获取当前的基准时间
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

     while(1)
    {
        //设置转速

        //启动电机
        //motor_start();

        vTaskDelayUntil(&xLastWakeTime, FLIGHT_TASK_PERIOD);
    }
}

void led_task(void *pvParameters)
{
    //获取当前的基准时间
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    uint8_t count = 0;

    while(1)
    {   
        count++;
        //前2个灯表示连接状态
        //判断是否连接成功
        if(remote_state == REMOTE_CONNECTED)
        {
            //点亮前2个灯
            led_on(&left_top_led);
            led_on(&right_top_led);
        }
        else if(remote_state == REMOTE_DISCONNECTED)
        {
            //关掉前2个灯
            led_off(&left_top_led);
            led_off(&right_top_led);
        }

        //后2个灯表示飞行状态
        //判断是否正在飞行
        if(flight_state == IDLE)
        {
            if (count % 5 == 0)
            {
                //灯慢闪 500ms亮 500ms灭 循环5次 一次100ms 
                led_toggle(&left_bottom_led);
                led_toggle(&right_bottom_led);
            }
        }
        else if(flight_state == NORMAL)
        {
            if (count % 2 == 0)
            {
                //灯快闪 200ms亮 200ms灭 循环2次 一次100ms 
                led_toggle(&left_bottom_led);
                led_toggle(&right_bottom_led);
            }
        }
        else if(flight_state == FIX_HEIGHT)
        {
            //点亮后2个灯
            led_on(&left_bottom_led);
            led_on(&right_bottom_led);
        }
        else if(flight_state == FAIL)
        {
            //关掉后2个灯
            led_off(&left_bottom_led);
            led_off(&right_bottom_led);
        }

        vTaskDelayUntil(&xLastWakeTime, LED_TASK_PERIOD);
    }
}

uint8_t com_data[TX_PLOAD_WIDTH] = {0};
void com_task(void *pvParameters)
{
    //获取当前的基准时间
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    
    while (1)
    {
        //接收数据到缓存区
        uint8_t res = SI24R1_RxPacket(com_data);
        if(res == 0)
        {
            printf("%s\n",com_data);
        }
        vTaskDelayUntil(&xLastWakeTime, COM_TASK_PERIOD);
    }
}
