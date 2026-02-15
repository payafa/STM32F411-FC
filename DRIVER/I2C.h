/*
 * I2C.h
 *
 * I2C驱动头文件
 * 用于STM32F411CEU6的I2C总线初始化
 *
 * 2026-02-15
 */

#ifndef I2C_H
#define I2C_H

#include "stm32f4xx.h"

/* 引脚定义 */
#define I2C1_SCL_PIN          GPIO_Pin_6
#define I2C1_SCL_PORT         GPIOB
#define I2C1_SDA_PIN          GPIO_Pin_7
#define I2C1_SDA_PORT         GPIOB

#define I2C2_SCL_PIN          GPIO_Pin_10
#define I2C2_SCL_PORT         GPIOB
#define I2C2_SDA_PIN          GPIO_Pin_11
#define I2C2_SDA_PORT         GPIOB

#define I2C1_CLOCK_SPEED      400000  // I2C1时钟速度 (400kHz)
#define I2C2_CLOCK_SPEED      400000  // I2C2时钟速度 (400kHz)

/* 函数声明 */
void I2C1_Init(void);
void I2C2_Init(void);

uint8_t I2C1_ReadByte(uint8_t devAddr, uint8_t regAddr);
void I2C1_ReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *buffer, uint16_t length);
void I2C1_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
void I2C1_WriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t length);

uint8_t I2C2_ReadByte(uint8_t devAddr, uint8_t regAddr);
void I2C2_ReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *buffer, uint16_t length);
void I2C2_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
void I2C2_WriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t length);

#endif /* I2C_H */
