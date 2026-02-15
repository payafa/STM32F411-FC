#include "radioLink.h"

/*
	*无线通信驱动，负责与 NRF51822 无线模块的通信
	*用处：
	*1.从UART2(TX:PA2，RX:PA3)读取从51822发来的数据包
	*2.把读到的数据包发送给atkpRxTask
	*3.从atkpTxTask接受一个数据包
	*4.把读到的来自于atkpTxTask的数据包发送给51822
	
	*数据接收流程：
	*1. 初始化状态机为 waitForStartByte1
	*2. 循环读取 UART 数据
	*3. 根据状态机解析数据包：
  	* - waitForStartByte1: 等待帧头第一个字节 (0xAA)
  	* - waitForStartByte2: 等待帧头第二个字节 (0xAF)
  	* - waitForMsgID: 接收消息 ID
  	* - waitForDataLength: 接收数据长度
  	* - waitForData: 接收数据内容
  	* - waitForChksum1: 验证校验和
	*4. 校验通过后调用 atkpPacketDispatch 处理数据包
	*5. 处理完成后重置状态机
	
	*数据发送流程：
	*1. 数据包被放入发送队列 txQueue
	
	*涉及到:
	*1.NRF51822与STM32F411CEU6的串口通讯
*/

void radioLinkTask(){
	while(1){
		
	}
}



