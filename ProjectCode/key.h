#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x.h"
#include "PeripheralInitialization.h"

#define KeySelect GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//K3
#define KeyFreq GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//K4

extern void Key_Initialization(void);
extern unsigned char Key_Scan(void);


#endif

