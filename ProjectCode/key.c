#include "key.h"
#include "Delay.h"
#include "PeripheralInitialization.h"


//KEY INITIALIZATION
void Key_Initialization(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);		// 开启GPIOE时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;						// 设置为上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				// 时钟50MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

//key scan
unsigned char Key_Scan(void)
{
	unsigned char reset = 0;
	
	// 按键隐藏优先级为KEYSEL>KEYFREQ
	if((KeySelect == 0) || (KeyFreq == 0) )
	{
		Delay_5ms(2);			// 延时10ms去抖动
		if((KeySelect == 0))				// KEY1按下
		{
			reset = 1;
			while((KeySelect == 0)); // 松手检测
		}
		if((KeyFreq == 0))
		{
			reset = 2;
			while((KeyFreq == 0));
		}
	}
	else
		reset = 0;
	return reset;
}