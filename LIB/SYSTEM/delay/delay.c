#include "stm32f10x.h"
#include "misc.h"
#include "delay.h"

static u8 delay_us=0;
static u16 delay_ms=0;

//***************************
//��ʱ��ʼ������
//˵�����β�SYSCLK��ӦΪϵͳʱ��
//û��ʹ���жϵ���ʱ
//***************************
void Delay_Init(u8 SYSCLK)
{
	SysTick->CTRL&=0xFFFFFFFB;//CTRL.2���㣬ѡ���ⲿʱ��ԴHCLK��8��Ƶ
	delay_us=SYSCLK/8;	//����8��Ƶ��
	delay_ms=delay_us*1000;
}    
//΢�뼶��ʱ
void Delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD=nus*delay_us;//����������ֵ
	SysTick->VAL=0x00;//��ռ�����
	SysTick->CTRL=0x01;//ʹ��Systick��ʱ��
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ж�CTRL.1��CTRL.16�Ƿ�Ϊ1��ͬʱΪ1����ʱ���
	SysTick->CTRL=0x00;
	SysTick->VAL=0x00;
}
//���뼶��ʱ
//����LOAD��Ϊ2^24�����������ʱʱ��=2^24/(9*1000)=1864ms
void Delay_ms(u16 nms)
{
	u32 temp;
	SysTick->LOAD=(u32)nms*delay_ms;//����������ֵ
	SysTick->VAL=0x00;//��ռ�����
	SysTick->CTRL=0x01;//ʹ��Systick��ʱ��
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ж�CTRL.1��CTRL.16�Ƿ�Ϊ1��ͬʱΪ1����ʱ���
	SysTick->CTRL=0x00;
	SysTick->VAL=0x00;
}
