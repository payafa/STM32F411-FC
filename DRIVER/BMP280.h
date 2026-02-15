/*
 * BMP280.h
 *
 * BMP280驱动头文件
 * 用于STM32F411CEU6的BMP280传感器
 *
 * 2026-02-15
 */

#ifndef BMP280_H
#define BMP280_H

#include "I2C.h"

/* 地址 */
#define BMP280_ADDR             0xEC        // BMP280 I2C地址
#define BMP280_WHO_AM_I_REG     0xD0        // WHO_AM_I寄存器地址
#define BMP280_WHO_AM_I_VAL     0x58        // WHO_AM_I值

/* 寄存器地址定义 */
#define BMP280_RESET_REG         0xE0        // 复位寄存器
#define BMP280_CTRL_MEAS_REG     0xF4        // 控制测量寄存器
#define BMP280_CONFIG_REG        0xF5        // 配置寄存器
#define BMP280_PRESS_MSB_REG     0xF7        // 压力MSB
#define BMP280_PRESS_LSB_REG     0xF8        // 压力LSB
#define BMP280_PRESS_XLSB_REG    0xF9        // 压力XLSB
#define BMP280_TEMP_MSB_REG      0xFA        // 温度MSB
#define BMP280_TEMP_LSB_REG      0xFB        // 温度LSB
#define BMP280_TEMP_XLSB_REG     0xFC        // 温度XLSB

/* 校准寄存器 */
#define BMP280_DIG_T1_LSB_REG    0x88
#define BMP280_DIG_T1_MSB_REG    0x89
#define BMP280_DIG_T2_LSB_REG    0x8A
#define BMP280_DIG_T2_MSB_REG    0x8B
#define BMP280_DIG_T3_LSB_REG    0x8C
#define BMP280_DIG_T3_MSB_REG    0x8D
#define BMP280_DIG_P1_LSB_REG    0x8E
#define BMP280_DIG_P1_MSB_REG    0x8F
#define BMP280_DIG_P2_LSB_REG    0x90
#define BMP280_DIG_P2_MSB_REG    0x91
#define BMP280_DIG_P3_LSB_REG    0x92
#define BMP280_DIG_P3_MSB_REG    0x93
#define BMP280_DIG_P4_LSB_REG    0x94
#define BMP280_DIG_P4_MSB_REG    0x95
#define BMP280_DIG_P5_LSB_REG    0x96
#define BMP280_DIG_P5_MSB_REG    0x97
#define BMP280_DIG_P6_LSB_REG    0x98
#define BMP280_DIG_P6_MSB_REG    0x99
#define BMP280_DIG_P7_LSB_REG    0x9A
#define BMP280_DIG_P7_MSB_REG    0x9B
#define BMP280_DIG_P8_LSB_REG    0x9C
#define BMP280_DIG_P8_MSB_REG    0x9D
#define BMP280_DIG_P9_LSB_REG    0x9E
#define BMP280_DIG_P9_MSB_REG    0x9F

/* 数据结构 */
typedef struct {
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
} BMP280_CalibData_t;

typedef struct {
    uint32_t press;
    uint32_t temp;
} BMP280_RawData_t;

typedef struct {
    float press;     // 气压 (hPa)
    float temp;      // 温度 (°C)
    float altitude;  // 海拔高度 (m)
} BMP280_Data_t;

/* 函数声明 */
uint8_t BMP280_Init(void);
uint8_t BMP280_Check(void);
void BMP280_ReadRawData(BMP280_RawData_t *data);
void BMP280_ReadData(BMP280_Data_t *data);

#endif /* BMP280_H */
