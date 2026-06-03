#include "stm32f4xx.h"                  // Device header

#include "PWM.h"

#include "Serial.h"
void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_IntstStrure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	GPIO_IntstStrure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_IntstStrure.GPIO_OType = GPIO_OType_PP; 
	GPIO_IntstStrure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_IntstStrure.GPIO_Pin =GPIO_Pin_13 | GPIO_Pin_1|GPIO_Pin_2 | GPIO_Pin_3;//½«0¸ÄÎª13
	GPIO_IntstStrure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_IntstStrure);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_IntstStrure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_IntstStrure.GPIO_OType = GPIO_OType_PP; 
	GPIO_IntstStrure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_IntstStrure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_3;
	GPIO_IntstStrure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_IntstStrure);

    PWM_Init();
//	Serial_Init();

	
}

void Motor_SetSpeedA(int16_t Speed)
{
	Speed *= 200;
  if (Speed>=0)
  {
   GPIO_SetBits(GPIOE,GPIO_Pin_1);
   GPIO_ResetBits(GPIOE,GPIO_Pin_0);
   PWM_SetCompare2(Speed);
   

  }
  else
  {
   GPIO_SetBits(GPIOE,GPIO_Pin_0);
   GPIO_ResetBits(GPIOE,GPIO_Pin_1);
   PWM_SetCompare2(-Speed);
	  

  
  }

}

void Motor_SetSpeedB(int16_t Speed)
{
	Speed *= 200;
  if (Speed>=0)
  {
   GPIO_SetBits(GPIOE,GPIO_Pin_3);
   GPIO_ResetBits(GPIOE,GPIO_Pin_2);
   PWM_SetCompare1(Speed);
  }
  else
  {
   GPIO_SetBits(GPIOE,GPIO_Pin_2);
   GPIO_ResetBits(GPIOE,GPIO_Pin_3);
   PWM_SetCompare1(-Speed);
  
  }

}

void Motor_SetSpeedC(int16_t Speed)
{
	Speed *= 200;
  if (Speed>=0)
  {
   GPIO_SetBits(GPIOC,GPIO_Pin_1);
   GPIO_ResetBits(GPIOC,GPIO_Pin_3);
   PWM_SetCompare4(Speed);
  }
  else
  {
   GPIO_SetBits(GPIOC,GPIO_Pin_3);
   GPIO_ResetBits(GPIOC,GPIO_Pin_1);
   PWM_SetCompare4(-Speed);
  
  }

}

void Motor_SetSpeedD(int16_t Speed)
{
	Speed *= 200;
  if (Speed>=0)
  {
   GPIO_SetBits(GPIOC,GPIO_Pin_2);
   GPIO_ResetBits(GPIOC,GPIO_Pin_13);
   PWM_SetCompare3(Speed);
  }
  else
  {
   GPIO_SetBits(GPIOC,GPIO_Pin_13);
   GPIO_ResetBits(GPIOC,GPIO_Pin_2);
   PWM_SetCompare3(-Speed);
  
  }

}
