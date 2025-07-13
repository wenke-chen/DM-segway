#ifndef VISION_UART_H
#define VISION_UART_H
#include "main.h"


#define SEND_DATA_CHECK   1          //Send data check flag bits //��������У���־λ
#define READ_DATA_CHECK   0          //Receive data to check flag bits //��������У���־λ
#define FRAME_HEADER      0X7B       //Frame head //֡ͷ
#define FRAME_TAIL        0X7D       //Frame tail //֡β
#define RECEIVE_DATA_SIZE 256         //The length of the data sent by the lower computer //��λ�����͹��������ݵĳ���
#define SEND_DATA_SIZE    100

//��λ����ROS�������ݵĽṹ��
typedef struct
{
  uint8_t tx[SEND_DATA_SIZE];  
	
}send_data_t;

//ROS����λ���������ݵĽṹ��
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


