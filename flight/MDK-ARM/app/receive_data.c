#include "receive_data.h"

extern Remote_Data remote_data;

uint8_t rx_buff[TX_PLOAD_WIDTH] = {0};
//遥控状态
extern Remote_State remote_state;
//飞行状态
extern Flight_State flight_state;

//油门解锁状态值
Thr_state thr_state = FREE;

//MAX状态进入时间
uint32_t max_enter_time = 0;

//MIN状态进入时间
uint32_t min_enter_time = 0;

//重装次数
uint8_t retry_count = 0;

/**
 * @brief 接收数据
 * 
 * @return 0: 接收成功 1: 接收失败
 */
uint8_t receive_data(void)
{
    memset(rx_buff, 0, TX_PLOAD_WIDTH);
    SI24R1_RxPacket(rx_buff);
    if(strlen((char *)rx_buff) == 0)
    {
        return 1;
    }
    //帧头校验
    if(rx_buff[0] != FRAME_HEAD_CHECK_1 || rx_buff[1] != FRAME_HEAD_CHECK_2 || rx_buff[2] != FRAME_HEAD_CHECK_3)
    {
        return 1;
    }

    //帧尾校验
    uint32_t sum = 0;
    uint32_t sum_receive = 0;
    for(uint8_t i = 0; i < 13; i++)
    {
        sum += rx_buff[i];
    }

    sum_receive = rx_buff[13] << 24 | rx_buff[14] << 16 | rx_buff[15] << 8 | rx_buff[16];

    if(sum != sum_receive)
    {
        return 1;
    }
    //保存数据   
    remote_data.thr = (rx_buff[3] << 8 | rx_buff[4]);
    remote_data.yaw = (rx_buff[5] << 8 | rx_buff[6]);
    remote_data.pit = (rx_buff[7] << 8 | rx_buff[8]);
    remote_data.rol = (rx_buff[9] << 8 | rx_buff[10]);
    remote_data.shutdown = rx_buff[11];
    remote_data.fix_height = rx_buff[12];
    
    debug_printf(":%d,%d,%d,%d\n",remote_data.thr,remote_data.yaw,remote_data.pit,remote_data.rol);

    return 0;
}   
/**
 * @brief 处理连接状态
 * 
 * @param res 上一次接收到数据的返回值
 */
void process_connect_state(uint8_t res)
{
    if(res == 0)
    {
        remote_state = REMOTE_CONNECTED;
        retry_count = 0;
    }
    else if(res == 1)
    {
        retry_count++;
        if(retry_count >= MAX_RETRY_TIMES)
        {
            remote_state = REMOTE_DISCONNECTED;
            retry_count = 0;
        }
    }
}
/**
 * @brief 处理解锁逻辑
 * 
 * @return 0: 解锁成功 1: 解锁失败
 */
static uint8_t process_unlock(void)
{
    //油门为0  解锁完成
    switch (thr_state)
    {
    case FREE:
        if(remote_data.thr >=  900)
        {
            thr_state = MAX;
            max_enter_time = xTaskGetTickCount();
        }
        break;
    case MAX:
        if(remote_data.thr < 900)
        {
            if(xTaskGetTickCount() - max_enter_time >= 1000)
            {
                //油门超过1s 进入LEAVE_MAX状态
                thr_state = LEAVE_MAX;
            }
            else 
            {
                //油门在最高状态未超过1s 进入FREE状态
                thr_state = FREE;
            }
        }

        break;
    case LEAVE_MAX:
        if(remote_data.thr <= 100)
        {
            //油门为0 进入MIN状态
            thr_state = MIN;
            min_enter_time = xTaskGetTickCount();
        }
        break;
    case MIN:
        //每次判断当前时间保持了多久
        if(xTaskGetTickCount() - min_enter_time <= 1000) 
        {
            if(remote_data.thr > 100)
            {
                //油门不超过1s 进入FREEE状态
                thr_state = FREE;
            }
            else 
            {
                //油门超过1s 进入UNLOCK状态
                thr_state = UNLOCK;
            }
        }

        break;
    case UNLOCK:
        /* code */
        break;
    default:
        break;
    }
    if (thr_state == UNLOCK)
    {
        return 0;
    }
    return 1;
}
/**
 * @brief 处理飞行状态
 * 
 */
void process_flight_state(void)
{
    switch (flight_state)
    {
    case IDLE:
        if(process_unlock() == 0)
        {
            flight_state = NORMAL;
            //重置解锁状态
            thr_state = FREE;
        }
        break;
    case NORMAL:
         //判断进入定高
        if(remote_data.fix_height == 1)
        {
            flight_state = FIX_HEIGHT;
            //重置解锁状态
            remote_data.fix_height = 0;
        }
        //判断故障失联状态
        if (remote_state == REMOTE_DISCONNECTED)
        {
            flight_state = FAIL;
        }
        
        break;
    case FIX_HEIGHT:
        //取消定高
        if(remote_data.fix_height == 1)
        {
            flight_state = NORMAL;
            //重置解锁状态
            remote_data.fix_height = 0;
        }
        //判断故障
        if(remote_state == REMOTE_DISCONNECTED)
        {
            flight_state = FAIL;
        }
        break;
    case FAIL:
            //处理失联状态 缓慢停止电机
            vTaskDelay(1);
            flight_state = IDLE;
        break;
    
    default:
        break;
    }
}


