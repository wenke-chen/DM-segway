#ifndef __LCD_TASK_H
#define __LCD_TASK_H

#include "main.h"

// ���尴��ö������
typedef enum {
    NONE,   // ��
    LEFT,   // ��
    RIGHT,  // ��
    UP,     // ��
    DOWN,   // ��
    CENTER  // �м�
} Key_Position;

extern void lcd_Init(void);
extern void lcd_task(void);

#endif


