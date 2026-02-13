# Flight_Control-STM32F411
这是一个由于购买正点原子MiniFly而受到启发，想要二次开发的仓库。

2026/2/12:
    1. 项目开始，创建工程，移植FreeRTOS，鉴于不明白具体FreeRTOSConfig.h的配置，先用AI生成的，后面有需要再修改
2026/2/12:
    1. 配置Task任务框架，包括radioLinkTask, atkpRxTask, stabilizerTask, atkpTxTask
    2. 配置MPU9250与BMP280的驱动