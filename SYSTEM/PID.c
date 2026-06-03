#include "stm32f4xx.h"                  // Device header
typedef struct{
// 参数
float Kp, Ki, Kd;

// 状态
float integral;
float prev_error;

// 限幅
float output_max;
float output_min;
float integral_max;

// 滤波
float alpha; // 微分滤波系数（0-1）
float filtered_derivative;
} PID_Handle;

/**
?* 初始化 PID
?*/
void PID_Init(PID_Handle *pid, float Kp, float Ki, float Kd,float output_max, float output_min) {
pid->Kp = Kp;
pid->Ki = Ki;
pid->Kd = Kd;
pid->output_max = output_max;
pid->output_min = output_min;
pid->integral_max = output_max / Ki; // 积分限幅
pid->integral =0;
pid->prev_error =0;
pid->alpha =0.1f; // 微分滤波
pid->filtered_derivative =0;
}

/**
?* PID 更新（带抗饱和和微分滤波）
?*/
float PID_Update(PID_Handle *pid, float error) {
// 比例项
float p = pid->Kp * error;

// 积分项
pid->integral += error;

// 积分限幅
if (pid->integral > pid->integral_max) {
pid->integral = pid->integral_max;
 }
if (pid->integral < -pid->integral_max) {
 pid->integral = -pid->integral_max;
}

float i = pid->Ki * pid->integral;

// 微分项（带滤波）
float derivative = (error - pid->prev_error);

// 一阶低通滤波
pid->filtered_derivative = pid->alpha * derivative +(1.0f- pid->alpha) * pid->filtered_derivative;

float d = pid->Kd * pid->filtered_derivative;

// 总输出
float output = p + i + d;

// 输出限幅
if (output > pid->output_max) {
 output = pid->output_max;
// 反计算抗饱和
 pid->integral -= (output - p - d) / pid->Ki;
}
if (output < pid->output_min) {
output = pid->output_min;
// 反计算抗饱和
pid->integral -= (output - p - d) / pid->Ki;
 }

// 保存状态
 pid->prev_error = error;

return output;
}

/**
?* 重置 PID 状态（用于模式切换）
?*/
void PID_Reset(PID_Handle *pid){
pid->integral =0;
 pid->prev_error =0;
pid->filtered_derivative =0;
}
