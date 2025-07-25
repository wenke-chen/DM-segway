#ifndef __LCD_TASK_H
#define __LCD_TASK_H

#include "main.h"

// 定义按键枚举类型
typedef enum {
    NONE,   // 无
    LEFT,   // 左
    RIGHT,  // 右
    UP,     // 上
    DOWN,   // 下
    CENTER  // 中间
} Key_Position;

extern void lcd_Init(void);
extern void lcd_task(void);

#endif


