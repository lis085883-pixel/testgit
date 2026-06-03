#include "stm32f4xx.h"                  // Device header
#include "delay.h"
void Key_Init(void)
{   GPIO_InitTypeDef GPIO_InitStructure;
	
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
}

uint8_t Key_GetNum(void)//获取按键输入
{
     uint8_t KeyNum=0;
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)==0)//按键按下
	{
		
	    delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)==0)//判断是否松手 不松就死循环
	    delay_ms(20);
		KeyNum=1;
		
	}
	
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)==0)//按键按下
	{
		
	    delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)==0)//判断是否松手 不松就死循环
	    delay_ms(20);
		KeyNum=2;
		
	}
	return KeyNum;
}






