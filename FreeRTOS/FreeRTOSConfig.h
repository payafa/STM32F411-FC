/*
 * FreeRTOSConfig.h
 *
 * 四轴无人机飞控系统配置文件
 * 主控：STM32F411CEU6
 * 基于FreeRTOS实时操作系统
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#include <stdint.h>

/* 确保使用正确的编译器指令 */
#define configUSE_PREEMPTION                    1
#define configCPU_CLOCK_HZ                      ( ( uint32_t ) 100000000 ) /* STM32F411CEU6最大主频100MHz */
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 )   /* 1ms tick */
#define configMAX_PRIORITIES                    ( 7 )                     /* 7个优先级等级，适合四轴飞控任务 */
#define configMINIMAL_STACK_SIZE                ( ( uint16_t ) 128 )      /* 最小栈大小 */
#define configMAX_TASK_NAME_LEN                 ( 16 )                    /* 任务名最大长度 */
#define configUSE_16_BIT_TICKS                  0                        /* 使用32位tick计数器 */
#define configUSE_MUTEXES                       1                        /* 使用互斥量 */
#define configQUEUE_REGISTRY_SIZE               8                        /* 队列注册表大小 */
#define configUSE_RECURSIVE_MUTEXES             1                        /* 使用递归互斥量 */
#define configUSE_COUNTING_SEMAPHORES           1                        /* 使用计数信号量 */
#define configUSE_ALTERNATIVE_API               0                        /* 不使用替代API */
#define configUSE_APPLICATION_TASK_TAG          0                        /* 不使用应用任务标签 */
#define configUSE_TIME_SLICING                  1                        /* 使用时间片轮转 */
#define configUSE_NEWLIB_REENTRANT              0                        /* 不使用newlib重入 */
#define configENABLE_BACKWARD_COMPATIBILITY     0                        /* 不启用向后兼容 */
#define configUSE_TICKLESS_IDLE                 0                        /* 不使用无tick空闲模式，实时性要求高 */

/* 内存配置 */
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 16 * 1024 ) ) /* 16KB堆大小，适合STM32F411CEU6 */
#define configAPPLICATION_ALLOCATED_HEAP        0                          /* 不使用应用程序分配的堆 */

/* 钩子函数配置 */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configUSE_MALLOC_FAILED_HOOK            0
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* 运行时统计信息 */
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                0
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* 协程配置 */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )

/* 软件定时器配置 */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            ( configMINIMAL_STACK_SIZE * 2 )

/* 中断嵌套配置 */
#define configKERNEL_INTERRUPT_PRIORITY         ( 7 << 4 )    /* 内核中断优先级，最低优先级 */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( 5 << 4 )    /* 系统调用中断优先级，允许FreeRTOS API在中断中使用 */
#define configMAX_API_CALL_INTERRUPT_PRIORITY   ( 5 << 4 )    /* API调用中断优先级 */

/* 任务通知配置 */
#define configUSE_TASK_NOTIFICATIONS            1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES   5

/* 流缓冲区配置 */
#define configUSE_STREAM_BUFFERS                1

/* 消息缓冲区配置 */
#define configUSE_MESSAGE_BUFFERS               1

/* 事件组配置 */
#define configUSE_EVENT_GROUPS                  1

/* 任务优先级相关 */
/* 四轴飞控任务优先级分配建议：
 * 0: 空闲任务（系统默认）
 * 1: 低优先级任务（日志、状态更新等）
 * 2: 中优先级任务（通信任务、参数管理等）
 * 3: 较高优先级任务（传感器数据处理）
 * 4: 高优先级任务（姿态解算）
 * 5: 最高优先级任务（电机控制）
 * 6: 系统任务（定时器任务等）
 */

/* 与STM32相关的配置 */
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_vTaskPrioritySet               1
#define INCLUDE_uxTaskPriorityGet              1
#define INCLUDE_vTaskDelete                    1
#define INCLUDE_vTaskSuspend                   1
#define INCLUDE_xResumeFromISR                 1
#define INCLUDE_vTaskDelayUntil                1
#define INCLUDE_vTaskDelay                     1
#define INCLUDE_xTaskGetIdleTaskHandle         1
#define INCLUDE_xTaskAbortDelay                1
#define INCLUDE_xTaskGetHandle                 1
#define INCLUDE_xTaskGetCurrentTaskHandle      1
#define INCLUDE_pxTaskGetStackStart            1
#define INCLUDE_eTaskGetState                  1
#define INCLUDE_xEventGroupSetBitsFromISR      1
#define INCLUDE_xTimerPendFunctionCall         1
#define INCLUDE_xSemaphoreGetMutexHolder       1

/* 端口特定配置 */
#define xPortPendSVHandler                     PendSV_Handler
#define vPortSVCHandler                        SVC_Handler
#define xPortSysTickHandler                    SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
