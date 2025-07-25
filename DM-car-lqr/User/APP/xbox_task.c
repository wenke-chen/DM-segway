#include "xbox_task.h"
#include "bsp_usart1.h"
#include "chassisR_task.h"
#include "string.h"
#include "stdlib.h"
#include "usart.h"
#include "cmsis_os.h"

XboxControllerData_t xbox_controller_data;

extern chassis_t chassis_move;

int start_flag = 0;
int pause_flag = 0;
float vel_ratio=0.2f;

void Xbox_Init(void)
{
    memset(&xbox_controller_data, 0, sizeof(XboxControllerData_t));
}

void Xbox_task(void)
{
    if (esp32_rx_flag)
    {
        // 清除接收完成标志
        esp32_rx_flag = 0;

        // 确保数据结尾有换行符
        if (esp32_rx_len > 0 && esp32_rx_buffer[esp32_rx_len - 1] == '\n')
        {
            // 复制数据到临时缓冲区，并添加字符串结束符
            char temp_buffer[ESP32_RX_BUFFER_SIZE];
            // 排除 '\n'，只复制中间的数据部分
            strncpy(temp_buffer, (char*)esp32_rx_buffer, esp32_rx_len - 1);
            temp_buffer[esp32_rx_len - 1] = '\0';

            char *token;
            char *rest = temp_buffer;
            int data_index = 0;

            // 解析所有22个数据
            while ((token = strtok_r(rest, ",", &rest)) != NULL)
            {
                switch (data_index)
                {
                    case 0: xbox_controller_data.btnY = atoi(token); break;
                    case 1: xbox_controller_data.btnX = atoi(token); break;
                    case 2: xbox_controller_data.btnB = atoi(token); break;
                    case 3: xbox_controller_data.btnA = atoi(token); break;
                    case 4: xbox_controller_data.btnLB = atoi(token); break;
                    case 5: xbox_controller_data.btnRB = atoi(token); break;
                    case 6: xbox_controller_data.btnSelect = atoi(token); break;
                    case 7: xbox_controller_data.btnStart = atoi(token); break;
                    case 8: xbox_controller_data.btnXbox = atoi(token); break;
                    case 9: xbox_controller_data.btnShare = atoi(token); break;
                    case 10: xbox_controller_data.btnLS = atoi(token); break;
                    case 11: xbox_controller_data.btnRS = atoi(token); break;
                    case 12: xbox_controller_data.btnDirUp = atoi(token); break;
                    case 13: xbox_controller_data.btnDirRight = atoi(token); break;
                    case 14: xbox_controller_data.btnDirDown = atoi(token); break;
                    case 15: xbox_controller_data.btnDirLeft = atoi(token); break;
                    case 16: xbox_controller_data.joyLHori = atoi(token); break;
                    case 17: xbox_controller_data.joyLVert = atoi(token); break;
                    case 18: xbox_controller_data.joyRHori = atoi(token); break;
                    case 19: xbox_controller_data.joyRVert = atoi(token); break;
                    case 20: xbox_controller_data.trigLT = atoi(token); break;
                    case 21: xbox_controller_data.trigRT = atoi(token); break;
                    default: break;
                }
                data_index++;
            }

					if(xbox_controller_data.btnY == 1 && start_flag == 0)
					{//启动
						chassis_move.start_flag=1;
						start_flag = 1;
					}
					else if(xbox_controller_data.btnY == 1 && start_flag == 1)
					{//停止
						chassis_move.start_flag=0;
						start_flag = 0;
					}
					
					if(chassis_move.start_flag==1)
					{
						if(xbox_controller_data.btnB == 1 && pause_flag == 0)
						{//暂停
							chassis_move.front_flag=0;
							chassis_move.turn_flag=0;
							chassis_move.v_set=0.0f;
							chassis_move.turn_set=chassis_move.total_yaw;
						}
						else if(xbox_controller_data.btnDirUp == 1)
						{//前进
							chassis_move.front_flag=1;
							chassis_move.v_set=5.0f*vel_ratio;
							chassis_move.x_set=chassis_move.x_set+chassis_move.v_set*0.02f;				
						}
						else if(xbox_controller_data.btnDirDown == 1)
						{//后退
							chassis_move.front_flag=1;
							chassis_move.v_set=-5.0f*vel_ratio;
							chassis_move.x_set=chassis_move.x_set+chassis_move.v_set*0.02f;				
						}
						else if(xbox_controller_data.btnDirLeft == 1)
						{//左转
							chassis_move.turn_flag=1;
							chassis_move.turn_set=10.0f*vel_ratio;
						}
						else if(xbox_controller_data.btnDirRight == 1)
						{//右转
							chassis_move.turn_flag=1;
							chassis_move.turn_set=-10.0f*vel_ratio;
						}
						else if(xbox_controller_data.btnLB == 1)
						{
							vel_ratio-=0.05f;
							// 将 vel_ratio 限制在 [0.2, 0.6] 范围内
							vel_ratio=(vel_ratio < 0.2f) ? 0.2f : ((vel_ratio > 0.6f) ? 0.6f : vel_ratio);
						}
						else if(xbox_controller_data.btnRB == 1)
						{
							vel_ratio+=0.05f;
							// 将 vel_ratio 限制在 [0.2, 0.6] 范围内
							vel_ratio=(vel_ratio < 0.2f) ? 0.2f : ((vel_ratio > 0.6f) ? 0.6f : vel_ratio);
						}
						else
						{//暂停
							chassis_move.front_flag=0;
							chassis_move.turn_flag=0;
							chassis_move.v_set=0.0f;
							chassis_move.turn_set=chassis_move.total_yaw;
						}
						
						pause_flag = xbox_controller_data.btnB;
					}	

        }
    }
		osDelay(1);
}


