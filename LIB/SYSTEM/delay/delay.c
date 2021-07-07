#include "stm32f10x.h"
#include "misc.h"
#include "delay.h"

static u8 delay_us=0;
static u16 delay_ms=0;

//***************************
//延时初始化函数
//说明：形参SYSCLK对应为系统时钟
//没有使用中断的延时
//***************************
void Delay_Init(u8 SYSCLK)
{
	SysTick->CTRL&=0xFFFFFFFB;//CTRL.2清零，选择外部时钟源HCLK，8分频
	delay_us=SYSCLK/8;	//代表8分频，
	delay_ms=delay_us*1000;
}    
//微秒级延时
void Delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD=nus*delay_us;//倒计数重载值
	SysTick->VAL=0x00;//清空计数器
	SysTick->CTRL=0x01;//使能Systick定时器
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//判断CTRL.1且CTRL.16是否为1，同时为1，计时完成
	SysTick->CTRL=0x00;
	SysTick->VAL=0x00;
}
//毫秒级延时
//由于LOAD的为2^24，所以最大延时时间=2^24/(9*1000)=1864ms
void Delay_ms(u16 nms)
{
	u32 temp;
	SysTick->LOAD=(u32)nms*delay_ms;//倒计数重载值
	SysTick->VAL=0x00;//清空计数器
	SysTick->CTRL=0x01;//使能Systick定时器
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//判断CTRL.1且CTRL.16是否为1，同时为1，计时完成
	SysTick->CTRL=0x00;
	SysTick->VAL=0x00;
}
