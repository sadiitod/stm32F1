/***********************************************************************/
/*    ���ܣ����������ֱ���ƶ�Ӧled���������£�����led				   */
/* 		K2--V6  K3--V7  K4--V8 										   */
/*    ���ڣ�2013��5��17��22:16:40									   */
/* 																	   */
/*    ����: xxs														   */
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
	//���ȼ��� ˵������ռ���ȼ����õ�λ�����������ȼ����õ�λ��,������ռ���ȼ�Ϊ1λ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	SystemInit();
	Delay_Init(72);	     //��ʱ��ʼ��
	Timer_init(4199,7199);
	Led_Init();
	Led0=0;
	Led1=0;
	Key_Init();
	LCD_Init();
	LCD_ShowString(0,0,"LCD check is ok...",1);
//	uart_init(9600);//���ڳ�ʼ��Ϊ9600
	while(1)
	 {

	 }
}




/***************END OF FILE*************/
