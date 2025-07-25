#ifndef VISION_UART_H
#define VISION_UART_H
#include "main.h"


#define SEND_DATA_CHECK   1          //Send data check flag bits //发送数据校验标志位
#define READ_DATA_CHECK   0          //Receive data to check flag bits //接收数据校验标志位
#define FRAME_HEADER      0X7B       //Frame head //帧头
#define FRAME_TAIL        0X7D       //Frame tail //帧尾
#define RECEIVE_DATA_SIZE 256         //The length of the data sent by the lower computer //下位机发送过来的数据的长度
#define SEND_DATA_SIZE    100

//下位机向ROS发送数据的结构体
typedef struct
{
  uint8_t tx[SEND_DATA_SIZE];  
	
}send_data_t;

//ROS向下位机发送数据的结构体
typedef struct      
{
  uint8_t rx[RECEIVE_DATA_SIZE];
	
}rev_data_t;


#define ESP32_RX_BUFFER_SIZE 100
extern uint8_t esp32_rx_buffer[ESP32_RX_BUFFER_SIZE];
extern uint16_t esp32_rx_len;
extern volatile uint8_t esp32_rx_flag;

extern void connect_usart1_init(void);

extern uint8_t Check_Sum(uint8_t Count_Number,uint8_t *buffer);

#endif


