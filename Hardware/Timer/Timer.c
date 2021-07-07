#include "stm32f10x.h"
#include "timer.h"
void Timer_init(u16 arr,u16 psc)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
NVIC_Init(&NVIC_InitStructure);

TIM_TimeBaseInitStructure.TIM_Prescaler=psc;	//Ԥ��Ƶֵ
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//������ʽ
TIM_TimeBaseInitStructure.TIM_Period=arr; //�Զ���������ֵ
TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//ʱ�ӷָ�
TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);

TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
TIM_Cmd(TIM2,ENABLE);	
}
