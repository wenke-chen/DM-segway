#ifndef __XBOX_TASK_H
#define __XBOX_TASK_H

#include "main.h"

typedef struct
{
    uint8_t btnY;
    uint8_t btnX;
    uint8_t btnB;
    uint8_t btnA;
    uint8_t btnLB;
    uint8_t btnRB;
    uint8_t btnSelect;
    uint8_t btnStart;
    uint8_t btnXbox;
    uint8_t btnShare;
    uint8_t btnLS;
    uint8_t btnRS;
    uint8_t btnDirUp;
    uint8_t btnDirRight;
    uint8_t btnDirDown;
    uint8_t btnDirLeft;
    uint16_t joyLHori;
    uint16_t joyLVert;
    uint16_t joyRHori;
    uint16_t joyRVert;
    uint16_t trigLT;
    uint16_t trigRT;
} XboxControllerData_t;

extern XboxControllerData_t xbox_controller_data;

extern void Xbox_Init(void);
extern void Xbox_task(void);

#endif // XBOX_TASK_H


