#include "lcd_task.h"
#include "xbox_task.h"
#include "chassisR_task.h"
#include "string.h"
#include "stdlib.h"
#include "usart.h"
#include "cmsis_os.h"
#include "adc.h"
#include "lcd.h"
#include "pic.h"

extern INS_t INS;
extern chassis_t chassis_move;

int start = 0;
uint16_t adc_val[1];
Key_Position key_position = NONE;

const char* positionToString(Key_Position pos);
Key_Position adc_key(uint16_t adc_val);

void lcd_Init(void)
{
  // 开启LCD背光
	LCD_Init();//LCD初始化
	LCD_Fill(0,0,LCD_W, LCD_H,BLACK);	
	
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_val,1);	// 读取ADC按键键值
}

// 竖屏,宽240*高280
void lcd_task(void)
{
		if(start == 0)//初始化
		{
			start =1;
			lcd_Init();
		}
	  // 遥控状态显示
		if(chassis_move.front_flag==1)
		{
			if(chassis_move.v_set>0.01f)
				LCD_ShowString(84, 0, (uint8_t *)"front", WHITE, BLACK, 32, 0);
			else if(chassis_move.v_set<-0.01f)
				LCD_ShowString(84,0, (uint8_t *)" back", WHITE, BLACK, 32, 0);
			else
				LCD_ShowString(84, 0, (uint8_t *)"pause", WHITE, BLACK, 32, 0);
		}
		else if(chassis_move.turn_flag==1)
		{
			if(INS.Gyro[2]>0.3f)
			  LCD_ShowString(84, 0,(uint8_t *)" left", WHITE, BLACK, 32, 0);
			else if(INS.Gyro[2]<-0.3f)
				LCD_ShowString(84, 0, (uint8_t *)"right", WHITE, BLACK, 32, 0);
			else
				LCD_ShowString(84, 0, (uint8_t *)"pause", WHITE, BLACK, 32, 0);
		}
		else
		{
			if(chassis_move.start_flag==1)
				LCD_ShowString(84, 0, (uint8_t *)"pause", WHITE, BLACK, 32, 0);
			else
				LCD_ShowString(84, 0, (uint8_t *)" stop", WHITE, BLACK, 32, 0);
		}

	  // 传感器数据显示
		LCD_ShowString(10, 40,(uint8_t *)"Pitch: ", BRRED, BLACK, 16, 0);
		LCD_ShowFloatNum(70, 40,INS.Pitch, 3, 3, WHITE, BLACK, 16);
		LCD_ShowString(120, 40,(uint8_t *)", ", BRRED, BLACK, 16, 0);
		LCD_ShowFloatNum(140, 40,chassis_move.myPithR, 3, 3, WHITE, BLACK, 16);

		LCD_ShowString(10, 56,(uint8_t *)"Yaw: ", BRRED, BLACK, 16, 0);		
		LCD_ShowFloatNum(70, 56,chassis_move.total_yaw, 3, 3, WHITE, BLACK, 16);
		LCD_ShowString(120, 56,(uint8_t *)", ", BRRED, BLACK, 16, 0);
		LCD_ShowFloatNum(140, 56,INS.Gyro[2], 3, 3, WHITE, BLACK, 16);
		
		LCD_ShowString(10, 88,(uint8_t *)"v_set: ", BRRED, BLACK, 16, 0);		
		LCD_ShowFloatNum(140, 88, chassis_move.v_set, 3, 3, WHITE, BLACK, 16);
		
		LCD_ShowString(10, 104,(uint8_t *)"turn_set: ", BRRED, BLACK, 16, 0);		
		LCD_ShowFloatNum(140, 104, chassis_move.turn_set, 3, 3, WHITE, BLACK, 16);

		LCD_ShowString(10, 152,(uint8_t *)"vel_ratio: ", BRRED, BLACK, 16, 0);		
		LCD_ShowFloatNum(140, 152, vel_ratio, 4, 4, WHITE, BLACK, 16);		
		
		LCD_ShowString(40, 216,(uint8_t *)"out_ang: ", BRRED, BLACK, 16, 0);		
		LCD_ShowFloatNum(120, 216, out_ang, 4, 4, WHITE, BLACK, 16);
		
		LCD_ShowString(40, 232,(uint8_t *)"out_vel: ", BRRED, BLACK, 16, 0);		
		LCD_ShowFloatNum(120, 232, out_vel, 4, 4, WHITE, BLACK, 16);
		LCD_ShowString(40, 248,(uint8_t *)"out_gyr: ", BRRED, BLACK, 16, 0);		
		LCD_ShowFloatNum(120, 248, out_gyr, 4, 4, WHITE, BLACK, 16);
	

//		LCD_ShowPicture(100, 180, 40, 40, gImage_smile);//开心
//		LCD_ShowPicture(100, 180, 40, 40, gImage_hushed);//惊讶
//		LCD_ShowPicture(100, 180, 40, 40, gImage_smiling);//微笑
//		LCD_ShowPicture(100, 180, 40, 40, gImage_squinting);//喜极而泣
//		LCD_ShowPicture(100, 180, 40, 40, gImage_sweat);//尴尬
//		LCD_ShowPicture(100, 180, 40, 40, gImage_tears);//哭笑不得
	  osDelay(1);
}

Key_Position adc_key(uint16_t adc_val)
{
  if(adc_val<2000)
		key_position=CENTER;
	else if(adc_val>1999 && adc_val<15000)
		key_position=LEFT;
	else if(adc_val>14999 && adc_val<28000)
		key_position=RIGHT;
	else if(adc_val>27999 && adc_val<40000)
		key_position=UP;
	else if(adc_val>39999 && adc_val<53000)
		key_position=DOWN;
	else if(adc_val>52999 && adc_val<65535)
		key_position=NONE;
	else
		key_position=NONE;
	
	return key_position;
}

// 定义一个函数，用于将枚举值转换为对应的字符串
const char* positionToString(Key_Position pos) 
{
	switch (pos)
		{
        case NONE:   return "NONE    ";
        case LEFT:   return "LEFT    ";
        case RIGHT:  return "RIGHT   ";
        case UP:     return "UP      ";
        case DOWN:   return "DOWN    ";
        case CENTER: return "CENTER  ";
        default:     return "UNKNOWED";
    }
		
}

