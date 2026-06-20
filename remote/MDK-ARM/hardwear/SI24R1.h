#ifndef __SI24R1_H__
#define __SI24R1_H__

#include "main.h"
#include "spi.h"
#include "com_debug.h"

//stm32f103 使用 需要在cubemx中配置

//片选(SPI1_NSS)
#define CS_LOW   HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET)
#define CS_HIGH  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET)

//使能(SI_EN)
#define CE_LOW   HAL_GPIO_WritePin(SI_EN_GPIO_Port, SI_EN_Pin, GPIO_PIN_RESET)
#define CE_HIGH  HAL_GPIO_WritePin(SI_EN_GPIO_Port, SI_EN_Pin, GPIO_PIN_SET)

//选择使用的射频通道
#define CHANNEL        40
#define TX_ADR_WIDTH   5  				// 5字节宽度的发送/接收地址
#define TX_PLOAD_WIDTH 32  				// 数据通道有效数据宽度

//********************************************************************************************************************//
// SPI(SI24R1) commands
#define SI24R1_READ_REG        0x00  // Define read command to register
#define SI24R1_WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

//********************************************************************************************************************//
// SPI(SI24R1) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define RSSI            0x09  // 'Received Signal Strength Indecator' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address

//********************************************************************************************************************//
// STATUS Register 
#define RX_DR						0x40  /**/
#define TX_DS						0x20
#define MAX_RT					    0x10

//********************************************************************************************************************//
//                                        FUNCTION's PROTOTYPES                                                       //
//********************************************************************************************************************//
//SI24R1 API Functions

//spi 读 写 一个字节
static uint8_t SPI_RW(uint8_t data);
/**
 * @brief 写入寄存器(单字节)
 * @param reg 寄存器地址
 * @param data 写入的数据
 * @return 状态寄存器的值
 */
uint8_t SI24R1_Write_Reg(uint8_t reg, uint8_t data);

/**
 * @brief 写寄存器的值(多字节)
 * @param reg 寄存器地址
 * @param pBuf 写入的数据首地址
 * @param size 写入的字节数
 * @return 状态寄存器的值
 */
uint8_t SI24R1_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t size);

/**
 * @brief 读取寄存器的值(单字节)
 * @param reg 寄存器地址
 * @return 寄存器值
 */

uint8_t SI24R1_Read_Reg(uint8_t reg);


/**
 * @brief 读取寄存器的值(多字节)
 * @param reg 寄存器地址
 * @param pBuf 接收缓冲区的首地址
 * @param bytes 接收的字节数
 * @return 状态寄存器的值
 */
uint8_t SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t size);

/**
 * @brief 接收模式初始化
 */
void SI24R1_RX_Mode(void);

/**
 * 接收模式初始化
 */
 void SI24R1_TX_Mode(void);

/**
 * @brief 读取接收数据 硬件直接接收数据保存到FIFO队列中 => 通过状态标志位判断队列中是否有数据
 * @param rxbuf  接收的数据首地址
 * @return 1：没有接收到数据 0：接收到数据
 */
uint8_t SI24R1_RxPacket(uint8_t *rxbuf);

/**
 * @brief 发送一个数据包
 * @param txbuf 发送的数据
 * @return 0：发送成功 1：发送失败
 */
uint8_t SI24R1_TxPacket(uint8_t *txbuf);
/**
 * @brief SI24R1的初始化检测
 * @return 0: 接收成功 1: 接收失败
 */
uint8_t SI24R1_Check(void);
//硬件的初始化
void SI24R1_Init(void);
//********************************************************************************************************************//
#endif

