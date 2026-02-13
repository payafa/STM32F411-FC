/*
 * UART2.c
 *
 * UART2驱动程序实现
 * 用于STM32F411CEU6与NRF51822的通信
 *
 * 四轴无人机飞控系统
 */

#include "UART2.h"

/* 全局变量 */
static uint8_t g_txBuffer[UART2_TX_BUFFER_SIZE];
static uint8_t g_rxBuffer[UART2_RX_BUFFER_SIZE];
static volatile uint16_t g_txIndex = 0;
static volatile uint16_t g_rxIndex = 0;
static volatile uint8_t g_rxFlag = 0;

/* 函数原型 */
static void UART2_GPIO_Config(void);
static void UART2_Config(void);
static void UART2_NVIC_Config(void);
static uint8_t CalculateChecksum(uint8_t *data, uint16_t length);

/**
 * @brief  UART2初始化
 * @param  无
 * @retval 无
 */
void UART2_Init(void)
{
    UART2_GPIO_Config();
    UART2_Config();
    UART2_NVIC_Config();
}

/**
 * @brief  GPIO配置
 * @param  无
 * @retval 无
 */
static void UART2_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 使能GPIOA和USART2的时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    /* 配置PA2为USART2_TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 配置PA3为USART2_RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 将PA2和PA3复用为USART2 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
}

/**
 * @brief  USART2配置
 * @param  无
 * @retval 无
 */
static void UART2_Config(void)
{
    USART_InitTypeDef USART_InitStructure;
    
    /* 配置USART2参数 */
    USART_InitStructure.USART_BaudRate = UART2_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    /* 初始化USART2 */
    USART_Init(USART2, &USART_InitStructure);
    
    /* 使能USART2接收中断 */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    
    /* 使能USART2 */
    USART_Cmd(USART2, ENABLE);
}

/**
 * @brief  NVIC配置
 * @param  无
 * @retval 无
 */
static void UART2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* 配置USART2中断 */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;  // 高优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  发送单个字节
 * @param  byte: 要发送的字节
 * @retval 无
 */
void UART2_SendByte(uint8_t byte)
{
    /* 等待发送缓冲区为空 */
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    
    /* 发送字节 */
    USART_SendData(USART2, byte);
}

/**
 * @brief  发送数据块
 * @param  data: 数据指针
 * @param  length: 数据长度
 * @retval 无
 */
void UART2_SendData(uint8_t *data, uint16_t length)
{
    uint16_t i;
    
    /* 发送数据 */
    for (i = 0; i < length; i++)
    {
        UART2_SendByte(data[i]);
    }
}

/**
 * @brief  计算校验和
 * @param  data: 数据指针
 * @param  length: 数据长度
 * @retval 校验和
 */
static uint8_t CalculateChecksum(uint8_t *data, uint16_t length)
{
    uint8_t checksum = 0;
    uint16_t i;
    
    /* 计算校验和 */
    for (i = 0; i < length; i++)
    {
        checksum ^= data[i];
    }
    
    return checksum;
}

/**
 * @brief  发送数据包
 * @param  packet: 数据包指针
 * @retval 无
 */
void UART2_SendPacket(Packet_t *packet)
{
    /* 计算校验和 */
    packet->checksum = CalculateChecksum(&packet->type, packet->length + 2);
    
    /* 发送数据包 */
    UART2_SendData((uint8_t *)packet, packet->length + 4);
}

/**
 * @brief  接收单个字节
 * @param  无
 * @retval 接收到的字节
 */
uint8_t UART2_ReceiveByte(void)
{
    /* 等待接收数据 */
    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
    
    /* 读取接收的数据 */
    return (uint8_t)USART_ReceiveData(USART2);
}

/**
 * @brief  接收数据块
 * @param  buffer: 接收缓冲区
 * @param  maxLength: 最大接收长度
 * @retval 实际接收的长度
 */
uint16_t UART2_ReceiveData(uint8_t *buffer, uint16_t maxLength)
{
    uint16_t length = 0;
    
    /* 接收数据 */
    while (UART2_IsDataAvailable() && length < maxLength)
    {
        buffer[length++] = UART2_ReceiveByte();
    }
    
    return length;
}

/**
 * @brief  接收数据包
 * @param  packet: 数据包指针
 * @retval 成功标志：1-成功，0-失败
 */
uint8_t UART2_ReceivePacket(Packet_t *packet)
{
    uint8_t i;
    uint8_t checksum;
    
    /* 等待数据包 */
    if (!g_rxFlag)
    {
        return 0;
    }
    
    /* 复制数据到数据包结构 */
    packet->start = g_rxBuffer[0];
    packet->type = g_rxBuffer[1];
    packet->length = g_rxBuffer[2];
    
    /* 复制数据内容 */
    for (i = 0; i < packet->length; i++)
    {
        packet->data[i] = g_rxBuffer[3 + i];
    }
    
    packet->checksum = g_rxBuffer[3 + packet->length];
    
    /* 验证校验和 */
    checksum = CalculateChecksum(&packet->type, packet->length + 2);
    if (checksum != packet->checksum)
    {
        g_rxFlag = 0;
        return 0;
    }
    
    /* 清除接收标志 */
    g_rxFlag = 0;
    
    return 1;
}

/**
 * @brief  检查是否有数据可用
 * @param  无
 * @retval 数据可用标志：1-有数据，0-无数据
 */
uint8_t UART2_IsDataAvailable(void)
{
    return USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET;
}

/**
 * @brief  USART2中断处理函数
 * @param  无
 * @retval 无
 */
void USART2_IRQHandler(void)
{
    uint8_t byte;
    
    /* 检查是否是接收中断 */
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        /* 读取接收的数据 */
        byte = USART_ReceiveData(USART2);
        
        /* 处理数据包接收 */
        if (byte == 0xAA)  // 起始字节
        {
            g_rxIndex = 0;
        }
        
        /* 存储数据到接收缓冲区 */
        if (g_rxIndex < UART2_RX_BUFFER_SIZE)
        {
            g_rxBuffer[g_rxIndex++] = byte;
        }
        
        /* 检查数据包是否完整 */
        if (g_rxIndex >= 4 && g_rxBuffer[2] + 4 == g_rxIndex)
        {
            g_rxFlag = 1;  // 数据包接收完成
        }
        
        /* 清除中断标志 */
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}
