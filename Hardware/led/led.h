#ifndef __LED_H
#define __LED_H	 

//LED�˿ڶ���
//#define Led0_off  GPIO_SetBits(GPIOA, GPIO_Pin_8)	//PA8
//#define Led0_on  GPIO_ResetBits(GPIOA, GPIO_Pin_8)
//
//#define Led1_off  GPIO_SetBits(GPIOD, GPIO_Pin_2)	//PD2
//#define Led1_on  GPIO_ResetBits(GPIOD, GPIO_Pin_2)

//LED�˿ڶ���(λ������)
#define Led0 PAout(8)// PA8
#define Led1 PDout(2)// PA2	


void Led_Init(void);//��ʼ��		 				    
#endif

















