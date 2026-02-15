/*
 * PWM.c
 *
 * PWM驱动实现
 * 用于STM32F411CEU6的电机控制
 *
 * 2026-02-15
 */

#include "PWM.h"

/* 内部函数 */
static void PWM_GPIO_Config(void);
static void PWM_TIM2_Config(void);
static void PWM_TIM4_Config(void);

/**
 * @brief  PWM初始化
 * @param  无
 * @retval 无
 */
void PWM_Init(void)
{
    /* 初始化GPIO */
    PWM_GPIO_Config();
    
    /* 初始化定时器 */
    PWM_TIM2_Config();
    PWM_TIM4_Config();
    
    /* 启动PWM */
    PWM_Start();
}

/**
 * @brief  GPIO配置
 * @param  无
 * @retval 无
 */
static void PWM_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 使能GPIO时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
    
    /* 配置PA5 (TIM2_CH1) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);
    
    /* 配置PB6 (TIM4_CH1) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
    
    /* 配置PB7 (TIM4_CH2) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
    
    /* 配置PB10 (TIM2_CH3) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);
}

/**
 * @brief  TIM2配置
 * @param  无
 * @retval 无
 */
static void PWM_TIM2_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    /* 使能TIM2时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    /* 计算预分频值 */
    uint32_t prescaler = (SystemCoreClock / 2) / (PWM_FREQUENCY * PWM_RESOLUTION) - 1;
    
    /* 配置TIM2基本参数 */
    TIM_TimeBaseStructure.TIM_Period = PWM_RESOLUTION - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    /* 配置TIM2_CH1 (MOTOR4) */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    /* 配置TIM2_CH3 (MOTOR3) */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    /* 使能TIM2自动重载 */
    TIM_ARRPreloadConfig(TIM2, ENABLE);
}

/**
 * @brief  TIM4配置
 * @param  无
 * @retval 无
 */
static void PWM_TIM4_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    /* 使能TIM4时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    /* 计算预分频值 */
    uint32_t prescaler = (SystemCoreClock / 2) / (PWM_FREQUENCY * PWM_RESOLUTION) - 1;
    
    /* 配置TIM4基本参数 */
    TIM_TimeBaseStructure.TIM_Period = PWM_RESOLUTION - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    
    /* 配置TIM4_CH1 (MOTOR2) */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    
    /* 配置TIM4_CH2 (MOTOR1) */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    
    /* 使能TIM4自动重载 */
    TIM_ARRPreloadConfig(TIM4, ENABLE);
}

/**
 * @brief  设置电机PWM占空比
 * @param  motor: 电机编号 (0-3)
 * @param  duty: 占空比 (0-1000)
 * @retval 无
 */
void PWM_SetDuty(uint8_t motor, uint16_t duty)
{
    /* 限制占空比范围 */
    if (duty < PWM_MIN_DUTY)
    {
        duty = PWM_MIN_DUTY;
    }
    else if (duty > PWM_MAX_DUTY)
    {
        duty = PWM_MAX_DUTY;
    }
    
    /* 设置对应通道的占空比 */
    switch (motor)
    {
        case MOTOR1:  // TIM4_CH2 (PB7)
            TIM_SetCompare2(TIM4, duty);
            break;
        case MOTOR2:  // TIM4_CH1 (PB6)
            TIM_SetCompare1(TIM4, duty);
            break;
        case MOTOR3:  // TIM2_CH3 (PB10)
            TIM_SetCompare3(TIM2, duty);
            break;
        case MOTOR4:  // TIM2_CH1 (PA5)
            TIM_SetCompare1(TIM2, duty);
            break;
        default:
            break;
    }
}

/**
 * @brief  启动PWM输出
 * @param  无
 * @retval 无
 */
void PWM_Start(void)
{
    /* 启动TIM2和TIM4 */
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}

/**
 * @brief  停止PWM输出
 * @param  无
 * @retval 无
 */
void PWM_Stop(void)
{
    /* 停止TIM2和TIM4 */
    TIM_Cmd(TIM2, DISABLE);
    TIM_Cmd(TIM4, DISABLE);
}
