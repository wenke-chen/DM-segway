/**
  *********************************************************************
  * @file      chassisR_task.c/h
  * @brief     该任务控制底盘两个的电机，两个都是DM3510、，这两个电机挂载在can1总线上
	*						 从底盘上往下看，开发板电源输入口朝右，左边电机发送id为0x01、接收id为0x11，
	*            右边电机发送id为0x02、接收id为0x12
  * @note       
  * @history
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  *********************************************************************
  */
	
#include "chassisR_task.h"
#include "fdcan.h"
#include "cmsis_os.h"
#include <math.h>
#include "usart.h"
#include "main.h"
#include "cmsis_os.h"
#include "xbox_task.h"

extern INS_t INS;
																
chassis_t chassis_move;

uint32_t CHASSR_TIME=1;	
 
typedef struct
{
    float kp;
    float ki;
    float kd;
    float integral;
    float prev_err;
} PID_t;

float out_ang,out_x,out_vel,out_gyr;
static PID_t pid_ang = { .kp = 100.668f, .ki = 0.0f, .kd = 0.0469f, .integral = 0, .prev_err = 0 };  // 俯仰角误差
static PID_t pid_x = { .kp = 2.3f, .ki = 0.0f, .kd = 0.01f, .integral = 0, .prev_err = 0 };  // x误差
static PID_t pid_vel = { .kp = 13.3f, .ki = 0.0f, .kd = 0.03f, .integral = 0, .prev_err = 0 };  // v误差
static PID_t pid_gyr = { .kp = 2.505f, .ki = 0.0f, .kd = 0.0f, .integral = 0, .prev_err = 0 };  // 转向角误差
 
/* ---------- PID 计算函数 ---------- */
float pid_ang_update(PID_t *pid, float err, float dt)
{	
    float out;
    pid->integral += err * dt;
    float derivative = (err - pid->prev_err) / dt;
    out = pid->kp * err + pid->ki * pid->integral + pid->kd * derivative;
    pid->prev_err  = err;
    return out;
}

float pid_x_update(PID_t *pid, float err, float dt)
{	
    float out;
    pid->integral += err * dt;
    float derivative = (err - pid->prev_err) / dt;
    out = pid->kp * err + pid->ki * pid->integral + pid->kd * derivative;
    pid->prev_err  = err;
    return out;
}

float pid_vel_update(PID_t *pid, float err, float dt)
{	
    float out;
    pid->integral += err * dt;
    float derivative = (err - pid->prev_err) / dt;
    out = pid->kp * err + pid->ki * pid->integral + pid->kd * derivative;
    pid->prev_err  = err;
    return out;
}
 
float pid_gyr_update(PID_t *pid, float err, float dt)
{			
    float out;
    pid->integral += err * dt;
    float derivative = (err - pid->prev_err) / dt;
    out = pid->kp * err + pid->ki * pid->integral + pid->kd * derivative;
    pid->prev_err  = err;
    return out;
}
 
 
void ChassisR_task(void)
{
	while(INS.ins_flag==0)
	{//等待加速度收敛
	  osDelay(1);	
	}

  ChassisR_init(&chassis_move);

	while(1)
	{	
		chassisR_feedback_update(&chassis_move,&INS);//更新数据
		
		out_ang = pid_ang_update(&pid_ang, 0.03f-chassis_move.myPithR, 0.002);
		out_x = pid_x_update(&pid_x, chassis_move.x_set-chassis_move.x, 0.002);
		out_vel = pid_vel_update(&pid_vel, chassis_move.v_set-chassis_move.v, 0.002);
		
		chassis_move.wheel_motor[0].wheel_T = out_ang - out_x + out_vel;
		
		out_gyr = pid_gyr_update(&pid_gyr, chassis_move.turn_set-INS.Gyro[2], 0.002);
		
		chassis_move.wheel_motor[1].wheel_T=-chassis_move.wheel_motor[0].wheel_T-out_gyr;
		
		chassis_move.wheel_motor[0].wheel_T=chassis_move.wheel_motor[0].wheel_T-out_gyr;
		
		scaled_down(&chassis_move.wheel_motor[0].wheel_T, &chassis_move.wheel_motor[1].wheel_T,-50.0f,50.0f);
			
		if(chassis_move.start_flag==1)	
		{
			speed_ctrl(&hfdcan1,0x01,chassis_move.wheel_motor[0].wheel_T);
			osDelay(CHASSR_TIME);
			speed_ctrl(&hfdcan1,0x02,chassis_move.wheel_motor[1].wheel_T);
			osDelay(CHASSR_TIME);
		}
		else if(chassis_move.start_flag==0)	
		{
			speed_ctrl(&hfdcan1,0x01,0);
			osDelay(CHASSR_TIME);
			speed_ctrl(&hfdcan1,0x02,0);
			osDelay(CHASSR_TIME);
		}
	}
}

void ChassisR_init(chassis_t *chassis)
{
	wheel_motor_init(&chassis->wheel_motor[0],0x01,MIT_MODE);//发送id为1

	wheel_motor_init(&chassis->wheel_motor[1],0x02,MIT_MODE);//发送id为2
	
	for(int j=0;j<10;j++)
	{//左边
    enable_motor_mode(&hfdcan1,chassis->wheel_motor[0].para.id,chassis->wheel_motor[0].mode);//右边轮毂电机
	  osDelay(1);
	}
	for(int j=0;j<10;j++)
	{
    enable_motor_mode(&hfdcan1,chassis->wheel_motor[1].para.id,chassis->wheel_motor[1].mode);//右边轮毂电机
	  osDelay(1);
	}	
}

int64_t time=0;
void chassisR_feedback_update(chassis_t *chassis,INS_t *ins)
{
	chassis->myPithR=ins->Pitch;
	chassis->myPithGyroR=ins->Gyro[1];
	
	chassis->total_yaw=ins->YawTotalAngle;
	chassis->v=((chassis->wheel_motor[1].para.vel-chassis->wheel_motor[0].para.vel)/2.0f)*0.03375f;//0.03375m是轮子半径
	chassis->x=chassis->x+chassis->v*0.001f;//控制周期为1ms
	
	if(chassis_move.wheel_motor[0].para.vel>180.0f||chassis_move.wheel_motor[0].para.vel<-180.0f
		||chassis_move.wheel_motor[1].para.vel>180.0f||chassis_move.wheel_motor[1].para.vel<-180.0f)
	{//速度过大，应该是被抬起来了
	  
	  time++;
		if(time>1500)
		{
		  time=0;
			chassis->start_flag=0;
		  chassis->turn_flag=0;
		}		
	}
	else
	{
	  time=0;
	}

}

void mySaturate(float *in,float min,float max)
{
  if(*in < min)
  {
    *in = min;
  }
  else if(*in > max)
  {
    *in = max;
  }
}

void scaled_down(float *in1,float *in2,float min,float max)
{
	for(int j=0;j<1;j++)//执行两次
	{
		if(*in1 < min || *in2 < min)
		{
			if(*in1 < *in2)
			{
				*in2 = *in2 * (min / (*in1));
				*in1 = min;
			}
			else
			{
				*in1 = *in1 * (min / (*in2));
				*in2 = min;
			}
		}
		else if(*in1 > max || *in2 > max)
		{
			if(*in1 < *in2)
			{
				*in1 = *in1 * (max / (*in2));
				*in2 = max;
			}
			else
			{
				*in2 = *in2 * (max / (*in1));
				*in1 = max;
			}
		}
  }
}



