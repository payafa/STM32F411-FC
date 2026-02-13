#include "stabilizer.h"

/*
	*stabilizerTask函数，是处理分析任务的核心函数，
	*用处：
	*1.使用I2C接收来自MPU9250（地址为0xD2），BMP280（地址为0xEC）的传感器数据
	*2.使用来自传感器的数据来进行姿态解算
	*3.接受atkpRx的对系统状态的调整，进行对应的控制
	*4.返回对应数据包给atkpTxTask
*/

void stabilizerTask(){
	while(1){
		
	}
}


