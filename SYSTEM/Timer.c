#include "stm32f4xx.h"                // Device header

// 定时中断->1ms（STM32F407 TIM5实现，复刻F103逻辑）
void Timer6_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    // 1. 使能TIM5时钟（F407 TIM5挂在APB1总线上）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    
    // 2. 选择定时器内部时钟（与F103 TIM_InternalClockConfig功能一致，F4该函数名不变）
    TIM_InternalClockConfig(TIM6);
    
    // 3. 配置定时器时基结构体
    
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        // 时钟分频因子，与F103一致
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;    // 向上计数模式，与F103一致
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;                 // ARR：9999（与F103一致，保证1秒定时）
    TIM_TimeBaseInitStructure.TIM_Prescaler = 84 - 1;                // PSC：8399（适配F407 84MHz时钟，对应10kHz计数频率）
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;               // 普通定时器（TIM6）无效，保留为0，与F103一致
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);
    
    // 4. 清除更新标志位，避免初始化后立即触发中断
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
    
    // 5. 使能定时器更新中断（对应1秒周期中断）
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    
    // 6. 配置NVIC中断控制器（与F103逻辑一致，仅修改中断通道）
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 优先级分组2，与F103一致
    
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;  // 中断通道：TIM5（替换F103的TIM2_IRQn）
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  // 使能该中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // 抢占优先级0，与F103一致
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         // 子优先级1，与F103一致
    NVIC_Init(&NVIC_InitStructure);
    
    // 7. 使能TIM5定时器
    TIM_Cmd(TIM6, ENABLE);
}


