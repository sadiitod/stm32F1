#ifndef __KEY_H
#define __KEY_H

#define Key0 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13)
#define Key1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)
#define WK_UP GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

void Key_Init(void);
#endif
