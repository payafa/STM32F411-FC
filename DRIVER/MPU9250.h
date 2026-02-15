/*
 * MPU9250.h
 *
 * MPU9250驱动头文件
 * 用于STM32F411CEU6的MPU9250传感器
 *
 * 2026-02-15
 */

#ifndef MPU9250_H
#define MPU9250_H

#include "I2C.h"

/* 地址 */
#define MPU9250_ADDR             0xD2        // MPU9250 I2C地址
#define MPU9250_WHO_AM_I_REG     0x75        // WHO_AM_I寄存器地址
#define MPU9250_WHO_AM_I_VAL     0x71        // WHO_AM_I值

/* 寄存器地址定义 */
#define MPU9250_SMPLRT_DIV_REG    0x19        // 采样率分频器
#define MPU9250_CONFIG_REG        0x1A        // 配置
#define MPU9250_GYRO_CONFIG_REG   0x1B        // 陀螺仪配置
#define MPU9250_ACCEL_CONFIG_REG  0x1C        // 加速度计配置
#define MPU9250_ACCEL_XOUT_H_REG  0x3B        // 加速度X轴高位
#define MPU9250_ACCEL_XOUT_L_REG  0x3C        // 加速度X轴低位
#define MPU9250_ACCEL_YOUT_H_REG  0x3D        // 加速度Y轴高位
#define MPU9250_ACCEL_YOUT_L_REG  0x3E        // 加速度Y轴低位
#define MPU9250_ACCEL_ZOUT_H_REG  0x3F        // 加速度Z轴高位
#define MPU9250_ACCEL_ZOUT_L_REG  0x40        // 加速度Z轴低位
#define MPU9250_TEMP_OUT_H_REG    0x41        // 温度高位
#define MPU9250_TEMP_OUT_L_REG    0x42        // 温度低位
#define MPU9250_GYRO_XOUT_H_REG   0x43        // 陀螺仪X轴高位
#define MPU9250_GYRO_XOUT_L_REG   0x44        // 陀螺仪X轴低位
#define MPU9250_GYRO_YOUT_H_REG   0x45        // 陀螺仪Y轴高位
#define MPU9250_GYRO_YOUT_L_REG   0x46        // 陀螺仪Y轴低位
#define MPU9250_GYRO_ZOUT_H_REG   0x47        // 陀螺仪Z轴高位
#define MPU9250_GYRO_ZOUT_L_REG   0x48        // 陀螺仪Z轴低位
#define MPU9250_PWR_MGMT_1_REG    0x6B        // 电源管理1
#define MPU9250_PWR_MGMT_2_REG    0x6C        // 电源管理2

/* 数据结构 */
typedef struct {
    int16_t accelX;  // 加速度X轴原始值
    int16_t accelY;  // 加速度Y轴原始值
    int16_t accelZ;  // 加速度Z轴原始值
    int16_t gyroX;   // 陀螺仪X轴原始值
    int16_t gyroY;   // 陀螺仪Y轴原始值
    int16_t gyroZ;   // 陀螺仪Z轴原始值
    int16_t temp;    // 温度原始值
} MPU9250_RawData_t;

typedef struct {
    float accelX;  // 加速度X轴 (g)
    float accelY;  // 加速度Y轴 (g)
    float accelZ;  // 加速度Z轴 (g)
    float gyroX;   // 陀螺仪X轴 (deg/s)
    float gyroY;   // 陀螺仪Y轴 (deg/s)
    float gyroZ;   // 陀螺仪Z轴 (deg/s)
    float temp;    // 温度 (°C)
} MPU9250_Data_t;

typedef struct {
    float accelBiasX;  // 加速度X轴偏移
    float accelBiasY;  // 加速度Y轴偏移
    float accelBiasZ;  // 加速度Z轴偏移
    float gyroBiasX;   // 陀螺仪X轴偏移
    float gyroBiasY;   // 陀螺仪Y轴偏移
    float gyroBiasZ;   // 陀螺仪Z轴偏移
} MPU9250_CalibData_t;

/* 函数声明 */
uint8_t MPU9250_Init(void);
uint8_t MPU9250_Check(void);
void MPU9250_ReadRawData(MPU9250_RawData_t *data);
void MPU9250_ReadData(MPU9250_Data_t *data);
void MPU9250_Calibrate(MPU9250_CalibData_t *calibData);

#endif /* MPU9250_H */
