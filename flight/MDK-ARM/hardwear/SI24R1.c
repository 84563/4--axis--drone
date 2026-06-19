#include "SI24R1.h"

//定义一静态的发送地址 => 发送和接收是同一个地址
uint8_t  TX_ADDRESS[TX_ADR_WIDTH] = {0x0A,0x01,0x07,0x0E,0x01}; 


//spi 读 写 一个字节
static uint8_t SPI_RW(uint8_t data)
{
    uint8_t rx_data = 0;
    HAL_SPI_TransmitReceive(&hspi1, &data, &rx_data, 1, 1000);
    return rx_data;
}

/**
 * @brief 写入寄存器(单字节)
 * @param reg 寄存器地址
 * @param data 写入的数据
 * @return 状态寄存器的值
 */
uint8_t SI24R1_Write_Reg(uint8_t reg, uint8_t data)
{
   uint8_t status = 0;
   
   CS_LOW;
   status = SPI_RW(reg);
   SPI_RW(data);
   CS_HIGH;

   return status;
}

/**
 * @brief 写寄存器的值(多字节)
 * @param reg 寄存器地址
 * @param pBuf 写入的数据首地址
 * @param size 写入的字节数
 * @return 状态寄存器的值
 */
uint8_t SI24R1_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t size)
{
   uint8_t status,byte_ctr;
   
   CS_LOW;
   status = SPI_RW(reg);
   for(byte_ctr=0; byte_ctr < size; byte_ctr++)
   {
      SPI_RW(*pBuf++);
   }
   CS_HIGH;

   return status;
}

/**
 * @brief 读取寄存器的值(单字节)
 * @param reg 寄存器地址
 * @return 寄存器值
 */

uint8_t SI24R1_Read_Reg(uint8_t reg)
{
    uint8_t value;

    CS_LOW;
    SPI_RW(reg);
    value = SPI_RW(0);
    CS_HIGH;

    return value;   
}


/**
 * @brief 读取寄存器的值(多字节)
 * @param reg 寄存器地址
 * @param pBuf 接收缓冲区的首地址
 * @param bytes 接收的字节数
 * @return 状态寄存器的值
 */
uint8_t SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t size)
{
    uint8_t status,byte_ctr;
    
    CS_LOW;
    status = SPI_RW(reg);
    for(byte_ctr=0; byte_ctr < size; byte_ctr++)
    {
        pBuf[byte_ctr] = SPI_RW(0);
    }
    CS_HIGH;

    return status;
}

/**
 * @brief 接收模式初始化
 */
void SI24R1_RX_Mode(void)
{
    CE_LOW;

    SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);//设置接收地址
    SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);//使能接收通道0自动应答
    SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01); //使能接收通道0
    SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, CHANNEL); //设置射频通道为40
    SI24R1_Write_Reg(SI24R1_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH); //接收通道0选择和发送通道有效宽度
    SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x06); //设置RF输出功率为4dBm，发射速率1Mbps
    SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x0F); //CRC使能 16位CRC校验，上电，接收    
    SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, 0xFF); //清除所有中断标志
   
    CE_HIGH;
}

/**
 * 接收模式初始化
 */
 void SI24R1_TX_Mode(void)
 {
    CE_LOW;

    SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);//写入发送地址
    SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);//为了应答接收设备 接收通道0地址和发送地址相同
    SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);//使能接收通道0自动应答
    SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01);//使能接收通道0
    SI24R1_Write_Reg(SI24R1_WRITE_REG + SETUP_RETR, 0x0A);//自动重发延时等待250us，自动重发次数为10次
    SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, CHANNEL);//设置射频通道为40
    SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x06);//设置RF输出功率为4dBm，发射速率1Mbps
    SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0X0E);//CRC使能 16位CRC校验，上电，发送

    CE_HIGH;
 }

/**
 * @brief 读取接收数据 硬件直接接收数据保存到FIFO队列中 => 通过状态标志位判断队列中是否有数据
 * @param rxbuf  接收的数据首地址
 * @return 1：没有接收到数据 0：接收到数据
 */
uint8_t SI24R1_RxPacket(uint8_t *rxbuf)
{
    uint8_t state;

    state = SI24R1_Read_Reg(STATUS);
    SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, state);//清楚RX_DR 中断标志位

    if(state & RX_DR)//接收到数据
    {
        SI24R1_Read_Buf(RD_RX_PLOAD, rxbuf, TX_PLOAD_WIDTH);
        SI24R1_Write_Reg(FLUSH_RX, 0XFF);//清除RX FIFO寄存器
    }
    return 1;//没有接收到数据
}

/**
 * @brief 发送一个数据包
 * @param txbuf 发送的数据
 * @return 0：发送成功 1：发送失败
 */
uint8_t SI24R1_TxPacket(uint8_t *txbuf)
{
   uint8_t state;

   CE_LOW;
   SI24R1_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);//写入发送数据
   CE_HIGH;
   
   //等待发送完成
   //while(IRQ == 1)
   
   state = SI24R1_Read_Reg(STATUS);

   while (((state & TX_DS) == 0) && ((state & MAX_RT) == 0))
   {
        state = SI24R1_Read_Reg(STATUS);
   }
   
   SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, state); //清除中断标志

   if(state & MAX_RT)
   {
       SI24R1_Write_Reg(FLUSH_TX, 0XFF);//清除TX FIFO寄存器
       return 1; //发送失败
   }
   if(state & TX_DS)
   {
       return 0; //发送成功
   }
   return 1; //发送失败
}
/**
 * @brief SI24R1的初始化检测
 * @return 0: 接收成功 1: 接收失败
 */
uint8_t si24r1_rx_buf[5] = {0};
uint8_t SI24R1_Check(void)
{
    //测试spi通信是否正常读写寄存器
    SI24R1_Read_Buf(SI24R1_READ_REG + TX_ADDR,si24r1_rx_buf,TX_ADR_WIDTH);
    //写入发送地址
    SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);
    //读取同样数据
    SI24R1_Read_Buf(SI24R1_READ_REG + TX_ADDR,si24r1_rx_buf,TX_ADR_WIDTH);
    for (uint8_t i = 0; i < TX_ADR_WIDTH; i++)
    {
        if (si24r1_rx_buf[i] != TX_ADDRESS[i])
        {
            return 1; //初始化失败
        }
    }       /* code */
    
    return 0;
}
//硬件的初始化
void SI24R1_Init(void)
{
    HAL_Delay(200);
  
    while (SI24R1_Check() == 1)
    {
        HAL_Delay(10);
    }
    
    SI24R1_RX_Mode();
    debug_printf("SI24R1 Init OK\n");
}
