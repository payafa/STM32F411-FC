/*
 * MPU9250.c
 *
 * MPU9250驱动程序实现
 * 用于STM32F411CEU6与MPU9250的通信
 *
 * 四轴无人机飞控系统
 */

#include "MPU9250.h"

/* 全局变量 */
static MPU9250_CalibData_t g_calibData = {
    .accelBiasX = 0.0f,
    .accelBiasY = 0.0f,
    .accelBiasZ = 0.0f,
    .gyroBiasX = 0.0f,
    .gyroBiasY = 0.0f,
    .gyroBiasZ = 0.0f
};

/* 灵敏度因子 */
#define ACCEL_SENSITIVITY_2G     16384.0f  // 2g量程下的灵敏度
#define GYRO_SENSITIVITY_250DPS   131.0f    // 250dps量程下的灵敏度
#define TEMP_SENSITIVITY         333.87f    // 温度灵敏度
#define TEMP_OFFSET             21.0f      // 温度偏移

/**
 * @brief  MPU9250初始化
 * @param  无
 * @retval 成功标志：1-成功，0-失败
 */
uint8_t MPU9250_Init(void)
{
    /* 检查设备是否存在 */
    if (!MPU9250_Check())
    {
        return 0;
    }
    
    /* 配置电源管理寄存器 */
    I2C1_WriteByte(MPU9250_ADDR, MPU9250_PWR_MGMT_1_REG, 0x00);  // 唤醒MPU9250
    
    /* 配置采样率分频器 */
    I2C1_WriteByte(MPU9250_ADDR, MPU9250_SMPLRT_DIV_REG, 0x07);  // 采样率 = 1kHz / (7+1) = 125Hz
    
    /* 配置低通滤波器 */
    I2C1_WriteByte(MPU9250_ADDR, MPU9250_CONFIG_REG, 0x06);  // 低通滤波器截止频率 = 5Hz
    
    /* 配置陀螺仪量程 */
    I2C1_WriteByte(MPU9250_ADDR, MPU9250_GYRO_CONFIG_REG, 0x00);  // 陀螺仪量程 = ±250dps
    
    /* 配置加速度计量程 */
    I2C1_WriteByte(MPU9250_ADDR, MPU9250_ACCEL_CONFIG_REG, 0x00);  // 加速度计量程 = ±2g
    
    return 1;
}

/**
 * @brief  检查MPU9250是否存在
 * @param  无
 * @retval 存在标志：1-存在，0-不存在
 */
uint8_t MPU9250_Check(void)
{
    uint8_t whoAmI;
    
    /* 读取WHO_AM_I寄存器 */
    whoAmI = I2C1_ReadByte(MPU9250_ADDR, MPU9250_WHO_AM_I_REG);
    
    /* 检查WHO_AM_I值 */
    if (whoAmI == MPU9250_WHO_AM_I_VAL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  读取原始数据
 * @param  data: 原始数据结构体指针
 * @retval 无
 */
void MPU9250_ReadRawData(MPU9250_RawData_t *data)
{
    uint8_t buffer[14];
    
    /* 读取14个字节的数据 */
    I2C1_ReadBytes(MPU9250_ADDR, MPU9250_ACCEL_XOUT_H_REG, buffer, 14);
    
    /* 解析数据 */
    data->accelX = (buffer[0] << 8) | buffer[1];
    data->accelY = (buffer[2] << 8) | buffer[3];
    data->accelZ = (buffer[4] << 8) | buffer[5];
    data->temp = (buffer[6] << 8) | buffer[7];
    data->gyroX = (buffer[8] << 8) | buffer[9];
    data->gyroY = (buffer[10] << 8) | buffer[11];
    data->gyroZ = (buffer[12] << 8) | buffer[13];
}

/**
 * @brief  读取处理后的数据
 * @param  data: 处理后的数据结构体指针
 * @retval 无
 */
void MPU9250_ReadData(MPU9250_Data_t *data)
{
    MPU9250_RawData_t rawData;
    
    /* 读取原始数据 */
    MPU9250_ReadRawData(&rawData);
    
    /* 转换为物理单位并应用校准 */
    data->accelX = ((float)rawData.accelX / ACCEL_SENSITIVITY_2G) - g_calibData.accelBiasX;
    data->accelY = ((float)rawData.accelY / ACCEL_SENSITIVITY_2G) - g_calibData.accelBiasY;
    data->accelZ = ((float)rawData.accelZ / ACCEL_SENSITIVITY_2G) - g_calibData.accelBiasZ;
    data->gyroX = ((float)rawData.gyroX / GYRO_SENSITIVITY_250DPS) - g_calibData.gyroBiasX;
    data->gyroY = ((float)rawData.gyroY / GYRO_SENSITIVITY_250DPS) - g_calibData.gyroBiasY;
    data->gyroZ = ((float)rawData.gyroZ / GYRO_SENSITIVITY_250DPS) - g_calibData.gyroBiasZ;
    data->temp = ((float)rawData.temp / TEMP_SENSITIVITY) + TEMP_OFFSET;
}

/**
 * @brief  校准MPU9250
 * @param  calibData: 校准数据结构体指针
 * @retval 无
 */
void MPU9250_Calibrate(MPU9250_CalibData_t *calibData)
{
    MPU9250_RawData_t rawData;
    int32_t accelXBias = 0;
    int32_t accelYBias = 0;
    int32_t accelZBias = 0;
    int32_t gyroXBias = 0;
    int32_t gyroYBias = 0;
    int32_t gyroZBias = 0;
    uint16_t i;
    
    /* 采样100次数据 */
    for (i = 0; i < 100; i++)
    {
        MPU9250_ReadRawData(&rawData);
        accelXBias += rawData.accelX;
        accelYBias += rawData.accelY;
        accelZBias += rawData.accelZ;
        gyroXBias += rawData.gyroX;
        gyroYBias += rawData.gyroY;
        gyroZBias += rawData.gyroZ;
        
        /* 延时10ms */
        for (volatile uint32_t j = 0; j < 10000; j++);
    }
    
    /* 计算平均值 */
    accelXBias /= 100;
    accelYBias /= 100;
    accelZBias /= 100;
    gyroXBias /= 100;
    gyroYBias /= 100;
    gyroZBias /= 100;
    
    /* 计算偏置 */
    calibData->accelBiasX = (float)accelXBias / ACCEL_SENSITIVITY_2G;
    calibData->accelBiasY = (float)accelYBias / ACCEL_SENSITIVITY_2G;
    calibData->accelBiasZ = ((float)accelZBias / ACCEL_SENSITIVITY_2G) - 1.0f;  // 减去重力加速度
    calibData->gyroBiasX = (float)gyroXBias / GYRO_SENSITIVITY_250DPS;
    calibData->gyroBiasY = (float)gyroYBias / GYRO_SENSITIVITY_250DPS;
    calibData->gyroBiasZ = (float)gyroZBias / GYRO_SENSITIVITY_250DPS;
    
    /* 保存校准数据 */
    g_calibData = *calibData;
}
