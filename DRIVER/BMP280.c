/*
 * BMP280.c
 *
 * BMP280???????????
 * ????STM32F411CEU6??BMP280?????
 *
 * ??????????????
 */

#include "BMP280.h"

/* ?????? */
static BMP280_CalibData_t g_calibData;
static int32_t g_t_fine;

/* ???????? */
#define BMP280_PRESSURE_SEA_LEVEL   101325.0f  // ???????? (Pa)
#define BMP280_TEMPERATURE_OFFSET  0.0f        // ??????

/**
 * @brief  BMP280?????
 * @param  ??
 * @retval ????????1-?????0-???
 */
uint8_t BMP280_Init(void)
{
    /* ????õô?????? */
    if (!BMP280_Check())
    {
        return 0;
    }
    
    /* ???§µ????? */
    BMP280_ReadCalibData();
    
    /* ??¦ËBMP280 */
    I2C2_WriteByte(BMP280_ADDR, BMP280_RESET_REG, 0xB6);
    
    /* ???10ms */
    for (volatile uint32_t i = 0; i < 10000; i++);
    
    /* ??????????????? */
    I2C2_WriteByte(BMP280_ADDR, BMP280_CTRL_MEAS_REG, 0x73);  // ???????????????????????8??/??
    
    /* ???????¨¹???? */
    I2C2_WriteByte(BMP280_ADDR, BMP280_CONFIG_REG, 0xA0);  //  standby???=1000ms??????????=4
    
    return 1;
}

/**
 * @brief  ???BMP280??????
 * @param  ??
 * @retval ????????1-?????0-??????
 */
uint8_t BMP280_Check(void)
{
    uint8_t whoAmI;
    
    /* ???WHO_AM_I????? */
    whoAmI = I2C2_ReadByte(BMP280_ADDR, BMP280_WHO_AM_I_REG);
    
    /* ???WHO_AM_I? */
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
 * @brief  ???BMP280§µ?????
 * @param  ??
 * @retval ??
 */
void BMP280_ReadCalibData(void)
{
    /* ??????§µ????? */
    g_calibData.dig_T1 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_T1_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_T1_LSB_REG);
    g_calibData.dig_T2 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_T2_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_T2_LSB_REG);
    g_calibData.dig_T3 = (I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_T3_MSB_REG) << 8) | I2C2_ReadByte(BMP280_ADDR, BMP280_DIG_T3_LSB_REG);
    
    /* ??????§µ????? */
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
 * @brief  ???BMP280??????
 * @param  data: ????????????
 * @retval ??
 */
void BMP280_ReadRawData(BMP280_RawData_t *data)
{
    uint8_t buffer[6];
    
    /* ??????????????? */
    I2C2_ReadBytes(BMP280_ADDR, BMP280_PRESS_MSB_REG, buffer, 6);
    
    /* ??????????? */
    data->press = ((uint32_t)buffer[0] << 12) | ((uint32_t)buffer[1] << 4) | ((uint32_t)buffer[2] >> 4);
    
    /* ??????????? */
    data->temp = ((uint32_t)buffer[3] << 12) | ((uint32_t)buffer[4] << 4) | ((uint32_t)buffer[5] >> 4);
}

/**
 * @brief  ???????
 * @param  adc_temp: ??????
 * @retval ???? (??C)
 */
static float BMP280_CalculateTemperature(uint32_t adc_temp)
{
    int32_t var1, var2, T;
    
    /* ??????? */
    var1 = ((((adc_temp >> 3) - ((int32_t)g_calibData.dig_T1 << 1)) * ((int32_t)g_calibData.dig_T2)) >> 11);
    var2 = (((((adc_temp >> 4) - ((int32_t)g_calibData.dig_T1)) * ((adc_temp >> 4) - ((int32_t)g_calibData.dig_T1))) >> 12) * ((int32_t)g_calibData.dig_T3) >> 14);
    g_t_fine = var1 + var2;
    T = (g_t_fine * 5 + 128) >> 8;
    
    return (float)T / 100.0f + BMP280_TEMPERATURE_OFFSET;
}

/**
 * @brief  ???????
 * @param  adc_press: ??????
 * @retval ???? (hPa)
 */
static float BMP280_CalculatePressure(uint32_t adc_press)
{
    int64_t var1, var2, p;
    
    /* ??????? */
    var1 = ((int64_t)g_t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)g_calibData.dig_P6;
    var2 = var2 + ((var1 * (int64_t)g_calibData.dig_P5) << 17);
    var2 = var2 + (((int64_t)g_calibData.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)g_calibData.dig_P3) >> 8) + ((var1 * (int64_t)g_calibData.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)g_calibData.dig_P1) >> 33;
    
    if (var1 == 0)
    {
        return 0;  // ??????????
    }
    
    p = 1048576 - adc_press;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)g_calibData.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)g_calibData.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)g_calibData.dig_P7) << 4);
    
    return (float)p / 256.0f / 100.0f;  // ????hPa
}

/**
 * @brief  ?????¦È??
 * @param  pressure: ???? (hPa)
 * @retval ???¦È?? (m)
 */
static float BMP280_CalculateAltitude(float pressure)
{
    float altitude;
    
    /* ???????????¦È?? */
    altitude = 44330.0f * (1.0f - pow(pressure * 100.0f / BMP280_PRESSURE_SEA_LEVEL, 1.0f / 5.255f));
    
    return altitude;
}

/**
 * @brief  ???BMP280???????????
 * @param  data: ?????????????????
 * @retval ??
 */
void BMP280_ReadData(BMP280_Data_t *data)
{
    BMP280_RawData_t rawData;
    
    /* ????????? */
    BMP280_ReadRawData(&rawData);
    
    /* ??????? */
    data->temp = BMP280_CalculateTemperature(rawData.temp);
    
    /* ??????? */
    data->press = BMP280_CalculatePressure(rawData.press);
    
    /* ?????¦È?? */
    data->altitude = BMP280_CalculateAltitude(data->press);
}
