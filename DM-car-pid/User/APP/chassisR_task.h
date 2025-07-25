#ifndef __CHASSISR_TASK_H
#define __CHASSISR_TASK_H

#include "main.h"
#include "dm4310_drv.h"
#include "pid.h"

#include "INS_task.h"

typedef struct
{

  Wheel_Motor_t wheel_motor[2];
	
	float v_set;//�����ٶȣ���λ��m/s
	float x_set;//����λ�ã���λ��m
  float v;//ʵ�ʵ��ٶ�,��λ��m/s
	float x;//ʵ�ʵ�λ�ƣ���λ��m
	
	
	float phi_set;
  float d_phi_set;
		
	
	float turn_set;//����yaw�ỡ��

	float v_filter;//�˲���ĳ����ٶȣ���λ��m/s
	float x_filter;//�˲���ĳ���λ�ã���λ��m
	
	float myPithR;
	float myPithGyroR;

	float total_yaw;
	
	uint8_t start_flag;//������־
	uint8_t front_flag;
	uint8_t turn_flag;

} chassis_t;


extern void ChassisR_init(chassis_t *chassis);
extern void ChassisR_task(void);
extern void mySaturate(float *in,float min,float max);
extern void chassisR_feedback_update(chassis_t *chassis,INS_t *ins);
extern void scaled_down(float *in1,float *in2,float min,float max);

extern float out_ang,out_x,out_vel,out_gyr;

#endif




