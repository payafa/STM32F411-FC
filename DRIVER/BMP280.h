/*
 * BMP280.h
 *
 * BMP280驱动程序头文件
 * 用于STM32F411CEU6与BMP280的通信
 *
 * 四轴无人机飞控系统
 */

#ifndef BMP280_H
#define BMP280_H

#include "I2C.h"

/* 宏定义 */
#define BMP280_ADDR             0xEC        // BMP280 I2C地址
#define BMP280_WHO_AM_I_REG     0xD0        // WHO_AM_I寄存器地址
#define BMP280_WHO_AM_I_VAL     0x58        // WHO_AM_I值

/* 寄存器地址定义 */
#define BMP280_TEMP_XLSB_REG     0xFC        // 温度最低有效位
#define BMP280_TEMP_LSB_REG      0xFB        // 温度低有效位
#define BMP280_TEMP_MSB_REG      0xFA        // 温度高有效位
#define BMP280_PRESS_XLSB_REG    0xF9        // 气压最低有效位
#define BMP280_PRESS_LSB_REG     0xF8        // 气压低有效位
#define BMP280_PRESS_MSB_REG     0xF7        // 气压高有效位
#define BMP280_CONFIG_REG        0xF5        // 配置
#define BMP280_CTRL_MEAS_REG     0xF4        // 控制测量
#define BMP280_STATUS_REG        0xF3        // 状态
#define BMP280_RESET_REG         0xE0        // 复位

/* 校准参数寄存器 */
#define BMP280_DIG_T1_LSB_REG    0x88        // 温度校准参数T1低字节
#define BMP280_DIG_T1_MSB_REG    0x89        // 温度校准参数T1高字节
#define BMP280_DIG_T2_LSB_REG    0x8A        // 温度校准参数T2低字节
#define BMP280_DIG_T2_MSB_REG    0x8B        // 温度校准参数T2高字节
#define BMP280_DIG_T3_LSB_REG    0x8C        // 温度校准参数T3低字节
#define BMP280_DIG_T3_MSB_REG    0x8D        // 温度校准参数T3高字节
#define BMP280_DIG_P1_LSB_REG    0x8E        // 气压校准参数P1低字节
#define BMP280_DIG_P1_MSB_REG    0x8F        // 气压校准参数P1高字节
#define BMP280_DIG_P2_LSB_REG    0x90        // 气压校准参数P2低字节
#define BMP280_DIG_P2_MSB_REG    0x91        // 气压校准参数P2高字节
#define BMP280_DIG_P3_LSB_REG    0x92        // 气压校准参数P3低字节
#define BMP280_DIG_P3_MSB_REG    0x93        // 气压校准参数P3高字节
#define BMP280_DIG_P4_LSB_REG    0x94        // 气压校准参数P4低字节
#define BMP280_DIG_P4_MSB_REG    0x95        // 气压校准参数P4高字节
#define BMP280_DIG_P5_LSB_REG    0x96        // 气压校准参数P5低字节
#define BMP280_DIG_P5_MSB_REG    0x97        // 气压校准参数P5高字节
#define BMP280_DIG_P6_LSB_REG    0x98        // 气压校准参数P6低字节
#define BMP280_DIG_P6_MSB_REG    0x99        // 气压校准参数P6高字节
#define BMP280_DIG_P7_LSB_REG    0x9A        // 气压校准参数P7低字节
#define BMP280_DIG_P7_MSB_REG    0x9B        // 气压校准参数P7高字节
#define BMP280_DIG_P8_LSB_REG    0x9C        // 气压校准参数P8低字节
#define BMP280_DIG_P8_MSB_REG    0x9D        // 气压校准参数P8高字节
#define BMP280_DIG_P9_LSB_REG    0x9E        // 气压校准参数P9低字节
#define BMP280_DIG_P9_MSB_REG    0x9F        // 气压校准参数P9高字节

/* 数据结构定义 */
typedef struct {
    uint16_t dig_T1;  // 温度校准参数T1
    int16_t  dig_T2;  // 温度校准参数T2
    int16_t  dig_T3;  // 温度校准参数T3
    uint16_t dig_P1;  // 气压校准参数P1
    int16_t  dig_P2;  // 气压校准参数P2
    int16_t  dig_P3;  // 气压校准参数P3
    int16_t  dig_P4;  // 气压校准参数P4
    int16_t  dig_P5;  // 气压校准参数P5
    int16_t  dig_P6;  // 气压校准参数P6
    int16_t  dig_P7;  // 气压校准参数P7
    int16_t  dig_P8;  // 气压校准参数P8
    int16_t  dig_P9;  // 气压校准参数P9
} BMP280_CalibData_t;

typedef struct {
    uint32_t press;   // 气压原始值
    uint32_t temp;    // 温度原始值
} BMP280_RawData_t;

typedef struct {
    float press;      // 气压值 (hPa)
    float temp;       // 温度值 (°C)
    float altitude;   // 海拔高度 (m)
} BMP280_Data_t;

/* 函数声明 */
uint8_t BMP280_Init(void);
uint8_t BMP280_Check(void);
void BMP280_ReadRawData(BMP280_RawData_t *data);
void BMP280_ReadData(BMP280_Data_t *data);
void BMP280_ReadCalibData(void);

#endif /* BMP280_H */
