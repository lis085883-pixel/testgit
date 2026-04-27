#include "stm32f10x.h"                  // Device header

#include "Delay.h"

uint16_t i=0;
uint8_t j=0;
int main(void)
{
	GPIO_InitTypeDef GPIO_Initstructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_Initstructure.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_1 ;
	GPIO_Initstructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	
	
	while(1)
	{
		//GPIO_ResetBits(GPIOA,GPIO_Pin_0);
		//Delay_ms(500);
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
		Delay_ms(1000);
//		
//		
//		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
//		Delay_ms(500);
//		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
//		Delay_ms(500);
//		
//		GPIO_WriteBit(GPIOA,GPIO_Pin_0,(BitAction)0);
//		Delay_ms(500);
//		GPIO_WriteBit(GPIOA,GPIO_Pin_0,(BitAction)1);
//		Delay_ms(500);
		
	}
}