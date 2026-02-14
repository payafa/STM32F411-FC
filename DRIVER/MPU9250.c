/*
 * MPU9250.c
 *
 * MPU9250???????????
 * ????STM32F411CEU6??MPU9250?????
 *
 * ??????????????
 */

#include "MPU9250.h"

/* ?????? */
static MPU9250_CalibData_t g_calibData = {
    .accelBiasX = 0.0f,
    .accelBiasY = 0.0f,
    .accelBiasZ = 0.0f,
    .gyroBiasX = 0.0f,
    .gyroBiasY = 0.0f,
    .gyroBiasZ = 0.0f
};

/* ?????????? */
#define ACCEL_SENSITIVITY_2G     16384.0f  // 2g?????????????
#define GYRO_SENSITIVITY_250DPS   131.0f    // 250dps?????????????
#define TEMP_SENSITIVITY         333.87f    // ?????????
#define TEMP_OFFSET             21.0f      // ??????

/**
 * @brief  MPU9250?????
 * @param  ??
 * @retval ????????1-?????0-???
 */
uint8_t MPU9250_Init(void)
{
    /* ????υτ?????? */
    if (!MPU9250_Check())
    {
        return 0;
    }
    
    /* ??????????????? */
    I2C1_WriteByte(MPU9250_ADDR, MPU9250_PWR_MGMT_1_REG, 0x00);  // ????MPU9250
    
    /* ???¨°????????? */
    I2C1_WriteByte(MPU9250_ADDR, MPU9250_SMPLRT_DIV_REG, 0x07);  // ?????? = 1kHz / (7+1) = 125Hz
    
    /* ??????????? */
    I2C1_WriteByte(MPU9250_ADDR, MPU9250_CONFIG_REG, 0x06);  // ?????????????? = 5Hz
    
    /* ?????????????? */
    I2C1_WriteByte(MPU9250_ADDR, MPU9250_GYRO_CONFIG_REG, 0x00);  // ?????????? = ??250dps
    
    /* ???¨Ή????????? */
    I2C1_WriteByte(MPU9250_ADDR, MPU9250_ACCEL_CONFIG_REG, 0x00);  // ?????????? = ??2g
    
    return 1;
}

/**
 * @brief  ???MPU9250??????
 * @param  ??
 * @retval ????????1-?????0-??????
 */
uint8_t MPU9250_Check(void)
{
    uint8_t whoAmI;
    
    /* ???WHO_AM_I????? */
    whoAmI = I2C1_ReadByte(MPU9250_ADDR, MPU9250_WHO_AM_I_REG);
    
    /* ???WHO_AM_I? */
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
 * @brief  ?????????
 * @param  data: ????????????
 * @retval ??
 */
void MPU9250_ReadRawData(MPU9250_RawData_t *data)
{
    uint8_t buffer[14];
    
    /* ???14?????????? */
    I2C1_ReadBytes(MPU9250_ADDR, MPU9250_ACCEL_XOUT_H_REG, buffer, 14);
    
    /* ???????? */
    data->accelX = (buffer[0] << 8) | buffer[1];
    data->accelY = (buffer[2] << 8) | buffer[3];
    data->accelZ = (buffer[4] << 8) | buffer[5];
    data->temp = (buffer[6] << 8) | buffer[7];
    data->gyroX = (buffer[8] << 8) | buffer[9];
    data->gyroY = (buffer[10] << 8) | buffer[11];
    data->gyroZ = (buffer[12] << 8) | buffer[13];
}

/**
 * @brief  ??????????????
 * @param  data: ?????????????????
 * @retval ??
 */
void MPU9250_ReadData(MPU9250_Data_t *data)
{
    MPU9250_RawData_t rawData;
    
    /* ????????? */
    MPU9250_ReadRawData(&rawData);
    
    /* ??????????¦Λ?????§µ? */
    data->accelX = ((float)rawData.accelX / ACCEL_SENSITIVITY_2G) - g_calibData.accelBiasX;
    data->accelY = ((float)rawData.accelY / ACCEL_SENSITIVITY_2G) - g_calibData.accelBiasY;
    data->accelZ = ((float)rawData.accelZ / ACCEL_SENSITIVITY_2G) - g_calibData.accelBiasZ;
    data->gyroX = ((float)rawData.gyroX / GYRO_SENSITIVITY_250DPS) - g_calibData.gyroBiasX;
    data->gyroY = ((float)rawData.gyroY / GYRO_SENSITIVITY_250DPS) - g_calibData.gyroBiasY;
    data->gyroZ = ((float)rawData.gyroZ / GYRO_SENSITIVITY_250DPS) - g_calibData.gyroBiasZ;
    data->temp = ((float)rawData.temp / TEMP_SENSITIVITY) + TEMP_OFFSET;
}

/**
 * @brief  §µ?MPU9250
 * @param  calibData: §µ???????????
 * @retval ??
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
    
    /* ????100?????? */
    for (i = 0; i < 100; i++)
    {
        MPU9250_ReadRawData(&rawData);
        accelXBias += rawData.accelX;
        accelYBias += rawData.accelY;
        accelZBias += rawData.accelZ;
        gyroXBias += rawData.gyroX;
        gyroYBias += rawData.gyroY;
        gyroZBias += rawData.gyroZ;
        
        /* ???10ms */
        for (volatile uint32_t j = 0; j < 10000; j++);
    }
    
    /* ???????? */
    accelXBias /= 100;
    accelYBias /= 100;
    accelZBias /= 100;
    gyroXBias /= 100;
    gyroYBias /= 100;
    gyroZBias /= 100;
    
    /* ??????? */
    calibData->accelBiasX = (float)accelXBias / ACCEL_SENSITIVITY_2G;
    calibData->accelBiasY = (float)accelYBias / ACCEL_SENSITIVITY_2G;
    calibData->accelBiasZ = ((float)accelZBias / ACCEL_SENSITIVITY_2G) - 1.0f;  // ????????????
    calibData->gyroBiasX = (float)gyroXBias / GYRO_SENSITIVITY_250DPS;
    calibData->gyroBiasY = (float)gyroYBias / GYRO_SENSITIVITY_250DPS;
    calibData->gyroBiasZ = (float)gyroZBias / GYRO_SENSITIVITY_250DPS;
    
    /* ????§µ????? */
    g_calibData = *calibData;
}
