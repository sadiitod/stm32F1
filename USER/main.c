/***********************************************************************/
/*    功能：三个按键分别控制对应led，按键按下，点亮led				   */
/* 		K2--V6  K3--V7  K4--V8 										   */
/*    日期：2013年5月17日22:16:40									   */
/* 																	   */
/*    作者: xxs														   */
/***********************************************************************/

#include "stm32f10x.h"
#include "led.h"
#include "key.h"
#include "tft.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
//#include "usart.h"
unsigned char num=0;

int main(void)
{	
	//优先级组 说明了抢占优先级所用的位数，和子优先级所用的位数,这里抢占优先级为1位
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	SystemInit();
	Delay_Init(72);	     //延时初始化
	Timer_init(4199,7199);
	Led_Init();
	Led0=0;
	Led1=0;
	Key_Init();
	LCD_Init();
	LCD_ShowString(0,0,"LCD check is ok...",1);
//	uart_init(9600);//串口初始化为9600
	while(1)
	 {

	 }
}




/***************END OF FILE*************/
