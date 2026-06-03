#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "Motor.h"
#include "Key.h"
#include "Serial.h"
#include "Encoder.h"
#include <string.h>
#include <stdlib.h>
#include "PID.h"
#include "Timer.h"
/**/
uint8_t KeyGetNum ;
/*测试git提交*/
float SpeedA ,ErrorA,outA,A;
float SpeedB ,ErrorB,outB,B;
float SpeedC ,ErrorC,outC,C;
float SpeedD ,ErrorD,outD,D;

int16_t Num1,Num2,Num3,Num4;
uint16_t n,motor_run,motor_unlock;

PID_Handle motorA_pid;
PID_Handle motorB_pid;
PID_Handle motorC_pid;
PID_Handle motorD_pid;

int main(void)
{   
	delay_init(168);//���ڿ�ͷ����������
	Timer6_Init();
	Encoder_Init();
	Motor_Init();
	Key_Init();
	Serial_Init();//��ʼ��USART
	
	PID_Init(&motorA_pid,0.5,0.01,0,100,20);
	PID_Init(&motorB_pid,0.5,0,0.3,100,20);
	PID_Init(&motorC_pid,0.5,0,0.3,100,20);
	PID_Init(&motorD_pid,0.5,0,0.3,100,20);
	
	
while(1)
 { 
	
	 
	KeyGetNum = Key_GetNum();
	 
	if(KeyGetNum == 1)
	{
        motor_run = 1;
		
	}
	else
	{
		motor_run = 0;
	}
	if(KeyGetNum == 2)
	{
		motor_unlock = 1;
	}
	else
	{
		motor_unlock = 0;
	}
	//���ܷ��жϼ��㣬Ƶ������ᵼ�´�ӡ������,���в��ܷ��ж�����ڣ���Ȼ��ֻ����һ��
	SpeedA = (float)Num1 * 100  / 4.0f / 30.0f / 13.0f * 0.20f;//1s��������/���ٱ�/�������/�ı�Ƶ*�����ܳ� 1 r/s
	SpeedB = (float)Num2 * 100  / 4.0f / 30.0f / 13.0f * 0.20f;//1s��������/���ٱ�/�������/�ı�Ƶ*�����ܳ� 1 r/s
	SpeedC = (float)Num3 * 100  / 4.0f / 30.0f / 13.0f * 0.20f;//1s��������/���ٱ�/�������/�ı�Ƶ*�����ܳ� 1 r/s
	SpeedD = (float)Num4 * 100  / 4.0f / 30.0f / 13.0f * 0.20f;//1s��������/���ٱ�/�������/�ı�Ƶ*�����ܳ� 1 r/s
	
	ErrorA = (0.1 - SpeedA);
	ErrorB = (0.1 - SpeedB);
	ErrorC = (0.1 - SpeedC);
	ErrorD = (0.1 - SpeedD);
	
	A = PID_Update(&motorA_pid, ErrorA);
	B = PID_Update(&motorB_pid, ErrorB);
	C = PID_Update(&motorC_pid, ErrorC);
	D = PID_Update(&motorD_pid, ErrorD);
	
	outA = A * 200;
	outB = B * 200;
	outC = C * 200;
	outD = D * 200;
			
    outA = outA > 100 ? 100 : outA;
	outB = outB > 100 ? 100 : outB;
	outC = outC > 100 ? 100 : outC;
	outD = outD > 100 ? 100 : outD;
//			
	if(motor_run == 1)
	{
    
	    Motor_SetSpeedA((uint8_t)outA);
//		Motor_SetSpeedB((uint8_t)outB);
//		Motor_SetSpeedC((uint8_t)outC);//(uint8_t)outC
//		Motor_SetSpeedD((uint8_t)outD);
//		Motor_SetSpeedD(50);
		
	
	}
	// ��ӡ������Ϣ��ÿ100msһ�Σ�����ˢ����
//    Serial_Printf("  PID_OutA:%.2f\r\n Num1:%d\r\n SpeedA:%.4f\r\n ", 
//                        outA,Num1,SpeedA);//Num1:%d\r\n SpeedA:%.4f\r\nNum1,SpeedA,
//	Serial_Printf("  PID_OutB:%.2f\r\n ", //Num1,SpeedA,Num1:%d\r\n SpeedA:%.4f\r\nNum2,SpeedB
//                        outB);
//	Serial_Printf("  PID_OutC:%.2f\r\nNum3:%d\r\n SpeedC:%.4f\r\n",// 
//                        outC,Num3,SpeedC);
//	Serial_Printf("  PID_OutD:%.2f\r\nNum4:%d\r\n SpeedD:%.4f\r\n", //Num1:%d\r\n SpeedA:%.4f\r\nNum1,SpeedA,
//                        outD,Num4,SpeedD);

//    delay_ms(20);
 } 
       


 
}
 //TIM6�жϷ�����������ʵ�֣������жϴ���������Ӳ����1ms
void TIM6_DAC_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
		// ��������жϱ�־λ���������������ж�
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		n++;
			
		if(n>=10)
		{
			Num1 = -Encoder_Get(1) ;
			Num2 = -Encoder_Get(2) ;
			Num3 = -Encoder_Get(3) ;
			Num4 = (int)(Encoder_Get(4)-25) ;
			n=0;
		}
		
		//		
		
		if( motor_unlock == 1)
		{
			
			Motor_SetSpeedA(0);
			Motor_SetSpeedB(0);
			Motor_SetSpeedC(0);
			Motor_SetSpeedD(0);
			
		}
        
		
	}
}
		


