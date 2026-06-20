#include	"process_data.h"


//摇杆数据结构体
joystick_struct joystick = {0};
Remote_Data remote_data = {0};

//区分按键的微调值和摇杆的控制值
uint16_t key_pit_offset = 0;        //往前飞是正
uint16_t key_roll_offset = 0;       //往右飞是正

//记录摇杆偏移量
uint16_t thr_offset = 0;
uint16_t yaw_offset = 0;
uint16_t pit_offset = 0;
uint16_t rol_offset = 0;


//校准摇杆函数
void calibrate_joystick(void)
{
    //零偏校准的逻辑就是减去偏零的值
    //先清空按键的微调值
    key_pit_offset = 0;
    key_roll_offset = 0;

    //多次读取求平均值
    int16_t thr_sum = 0;
    int16_t yaw_sum = 0;
    int16_t pit_sum = 0;
    int16_t rol_sum = 0;

    for (int i = 0; i < 10; i++)
    {
        process_joystick_data();
        thr_sum += joystick.thr - 0;
        yaw_sum += joystick.yaw - 500;
        pit_sum += joystick.pit - 500;
        rol_sum += joystick.rol - 500;
    }
    //零偏校准的偏移值没有累加的效果 会造成2次校准回退的情况
    thr_offset = thr_sum / 10;
    yaw_offset = yaw_sum / 10;
    pit_offset = pit_sum / 10;
    rol_offset = rol_sum / 10;
}


/**
 * @brief 处理按键数据
 */
void process_key_data(void)
{
    key_type key = key_get();
    switch (key)
    {
    case KEY_UP:
        key_pit_offset += 10;
        break;
    case KEY_DOWN:
        key_pit_offset -= 10;
        break;
    case KEY_LEFT:
        key_roll_offset += 10;
        break;
    case KEY_RIGHT:
        key_roll_offset -= 10;
        break;
    case KEY_LEFT_X:
        remote_data.shutdown = 1;
        break;
    case KEY_RIGHT_X:
        remote_data.fix_height = 1;
        break;
    default:
        break;
    }
    if (key == KEY_NONE)
    {
        calibrate_joystick();
    }
}
/**
 * @brief 处理摇杆数据
 */
void process_joystick_data(void)
{
    //使用临界区解决freertos的任务切换问题
    taskENTER_CRITICAL();
    
    //读取摇杆数据
    joystick_get(&joystick);

    //处理范围和极性 0-1000使用范围   ==>> adc的范围0-4095
    joystick.thr = 1000 - (joystick.thr * 1000 / 4095);
    joystick.yaw = 1000 - (joystick.yaw * 1000 / 4095);
	joystick.pit = 1000 - (joystick.pit * 1000 / 4095);
	joystick.rol = 1000 - (joystick.rol * 1000 / 4095);

    //处理摇杆偏移量
    joystick.thr -= thr_offset;
	joystick.yaw -= yaw_offset;
	joystick.pit -= pit_offset;
	joystick.rol -= rol_offset;
    
    //处理按键微调值
	joystick.pit += key_pit_offset;
	joystick.rol += key_roll_offset;

    //计算偏移可能超出范围 => 限制范围 0-1000
	joystick.thr = com_limit(joystick.thr,0,1000);
	joystick.yaw = com_limit(joystick.yaw,0,1000);
	joystick.pit = com_limit(joystick.pit,0,1000);
	joystick.rol = com_limit(joystick.rol,0,1000);

    //推出临界区
    taskEXIT_CRITICAL();

    //将处理数据赋值给遥控数据
    remote_data.thr = joystick.thr;
	remote_data.yaw = joystick.yaw;
	remote_data.pit = joystick.pit;
	remote_data.rol = joystick.rol;

    //debug_printf(":%d,%d,%d,%d\r\n",joystick.thr,joystick.yaw,joystick.pit,joystick.rol);
}
