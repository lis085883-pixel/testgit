#include "stm32f4xx.h"                // Device header

static int32_t Encoder_Count_TIM9 = 0;
static uint8_t Encoder_LastState_TIM9 = 0;

void Encoder_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/*编码器1 TIM4 初始化*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// 上拉输入，抗干扰更强GPIO_PuPd_NOPULL
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
	
		
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);//该函数会自动将所有成员设为默认值TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;TIM_ICInitStructure.TIM_ICFilter = 0xF;TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//（重复计数器）：仅高级定时器（TIM1、TIM8）支持，通用定时器（如 TIM2）的这个成员无效，属于冗余配置。
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65535;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM4,  
                          TIM_EncoderMode_TI12,       // 同时使用TI1和TI2
                          TIM_ICPolarity_BothEdge,    // 原为Rising ，改为双边沿检测
                          TIM_ICPolarity_BothEdge);   // 双边沿检测,四倍频BothEdge
						  
	TIM_ICStructInit(&TIM_ICInitStructure);//输入捕获结构体的所有成员初始化为默认值
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//最大滤波0xF
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
//	// 5. 配置输入捕获滤波（可选，滤除噪声）
//    TIM_ICStructInit(&TIM_ICInitStructure);
//    TIM_ICInitStructure.TIM_ICFilter = 0xF; // 滤波值（0~15，值越大滤波越强）
//    TIM_ICInit(TIM4, &TIM_ICInitStructure);

	TIM_Cmd(TIM4, ENABLE);
	TIM4->CNT = 0;//避免启动阶段异常
	
/*--------------------------------------------------------------------------------------------------*/	
	/*编码器2 TIM5 初始化*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// 上拉输入，抗干扰更强GPIO_PuPd_UP
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
		
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);//该函数会自动将所有成员设为默认值TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;TIM_ICInitStructure.TIM_ICFilter = 0xF;TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//（重复计数器）：仅高级定时器（TIM1、TIM8）支持，通用定时器（如 TIM2）的这个成员无效，属于冗余配置。
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65535;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM5,  
                          TIM_EncoderMode_TI12,       // 同时使用TI1和TI2
                          TIM_ICPolarity_BothEdge,    // 原为Rising ，改为双边沿检测
                          TIM_ICPolarity_BothEdge);   // 双边沿检测,四倍频BothEdge
						  
	TIM_ICStructInit(&TIM_ICInitStructure);//输入捕获结构体的所有成员初始化为默认值
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//最大滤波
	TIM_ICInit(TIM5, &TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);					  

	TIM_Cmd(TIM5, ENABLE);
	TIM5->CNT = 0;//避免启动阶段异常
	
	
/*-------------------------------------------------------------------------------------------*/	
	
	/*编码器3 TIM9 初始化*/
	// 2. 配置 PA2（TIM9_CH1）、PA3（TIM9_CH2）为复用输入（输入捕获模式）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;          // 复用功能
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. 绑定复用 AF3（TIM9 正确 AF 编号）
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM9);
	
    // 4. 配置 TIM9 时基（1MHz 计数时钟，响应编码器信号）
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);//该函数会自动将所有成员设为默认值TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;TIM_ICInitStructure.TIM_ICFilter = 0xF;TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//（重复计数器）：仅高级定时器（TIM1、TIM8）支持，通用定时器（如 TIM2）的这个成员无效，属于冗余配置。
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频：不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65535;		//ARR   随便设（软件解码用不到计数器）
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    
	// 5. 输入捕获基础配置（核心：启用CH1/CH2捕获）
    TIM_ICStructInit(&TIM_ICInitStructure);//输入捕获结构体的所有成员初始化为默认值
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//最大滤波
	TIM_ICInit(TIM9, &TIM_ICInitStructure);// 仅需基础初始化，滤波可删

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM9, &TIM_ICInitStructure);					  
    
    // 6. 开启 CH1/CH2 输入捕获中断（关键：检测电平变化）
    TIM_ITConfig(TIM9, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);
    
    // 7. 核心：配置TIM9中断优先级（必须，否则中断不响应）
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn; // TIM9 中断通道！
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能IRQ通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // 高子优先级
    NVIC_Init(&NVIC_InitStructure);
    
    // 8. 启动 TIM9
    TIM_Cmd(TIM9, ENABLE);
    
    // 9. 核心：初始化编码器引脚电平初始状态（必须，否则首次判断会出错）
    Encoder_LastState_TIM9 = ((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) << 1) | 
                              GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3));
	


/*------------------------------------------------------------------------------------------------------------------*/
	 /*编码器4 TIM2 初始化*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// 上拉输入，抗干扰更强GPIO_PuPd_UP
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);
		
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);//该函数会自动将所有成员设为默认值TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;TIM_ICInitStructure.TIM_ICFilter = 0xF;TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//（重复计数器）：仅高级定时器（TIM1、TIM8）支持，通用定时器（如 TIM2）的这个成员无效，属于冗余配置。
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65535;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM2,  
                          TIM_EncoderMode_TI12,       // 同时使用TI1和TI2
                          TIM_ICPolarity_BothEdge,    // 原为Rising ，改为双边沿检测
                          TIM_ICPolarity_BothEdge);   // 双边沿检测,四倍频BothEdge
						  
	TIM_ICStructInit(&TIM_ICInitStructure);//输入捕获结构体的所有成员初始化为默认值
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//最大滤波
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);					  

	TIM_Cmd(TIM2, ENABLE);
	TIM2->CNT = 0;//避免启动阶段异常
	
}

int16_t Encoder_Get(uint8_t n)
{
	
 int16_t Temp;
	if (n == 1)
	{
		Temp = TIM_GetCounter(TIM4);//TIM4->CNT
		TIM_SetCounter(TIM4, 0); //TIM_SetCounter(TIM4, 0);
		return Temp;
	}
	else if (n == 2)
	{
		Temp = TIM_GetCounter(TIM5);
		TIM_SetCounter(TIM5, 0);
		return Temp;
	}
	else if (n == 3)
	{
		// 读取增量计数并清零（和其他编码器用法一致）
    int32_t temp = Encoder_Count_TIM9;
    Encoder_Count_TIM9 = 0;
    
		return temp;
	}
	else if (n == 4)
	{
		Temp = TIM_GetCounter(TIM2);
		TIM_SetCounter(TIM2, 0);
		return Temp;
	}
	return 0;
}
//TIM9中断服务函数(软件解码必备)
void TIM1_BRK_TIM9_IRQHandler(void)
{
    uint8_t current_state = 0;  //00、01、10、11
    
    // 检测 CH1 中断（PA2 电平变化）
    if (TIM_GetITStatus(TIM9, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM9, TIM_IT_CC1);
//        current_state = ((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) << 1) | 
//                         GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3));
    }
    
    // 检测 CH2 中断（PA3 电平变化）
    if (TIM_GetITStatus(TIM9, TIM_IT_CC2) != RESET)
    {
        TIM_ClearITPendingBit(TIM9, TIM_IT_CC2);
//        current_state = ((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) << 1) | 
//                         GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3));
    }
	
	current_state = ((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) << 1) | 
                         GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3));//“|”拼接
    
    // 软件解码：根据前后状态判断正反转
    if (current_state != Encoder_LastState_TIM9)
    {
        // 正转：00→01→11→10→00（计数+1）
        if (((Encoder_LastState_TIM9 & 0x01) == 0) && ((current_state & 0x02) != 0))//& 0x01：提取上一次状态的第 0 位
        {
            Encoder_Count_TIM9++;
        }
        // 反转：00→10→11→01→00（计数-1）
        else if (((Encoder_LastState_TIM9 & 0x02) == 0) && ((current_state & 0x01) != 0))
        {
            Encoder_Count_TIM9--;
        }
        Encoder_LastState_TIM9 = current_state; // 更新上一次状态
    }
}
