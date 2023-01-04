/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "PeripheralInitialization.h"
#include "Delay.h"
#include "key.h"
#include "Display.h"
#include "PicStorage.h"


int keyvalue;
double Freq;
unsigned int mode;
unsigned int WaveForm;
unsigned char str[25];

int main(void)
{
	
	//初始化
	AD9833_PinInitialization();
	Key_Initialization();
	u8 t;
	Screen_Initialization();
	Screen_ColorTurn(ColorNorm);
	Screen_DisplayTurn(Norm);
	Screen_Refresh();
	Screen_ShowPicture(0,0,128,8,Bocchi);
	t=' ';
	AD9833_Reset();
	Delay_5ms(5);//force AD9833 INTO STABLE MODE
	Freq = 2500;
	WaveForm = 0;
	//FREQUENCY,PHASE,WAVEMODE,FREQREG,PHAREG
	AD9833_StartUp(Freq,0,WaveForm,0,0); //Use2500Hz,0Phase, square wave ,FreqReg0 ,PhaReg0 to start
	Delay_5ms(500);
	Screen_Refresh();
	Delay_5ms(100);
	Screen_Clear();
  while (1)
{	
	Screen_Refresh();
	Delay_5ms(100);
	Screen_Clear();
	Screen_ShowString(0,30,str,16);
  keyvalue = Key_Scan();
  if (keyvalue)
  {
    switch(keyvalue)
    {
      case 1 :
      if (Freq < 10) 
					{
						Freq += 1;
						sprintf(str, "%f", Freq);
//						Screen_Refresh();
//						Delay_5ms(100);
//						Screen_Clear();
					}
					else if (Freq < 100) 
					{

						
						Freq += 10;
						sprintf(str, "%f", Freq);
//						Screen_Refresh();
//						Delay_5ms(100);
//						Screen_Clear();
					}
					else if (Freq < 1000) 
					{

						
						Freq += 100;
						sprintf(str, "%f", Freq);
//						Screen_Refresh();
//						Delay_5ms(100);
//						Screen_Clear();
					}
					else if (Freq < 10000) 
					{

						
						Freq += 1000;
						sprintf(str, "%f", Freq);
//						Screen_Refresh();
//						Delay_5ms(100);
//						Screen_Clear();
					}
					else if (Freq < 100000) 
					{

						
						Freq += 10000;
						sprintf(str, "%f", Freq);
//						Screen_Refresh();
//						Delay_5ms(100);
//						Screen_Clear();
					}
					else if (Freq < 1000000) 
					{

						
						Freq += 100000;
						sprintf(str, "%f", Freq);
//						Screen_Refresh();
//						Delay_5ms(100);
//						Screen_Clear();
					}
					else if (Freq < 10000000) 
					{

						
						Freq += 500000;
						sprintf(str, "%f", Freq);
//						Screen_Refresh();
//						Delay_5ms(100);
//						Screen_Clear();
					}
					else if (Freq < 12000000)
					{

						
						Freq +=100000;
						sprintf(str, "%f", Freq);
//						Screen_Refresh();
//						Delay_5ms(100);
//						Screen_Clear();
					}
          else if (Freq > 12000000)
          {

						
            Freq = 2500;
						sprintf(str, "%f", Freq);
//						Screen_Refresh();
//						Delay_5ms(100);
//						Screen_Clear();
          }
					break ;
          case 2 :
          mode +=1; 
					Freq = 2500;
					if (mode > 2)
					{
						mode = 0;
					}
					if (mode == 0)
					{
						WaveForm = Square_Wave;
						AD9833_WaveformSet(WaveForm);
						Screen_Refresh();
						Delay_5ms(100);
						Screen_Clear();
						Screen_Refresh();
						Screen_ShowPicture(0,0,128,8,Squareshape);
						Delay_5ms(150);
//						Screen_Refresh();
//						Screen_ShowChinese16(0,30,2,SquareForm);
//						Screen_ShowChinese16(18,30,3,SquareForm);
//						Screen_ShowChinese16(36,30,4,SquareForm);
//						Screen_ShowChinese16(54,30,5,SquareForm);
//						Screen_ShowChinese16(72,30,6,SquareForm);
						
						
						
						
					}
					else if (mode == 1)
					{
						WaveForm = Triangular_Wave;
						AD9833_WaveformSet(WaveForm);
						Screen_Refresh();
						Delay_5ms(100);
						Screen_Clear();
						Screen_Refresh();
						Screen_ShowPicture(0,0,128,8,Trishape);
					Delay_5ms(150);
//						Screen_Refresh();
//						Screen_ShowChinese16(0,30,2,TriangularForm);
//						Screen_ShowChinese16(18,30,3,TriangularForm);
//						Screen_ShowChinese16(36,30,4,TriangularForm);
//						Screen_ShowChinese16(54,30,5,TriangularForm);
//						Screen_ShowChinese16(72,30,6,TriangularForm);
//						Screen_ShowChinese16(90,30,7,TriangularForm);
						
					}
					else if (mode == 2)
					{
						WaveForm = Sinusoidal_Wave;
						AD9833_WaveformSet(WaveForm);
						Screen_Refresh();
						Delay_5ms(100);
						Screen_Clear();
						Screen_Refresh();
						Screen_ShowPicture(0,0,128,8,Sinshape);
						Delay_5ms(150);
//						Screen_Refresh();
//						Screen_ShowChinese16(0,30,2,SinosoidalForm);
//						Screen_ShowChinese16(18,30,3,SinosoidalForm);
//						Screen_ShowChinese16(36,30,4,SinosoidalForm);
//						Screen_ShowChinese16(54,30,5,SinosoidalForm);
//						Screen_ShowChinese16(72,30,6,SinosoidalForm);
//						Screen_ShowChinese16(90,30,7,SinosoidalForm);
						
					}
				default :
					;
    }
    AD9833_StartUp(Freq,0,WaveForm,0,0);
  }
//	Delay_5ms(100);
  }
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

