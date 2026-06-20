#ifndef __SI24R1_H__
#define __SI24R1_H__

#include "main.h"
#include "spi.h"
#include "com_debug.h"

//stm32f103 ʹ�� ��Ҫ��cubemx������

//Ƭѡ(SPI1_NSS)
#define CS_LOW   HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET)
#define CS_HIGH  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET)

//ʹ��(SI_EN)
#define CE_LOW   HAL_GPIO_WritePin(SI_EN_GPIO_Port, SI_EN_Pin, GPIO_PIN_RESET)
#define CE_HIGH  HAL_GPIO_WritePin(SI_EN_GPIO_Port, SI_EN_Pin, GPIO_PIN_SET)

//ѡ��ʹ�õ���Ƶͨ��
#define CHANNEL        40
#define TX_ADR_WIDTH   5  				// 5�ֽڿ��ȵķ���/���յ�ַ
#define TX_PLOAD_WIDTH 32  				// ����ͨ����Ч���ݿ���

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

//spi �� д һ���ֽ�
static uint8_t SPI_RW(uint8_t data);
/**
 * @brief д��Ĵ���(���ֽ�)
 * @param reg �Ĵ�����ַ
 * @param data д�������
 * @return ״̬�Ĵ�����ֵ
 */
uint8_t SI24R1_Write_Reg(uint8_t reg, uint8_t data);

/**
 * @brief д�Ĵ�����ֵ(���ֽ�)
 * @param reg �Ĵ�����ַ
 * @param pBuf д��������׵�ַ
 * @param size д����ֽ���
 * @return ״̬�Ĵ�����ֵ
 */
uint8_t SI24R1_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t size);

/**
 * @brief ��ȡ�Ĵ�����ֵ(���ֽ�)
 * @param reg �Ĵ�����ַ
 * @return �Ĵ���ֵ
 */

uint8_t SI24R1_Read_Reg(uint8_t reg);


/**
 * @brief ��ȡ�Ĵ�����ֵ(���ֽ�)
 * @param reg �Ĵ�����ַ
 * @param pBuf ���ջ��������׵�ַ
 * @param bytes ���յ��ֽ���
 * @return ״̬�Ĵ�����ֵ
 */
uint8_t SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t size);

/**
 * @brief ����ģʽ��ʼ��
 */
void SI24R1_RX_Mode(void);

/**
 * ����ģʽ��ʼ��
 */
 void SI24R1_TX_Mode(void);

/**
 * @brief ��ȡ�������� Ӳ��ֱ�ӽ������ݱ��浽FIFO������ => ͨ��״̬��־λ�ж϶������Ƿ�������
 * @param rxbuf  ���յ������׵�ַ
 * @return 1��û�н��յ����� 0�����յ�����
 */
uint8_t SI24R1_RxPacket(uint8_t *rxbuf);

/**
 * @brief ����һ�����ݰ�
 * @param txbuf ���͵�����
 * @return 0�����ͳɹ� 1������ʧ��
 */
uint8_t SI24R1_TxPacket(uint8_t *txbuf);
/**
 * @brief SI24R1�ĳ�ʼ�����
 * @return 0: ���ճɹ� 1: ����ʧ��
 */
uint8_t SI24R1_Check(void);
//Ӳ���ĳ�ʼ��
void SI24R1_Init(void);
//********************************************************************************************************************//
#endif
