/*
 * BMP280.c
 *
 * BMP280驱动程序实现
 * 用于STM32F411CEU6与BMP280的通信
 *
 * 四轴无人机飞控系统
 */

#include "BMP280.h"

/* 全局变量 */
static BMP280_CalibData_t g_calibData;
static int32_t g_t_fine;

/* 常量定义 */
#define BMP280_PRESSURE_SEA_LEVEL   101325.0f  // 海平面气压 (Pa)
#define BMP280_TEMPERATURE_OFFSET  0.0f        // 温度偏移

/**
 * @brief  BMP280初始化
 * @param  无
 * @retval 成功标志：1-成功，0-失败
 */
uint8_t BMP280_Init(void)
{
    /* 检查设备是否存在 */
    if (!BMP280_Check())
    {
        return 0;
    }
    
    /* 读取校准数据 */
    BMP280_ReadCalibData();
    
    /* 复位BMP280 */
    I2C2_WriteByte(BMP280_ADDR, BMP280_RESET_REG, 0xB6);
    
    /* 延时10ms */
    for (volatile uint32_t i = 0; i < 10000; i++);
    
    /* 配置控制测量寄存器 */
    I2C2_WriteByte(BMP280_ADDR, BMP280_CTRL_MEAS_REG, 0x73);  // 正常模式，温度和气压采样率均为8次/秒
    
    /* 配置配置寄存器 */
    I2C2_WriteByte(BMP280_ADDR, BMP280_CONFIG_REG, 0xA0);  //  standby时间=1000ms，滤波器系数=4
    
    return 1;
}

/**
 * @brief  检查BMP280是否存在
 * @param  无
 * @retval 存在标志：1-存在，0-不存在
 */
uint8_t BMP280_Check(void)
{
    uint8_t whoAmI;
    
    /* 读取WHO_AM_I寄存器 */
    whoAmI = I2C2_ReadByte(BMP280_ADDR, BMP280_WHO_AM_I_REG);
    
    /* 检查WHO_AM_I值 */
    if (whoAmI == BMP280_WHO_AM_I_VAL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  读取BMP280校准数据
 * @param  无
 * @retval 无
 */
void BMP280_ReadCalibData(void)
{
    /* 读取温度校准参数 */
    g_calibData.dig_T1 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_T1_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_T1_LSB_REG);
    g_calibData.dig_T2 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_T2_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_T2_LSB_REG);
    g_calibData.dig_T3 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_T3_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_T3_LSB_REG);
    
    /* 读取气压校准参数 */
    g_calibData.dig_P1 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P1_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P1_LSB_REG);
    g_calibData.dig_P2 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P2_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P2_LSB_REG);
    g_calibData.dig_P3 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P3_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P3_LSB_REG);
    g_calibData.dig_P4 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P4_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P4_LSB_REG);
    g_calibData.dig_P5 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P5_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P5_LSB_REG);
    g_calibData.dig_P6 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P6_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P6_LSB_REG);
    g_calibData.dig_P7 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P7_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P7_LSB_REG);
    g_calibData.dig_P8 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P8_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P8_LSB_REG);
    g_calibData.dig_P9 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P9_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_P9_LSB_REG);
}

/**
 * @brief  读取BMP280原始数据
 * @param  data: 原始数据结构体指针
 * @retval 无
 */
void BMP280_ReadRawData(BMP280_RawData_t *data)
{
    uint8_t buffer[6];
    
    /* 读取气压和温度数据 */
    I2C2_ReadBytes(BMP280_ADDR, BMP280_PRESS_MSB_REG, buffer, 6);
    
    /* 解析气压数据 */
    data->press = ((uint32_t)buffer[0] << 12) | ((uint32_t)buffer[1] << 4) | ((uint32_t)buffer[2] >> 4);
    
    /* 解析温度数据 */
    data->temp = ((uint32_t)buffer[3] << 12) | ((uint32_t)buffer[4] << 4) | ((uint32_t)buffer[5] >> 4);
}

/**
 * @brief  计算温度
 * @param  adc_temp: 温度原始值
 * @retval 温度值 (°C)
 */
static float BMP280_CalculateTemperature(uint32_t adc_temp)
{
    int32_t var1, var2, T;
    
    /* 计算温度 */
    var1 = ((((adc_temp >> 3) - ((int32_t)g_calibData.dig_T1 << 1)) * ((int32_t)g_calibData.dig_T2)) >> 11);
    var2 = (((((adc_temp >> 4) - ((int32_t)g_calibData.dig_T1)) * ((adc_temp >> 4) - ((int32_t)g_calibData.dig_T1))) >> 12) * ((int32_t)g_calibData.dig_T3) >> 14);
    g_t_fine = var1 + var2;
    T = (g_t_fine * 5 + 128) >> 8;
    
    return (float)T / 100.0f + BMP280_TEMPERATURE_OFFSET;
}

/**
 * @brief  计算气压
 * @param  adc_press: 气压原始值
 * @retval 气压值 (hPa)
 */
static float BMP280_CalculatePressure(uint32_t adc_press)
{
    int64_t var1, var2, p;
    
    /* 计算气压 */
    var1 = ((int64_t)g_t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)g_calibData.dig_P6;
    var2 = var2 + ((var1 * (int64_t)g_calibData.dig_P5) << 17);
    var2 = var2 + (((int64_t)g_calibData.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)g_calibData.dig_P3) >> 8) + ((var1 * (int64_t)g_calibData.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)g_calibData.dig_P1) >> 33;
    
    if (var1 == 0)
    {
        return 0;  // 避免除零错误
    }
    
    p = 1048576 - adc_press;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)g_calibData.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)g_calibData.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)g_calibData.dig_P7) << 4);
    
    return (float)p / 256.0f / 100.0f;  // 转换为hPa
}

/**
 * @brief  计算海拔高度
 * @param  pressure: 气压值 (hPa)
 * @retval 海拔高度 (m)
 */
static float BMP280_CalculateAltitude(float pressure)
{
    float altitude;
    
    /* 使用气压计算海拔高度 */
    altitude = 44330.0f * (1.0f - pow(pressure * 100.0f / BMP280_PRESSURE_SEA_LEVEL, 1.0f / 5.255f));
    
    return altitude;
}

/**
 * @brief  读取BMP280处理后的数据
 * @param  data: 处理后的数据结构体指针
 * @retval 无
 */
void BMP280_ReadData(BMP280_Data_t *data)
{
    BMP280_RawData_t rawData;
    
    /* 读取原始数据 */
    BMP280_ReadRawData(&rawData);
    
    /* 计算温度 */
    data->temp = BMP280_CalculateTemperature(rawData.temp);
    
    /* 计算气压 */
    data->press = BMP280_CalculatePressure(rawData.press);
    
    /* 计算海拔高度 */
    data->altitude = BMP280_CalculateAltitude(data->press);
}
