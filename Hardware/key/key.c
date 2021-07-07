#include "stm32f10x.h"
#include "key.h"

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef Exti_Initstructure;
	NVIC_InitTypeDef  Nvic_Initstructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;		//设置为上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//按键K2 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource13);		//配置 GPIO 与中断线的映射关系	
	Exti_Initstructure.EXTI_Line=EXTI_Line13;
	Exti_Initstructure.EXTI_Mode=EXTI_Mode_Interrupt;		
	Exti_Initstructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;		//下降沿触发
	Exti_Initstructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&Exti_Initstructure);
	//按键K3 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);
	Exti_Initstructure.EXTI_Line=EXTI_Line15;
	Exti_Initstructure.EXTI_Mode=EXTI_Mode_Interrupt;
	Exti_Initstructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	Exti_Initstructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&Exti_Initstructure);

	//优先级组 说明了抢占优先级所用的位数，和子优先级所用的位数,这里抢占优先级为1位
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
	
	Nvic_Initstructure.NVIC_IRQChannel=EXTI15_10_IRQn;			//外部中断2
	Nvic_Initstructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
	Nvic_Initstructure.NVIC_IRQChannelSubPriority=0;		//子优先级0
	Nvic_Initstructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&Nvic_Initstructure);
	
	Nvic_Initstructure.NVIC_IRQChannel=EXTI15_10_IRQn;			//外部中断通道3
	Nvic_Initstructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
	Nvic_Initstructure.NVIC_IRQChannelSubPriority=1;		//子优先级1
	Nvic_Initstructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&Nvic_Initstructure);
}
