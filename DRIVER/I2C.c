/*
 * I2C.c
 *
 * I2C基础驱动程序实现
 * 用于STM32F411CEU6与I2C设备的通信
 *
 * 四轴无人机飞控系统
 */

#include "I2C.h"

/* 函数原型 */
static void I2C1_GPIO_Config(void);
static void I2C2_GPIO_Config(void);
static void I2C1_Master_Config(void);
static void I2C2_Master_Config(void);

/**
 * @brief  I2C1初始化
 * @param  无
 * @retval 无
 */
void I2C1_Init(void)
{
    I2C1_GPIO_Config();
    I2C1_Master_Config();
}

/**
 * @brief  I2C2初始化
 * @param  无
 * @retval 无
 */
void I2C2_Init(void)
{
    I2C2_GPIO_Config();
    I2C2_Master_Config();
}

/**
 * @brief  I2C1 GPIO配置
 * @param  无
 * @retval 无
 */
static void I2C1_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 使能GPIOB和I2C1的时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    
    /* 配置PB6和PB7为I2C1的SCL和SDA引脚 */
    GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN | I2C1_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(I2C1_SCL_PORT, &GPIO_InitStructure);
    
    /* 将PB6和PB7复用为I2C1 */
    GPIO_PinAFConfig(I2C1_SCL_PORT, GPIO_PinSource6, GPIO_AF_I2C1);
    GPIO_PinAFConfig(I2C1_SCL_PORT, GPIO_PinSource7, GPIO_AF_I2C1);
}

/**
 * @brief  I2C2 GPIO配置
 * @param  无
 * @retval 无
 */
static void I2C2_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 使能GPIOB和I2C2的时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    
    /* 配置PB10和PB11为I2C2的SCL和SDA引脚 */
    GPIO_InitStructure.GPIO_Pin = I2C2_SCL_PIN | I2C2_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(I2C2_SCL_PORT, &GPIO_InitStructure);
    
    /* 将PB10和PB11复用为I2C2 */
    GPIO_PinAFConfig(I2C2_SCL_PORT, GPIO_PinSource10, GPIO_AF_I2C2);
    GPIO_PinAFConfig(I2C2_SCL_PORT, GPIO_PinSource11, GPIO_AF_I2C2);
}

/**
 * @brief  I2C1主机配置
 * @param  无
 * @retval 无
 */
static void I2C1_Master_Config(void)
{
    I2C_InitTypeDef I2C_InitStructure;
    
    /* 复位I2C1 */
    I2C_DeInit(I2C1);
    
    /* 配置I2C1 */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C1_CLOCK_SPEED;
    
    /* 初始化I2C1 */
    I2C_Init(I2C1, &I2C_InitStructure);
    
    /* 使能I2C1 */
    I2C_Cmd(I2C1, ENABLE);
}

/**
 * @brief  I2C2主机配置
 * @param  无
 * @retval 无
 */
static void I2C2_Master_Config(void)
{
    I2C_InitTypeDef I2C_InitStructure;
    
    /* 复位I2C2 */
    I2C_DeInit(I2C2);
    
    /* 配置I2C2 */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C2_CLOCK_SPEED;
    
    /* 初始化I2C2 */
    I2C_Init(I2C2, &I2C_InitStructure);
    
    /* 使能I2C2 */
    I2C_Cmd(I2C2, ENABLE);
}

/**
 * @brief  等待I2C总线空闲
 * @param  I2Cx: I2C外设
 * @retval 无
 */
static void I2C_WaitForIdle(I2C_TypeDef* I2Cx)
{
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
}

/**
 * @brief  发送起始条件
 * @param  I2Cx: I2C外设
 * @param  devAddr: 设备地址
 * @param  direction: 方向
 * @retval 成功标志
 */
static uint8_t I2C_Start(I2C_TypeDef* I2Cx, uint8_t devAddr, uint8_t direction)
{
    /* 发送起始条件 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    
    /* 等待起始条件发送完成 */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    
    /* 发送设备地址 */
    I2C_Send7bitAddress(I2Cx, devAddr, direction);
    
    /* 等待地址发送完成 */
    if (direction == I2C_Direction_Transmitter)
    {
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    }
    else
    {
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
    
    return 1;
}

/**
 * @brief  发送停止条件
 * @param  I2Cx: I2C外设
 * @retval 无
 */
static void I2C_Stop(I2C_TypeDef* I2Cx)
{
    /* 发送停止条件 */
    I2C_GenerateSTOP(I2Cx, ENABLE);
    
    /* 等待停止条件发送完成 */
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
}

/**
 * @brief  I2C1读取单个字节
 * @param  devAddr: 设备地址
 * @param  regAddr: 寄存器地址
 * @retval 读取的数据
 */
uint8_t I2C1_ReadByte(uint8_t devAddr, uint8_t regAddr)
{
    uint8_t data;
    
    /* 等待I2C总线空闲 */
    I2C_WaitForIdle(I2C1);
    
    /* 发送起始条件和设备地址（写方向） */
    I2C_Start(I2C1, devAddr, I2C_Direction_Transmitter);
    
    /* 发送寄存器地址 */
    I2C_SendData(I2C1, regAddr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    /* 发送重复起始条件和设备地址（读方向） */
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    /* 禁止应答 */
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    
    /* 读取数据 */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    data = I2C_ReceiveData(I2C1);
    
    /* 发送停止条件 */
    I2C_Stop(I2C1);
    
    /* 重新使答应答 */
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    
    return data;
}

/**
 * @brief  I2C1读取多个字节
 * @param  devAddr: 设备地址
 * @param  regAddr: 寄存器地址
 * @param  buffer: 接收缓冲区
 * @param  length: 数据长度
 * @retval 无
 */
void I2C1_ReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *buffer, uint16_t length)
{
    uint16_t i;
    
    /* 等待I2C总线空闲 */
    I2C_WaitForIdle(I2C1);
    
    /* 发送起始条件和设备地址（写方向） */
    I2C_Start(I2C1, devAddr, I2C_Direction_Transmitter);
    
    /* 发送寄存器地址 */
    I2C_SendData(I2C1, regAddr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    /* 发送重复起始条件和设备地址（读方向） */
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    /* 读取数据 */
    for (i = 0; i < length; i++)
    {
        if (i == (length - 1))
        {
            /* 最后一个字节，禁止应答 */
            I2C_AcknowledgeConfig(I2C1, DISABLE);
        }
        
        /* 等待数据接收完成 */
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
        buffer[i] = I2C_ReceiveData(I2C1);
    }
    
    /* 发送停止条件 */
    I2C_Stop(I2C1);
    
    /* 重新使答应答 */
    I2C_AcknowledgeConfig(I2C1, ENABLE);
}

/**
 * @brief  I2C1写入单个字节
 * @param  devAddr: 设备地址
 * @param  regAddr: 寄存器地址
 * @param  data: 要写入的数据
 * @retval 无
 */
void I2C1_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    /* 等待I2C总线空闲 */
    I2C_WaitForIdle(I2C1);
    
    /* 发送起始条件和设备地址（写方向） */
    I2C_Start(I2C1, devAddr, I2C_Direction_Transmitter);
    
    /* 发送寄存器地址 */
    I2C_SendData(I2C1, regAddr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    /* 发送数据 */
    I2C_SendData(I2C1, data);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    /* 发送停止条件 */
    I2C_Stop(I2C1);
}

/**
 * @brief  I2C1写入多个字节
 * @param  devAddr: 设备地址
 * @param  regAddr: 寄存器地址
 * @param  data: 要写入的数据
 * @param  length: 数据长度
 * @retval 无
 */
void I2C1_WriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t length)
{
    uint16_t i;
    
    /* 等待I2C总线空闲 */
    I2C_WaitForIdle(I2C1);
    
    /* 发送起始条件和设备地址（写方向） */
    I2C_Start(I2C1, devAddr, I2C_Direction_Transmitter);
    
    /* 发送寄存器地址 */
    I2C_SendData(I2C1, regAddr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    /* 发送数据 */
    for (i = 0; i < length; i++)
    {
        I2C_SendData(I2C1, data[i]);
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
    
    /* 发送停止条件 */
    I2C_Stop(I2C1);
}

/**
 * @brief  I2C2读取单个字节
 * @param  devAddr: 设备地址
 * @param  regAddr: 寄存器地址
 * @retval 读取的数据
 */
uint8_t I2C2_ReadByte(uint8_t devAddr, uint8_t regAddr)
{
    uint8_t data;
    
    /* 等待I2C总线空闲 */
    I2C_WaitForIdle(I2C2);
    
    /* 发送起始条件和设备地址（写方向） */
    I2C_Start(I2C2, devAddr, I2C_Direction_Transmitter);
    
    /* 发送寄存器地址 */
    I2C_SendData(I2C2, regAddr);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    /* 发送重复起始条件和设备地址（读方向） */
    I2C_GenerateSTART(I2C2, ENABLE);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C2, devAddr, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    /* 禁止应答 */
    I2C_AcknowledgeConfig(I2C2, DISABLE);
    
    /* 读取数据 */
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
    data = I2C_ReceiveData(I2C2);
    
    /* 发送停止条件 */
    I2C_Stop(I2C2);
    
    /* 重新使答应答 */
    I2C_AcknowledgeConfig(I2C2, ENABLE);
    
    return data;
}

/**
 * @brief  I2C2读取多个字节
 * @param  devAddr: 设备地址
 * @param  regAddr: 寄存器地址
 * @param  buffer: 接收缓冲区
 * @param  length: 数据长度
 * @retval 无
 */
void I2C2_ReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *buffer, uint16_t length)
{
    uint16_t i;
    
    /* 等待I2C总线空闲 */
    I2C_WaitForIdle(I2C2);
    
    /* 发送起始条件和设备地址（写方向） */
    I2C_Start(I2C2, devAddr, I2C_Direction_Transmitter);
    
    /* 发送寄存器地址 */
    I2C_SendData(I2C2, regAddr);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    /* 发送重复起始条件和设备地址（读方向） */
    I2C_GenerateSTART(I2C2, ENABLE);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C2, devAddr, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    /* 读取数据 */
    for (i = 0; i < length; i++)
    {
        if (i == (length - 1))
        {
            /* 最后一个字节，禁止应答 */
            I2C_AcknowledgeConfig(I2C2, DISABLE);
        }
        
        /* 等待数据接收完成 */
        while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
        buffer[i] = I2C_ReceiveData(I2C2);
    }
    
    /* 发送停止条件 */
    I2C_Stop(I2C2);
    
    /* 重新使答应答 */
    I2C_AcknowledgeConfig(I2C2, ENABLE);
}

/**
 * @brief  I2C2写入单个字节
 * @param  devAddr: 设备地址
 * @param  regAddr: 寄存器地址
 * @param  data: 要写入的数据
 * @retval 无
 */
void I2C2_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    /* 等待I2C总线空闲 */
    I2C_WaitForIdle(I2C2);
    
    /* 发送起始条件和设备地址（写方向） */
    I2C_Start(I2C2, devAddr, I2C_Direction_Transmitter);
    
    /* 发送寄存器地址 */
    I2C_SendData(I2C2, regAddr);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    /* 发送数据 */
    I2C_SendData(I2C2, data);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    /* 发送停止条件 */
    I2C_Stop(I2C2);
}

/**
 * @brief  I2C2写入多个字节
 * @param  devAddr: 设备地址
 * @param  regAddr: 寄存器地址
 * @param  data: 要写入的数据
 * @param  length: 数据长度
 * @retval 无
 */
void I2C2_WriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t length)
{
    uint16_t i;
    
    /* 等待I2C总线空闲 */
    I2C_WaitForIdle(I2C2);
    
    /* 发送起始条件和设备地址（写方向） */
    I2C_Start(I2C2, devAddr, I2C_Direction_Transmitter);
    
    /* 发送寄存器地址 */
    I2C_SendData(I2C2, regAddr);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    /* 发送数据 */
    for (i = 0; i < length; i++)
    {
        I2C_SendData(I2C2, data[i]);
        while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
    
    /* 发送停止条件 */
    I2C_Stop(I2C2);
}
