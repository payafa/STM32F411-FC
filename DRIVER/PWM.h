/*
 * PWM.h
 *
 * PWM驱动头文件
 * 用于STM32F411CEU6的PWM电机控制
 *
 * 2026-02-15
 */

#ifndef PWM_H
#define PWM_H

#include "stm32f4xx.h"

/* 电机定义 */
#define MOTOR1  0
#define MOTOR2  1
#define MOTOR3  2
#define MOTOR4  3

/* PWM参数 */
#define PWM_FREQUENCY      5000  // PWM频率 5kHz
#define PWM_RESOLUTION     1000  // PWM分辨率 0-1000
#define PWM_MIN_DUTY       50    // 最小占空比
#define PWM_MAX_DUTY       950   // 最大占空比

/* 函数声明 */
void PWM_Init(void);
void PWM_SetDuty(uint8_t motor, uint16_t duty);
void PWM_Start(void);
void PWM_Stop(void);

#endif /* PWM_H */
