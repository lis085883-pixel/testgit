#ifndef __PID_H
#define __PID_H

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

void PID_Init(PID_Handle *pid, float Kp, float Ki, float Kd,float output_max, float output_min);
float PID_Update(PID_Handle *pid, float error);
void PID_Reset(PID_Handle *pid);

#endif
