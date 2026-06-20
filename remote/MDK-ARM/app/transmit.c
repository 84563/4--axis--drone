#include "transmit.h"


extern Remote_Data remote_data;
uint8_t transmit_buff[TX_PLOAD_WIDTH] = {0};



/**
 * @brief 自动切换SI24R1的模式 => 将采集的ADC数据发送给飞控
 */
void transmit_data(void)
{
    //切换到TX模式
    SI24R1_TX_Mode();

    //发送数据
    uint32_t sum = 0;

    //帧头
    transmit_buff[0] = FRAME_HEAD_CHECK1;
    transmit_buff[1] = FRAME_HEAD_CHECK2;
    transmit_buff[2] = FRAME_HEAD_CHECK3;

    //高8位在前
    transmit_buff[3] = (remote_data.thr >> 8) & 0xFF;
    transmit_buff[4] = remote_data.thr & 0xFF;

    transmit_buff[5] = (remote_data.yaw >> 8) & 0xFF;
    transmit_buff[6] = remote_data.yaw & 0xFF;

    transmit_buff[7] = (remote_data.pit >> 8) & 0xFF;
    transmit_buff[8] = remote_data.pit & 0xFF;

    transmit_buff[9] = (remote_data.rol >> 8) & 0xFF;
    transmit_buff[10] = remote_data.rol & 0xFF;

    taskENTER_CRITICAL();
    transmit_buff[11] = remote_data.shutdown;
    remote_data.shutdown = 0;
    transmit_buff[12] = remote_data.fix_height;
    remote_data.fix_height = 0;
    taskEXIT_CRITICAL();

    for (uint8_t i = 0; i < 13; i++)
    {
        sum += transmit_buff[i];
    }
    transmit_buff[13] = (sum >> 24) & 0xFF;
    transmit_buff[14] = (sum >> 16) & 0xFF;
	transmit_buff[15] = (sum >> 8) & 0xFF;
	transmit_buff[16] = sum & 0xFF;
    
    debug_printf(":%d,%d,%d,%d\r\n",remote_data.thr,remote_data.yaw,remote_data.pit,remote_data.rol);

    SI24R1_TxPacket(transmit_buff);

    //切换到RX模式
    SI24R1_RX_Mode();
}
