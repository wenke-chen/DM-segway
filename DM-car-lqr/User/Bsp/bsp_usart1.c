#include "bsp_usart1.h"
#include "chassisR_task.h"

extern UART_HandleTypeDef huart1;

extern chassis_t chassis_move;
send_data_t send_data;

rev_data_t rev_data;

uint8_t esp32_rx_buffer[ESP32_RX_BUFFER_SIZE];
uint16_t esp32_rx_len = 0;
volatile uint8_t esp32_rx_flag = 0;

void connect_usart1_init(void)
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, esp32_rx_buffer, ESP32_RX_BUFFER_SIZE);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	
    if (huart->Instance == USART1)
    {
			  esp32_rx_len = Size;
        esp32_rx_flag = 1; // 设置接收完成标志
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, esp32_rx_buffer, ESP32_RX_BUFFER_SIZE);
    }
}

/*
void connect_usart1_init(void)
{ 
	 HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rev_data.rx,RECEIVE_DATA_SIZE*2);	
}

float vel_ratio=0.05f;
extern chassis_t chassis_move;

uint8_t rev_data_rx[RECEIVE_DATA_SIZE];
uint8_t rev_data_index = 0;

char tx_data[100];
int len;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{
	if(huart->Instance==USART1)
	{
		if(rev_data.rx[0]=='X')
		{//启动
		  chassis_move.start_flag=1;
		}
		else if(rev_data.rx[0]=='Y')
		{//停止
		  chassis_move.start_flag=0;
		}
		
		if(chassis_move.start_flag==1)
		{
			if(rev_data.rx[0]=='Z')
			{//暂停
				chassis_move.front_flag=0;
				chassis_move.turn_flag=0;
				chassis_move.v_set=0.0f;
				chassis_move.turn_set=chassis_move.total_yaw;
			}
			else if(rev_data.rx[0]=='A')
			{//前进
				chassis_move.front_flag=1;

				//chassis_move.v_set=2.0f;	
				chassis_move.v_set=65.0f*vel_ratio;
				chassis_move.x_set=chassis_move.x_set+chassis_move.v_set*0.02f;				
			}
			else if(rev_data.rx[0]=='E')
			{//后退
				chassis_move.front_flag=1;
				//chassis_move.v_set=-2.0f;
			  chassis_move.v_set=-65.0f*vel_ratio;
				chassis_move.x_set=chassis_move.x_set+chassis_move.v_set*0.02f;				
			}
			else if(rev_data.rx[0]=='G')
			{//左转
				chassis_move.turn_flag=1;
				chassis_move.turn_set=4.5f;
			}
			else if(rev_data.rx[0]=='C')
			{//右转
				chassis_move.turn_flag=1;
				chassis_move.turn_set=-4.5f;
			}
		//	else if(rev_data.rx[0]=='F')
			//{
		//		chassis_move.turn_flag=1;
				//chassis_move.turn_set=-20.0f;
		//	}
		//	else if(rev_data.rx[0]=='D')
		//	{
		//	chassis_move.turn_flag=1;
		//	}
		}	
		HAL_UART_Receive_IT(&huart1,rev_data.rx, 1);//接收
	}
	
}
*/


//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{

//    if (huart->Instance == USART1)
//    {
//        uint8_t received_byte = rev_data.rx[0];

//				rev_data_rx[rev_data_index++] = received_byte;
////				if (received_byte == 1)
////				{
//			  len = sprintf(tx_data, "1: %d\r\n", 3);
//				HAL_UART_Transmit(&huart1, (uint8_t*)tx_data, len, 100);
////				}
//				rev_data_index = 0;
////				if (received_byte == '\n')
////				{
////						rev_data_rx[rev_data_index - 1] = '\0'; 

////						char *token = strtok((char *)rev_data_rx, ",");

////						int data_index = 0;
////						while ((token = strtok(NULL, ",")) != NULL)
////						{

////								switch (data_index)
////								{
////										case 1: 
////													
////												len = sprintf(tx_data, "Pitch: %d\r\n", 2);
////												HAL_UART_Transmit(&huart1, (uint8_t*)tx_data, len, 1000);
////										
////												if (atoi(token) == 1)
////												{
////														chassis_move.front_flag = 1;
////														chassis_move.v_set = 65.0f * vel_ratio;
////														chassis_move.x_set = chassis_move.x_set + chassis_move.v_set * 0.02f;
////												}
////												break;
////										// ??????????
////								}
////								data_index++;
////						}
////						rev_data_index = 0;
////				}

//        HAL_UART_Receive_IT(&huart1,rev_data.rx, 1);//接收
//    }
//}
