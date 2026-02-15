/*
 * UART2.h
 *
 * UART2驱动头文件
 * 用于STM32F411CEU6的串口通信
 *
 * 2026-02-15
 */

#ifndef UART2_H
#define UART2_H

#include "stm32f4xx.h"

/* 配置参数 */
#define UART2_BAUDRATE         115200      // 波特率
#define UART2_TX_BUFFER_SIZE   256         // 发送缓冲区大小
#define UART2_RX_BUFFER_SIZE   256         // 接收缓冲区大小

/* 数据包结构 */
typedef struct {
    uint8_t start;     // 起始字节 (0xAA)
    uint8_t type;      // 数据包类型
    uint8_t length;    // 数据长度
    uint8_t data[64];  // 数据内容
    uint8_t checksum;  // 校验和
} Packet_t;

/* 函数声明 */
void UART2_Init(void);
void UART2_SendByte(uint8_t byte);
void UART2_SendData(uint8_t *data, uint16_t length);
void UART2_SendPacket(Packet_t *packet);
uint8_t UART2_ReceiveByte(void);
uint16_t UART2_ReceiveData(uint8_t *buffer, uint16_t maxLength);
uint8_t UART2_ReceivePacket(Packet_t *packet);
uint8_t UART2_IsDataAvailable(void);

#endif /* UART2_H */
