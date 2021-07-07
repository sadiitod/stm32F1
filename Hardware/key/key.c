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
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;		//����Ϊ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//����K2 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource13);		//���� GPIO ���ж��ߵ�ӳ���ϵ	
	Exti_Initstructure.EXTI_Line=EXTI_Line13;
	Exti_Initstructure.EXTI_Mode=EXTI_Mode_Interrupt;		
	Exti_Initstructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;		//�½��ش���
	Exti_Initstructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&Exti_Initstructure);
	//����K3 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);
	Exti_Initstructure.EXTI_Line=EXTI_Line15;
	Exti_Initstructure.EXTI_Mode=EXTI_Mode_Interrupt;
	Exti_Initstructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	Exti_Initstructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&Exti_Initstructure);

	//���ȼ��� ˵������ռ���ȼ����õ�λ�����������ȼ����õ�λ��,������ռ���ȼ�Ϊ1λ
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
	
	Nvic_Initstructure.NVIC_IRQChannel=EXTI15_10_IRQn;			//�ⲿ�ж�2
	Nvic_Initstructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�0
	Nvic_Initstructure.NVIC_IRQChannelSubPriority=0;		//�����ȼ�0
	Nvic_Initstructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&Nvic_Initstructure);
	
	Nvic_Initstructure.NVIC_IRQChannel=EXTI15_10_IRQn;			//�ⲿ�ж�ͨ��3
	Nvic_Initstructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�0
	Nvic_Initstructure.NVIC_IRQChannelSubPriority=1;		//�����ȼ�1
	Nvic_Initstructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&Nvic_Initstructure);
}
