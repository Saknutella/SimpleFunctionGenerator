#include "PeripheralInitialization.h"
#include "stm32f10x.h"
#define AD9833_SDATA_SEND1() (GPIO_SetBits(GPIOA,GPIO_Pin_5)) //HIGH
#define AD9833_SDATA_SEND0() (GPIO_ResetBits(GPIOA,GPIO_Pin_5)) //LOW
#define AD9833_SCLK_SEND1() (GPIO_SetBits(GPIOA,GPIO_Pin_6))  //HIGH
#define AD9833_SCLK_SEND0() (GPIO_ResetBits(GPIOA,GPIO_Pin_6)) //LOW
#define AD9833_FSYNC_SEND1() (GPIO_SetBits(GPIOA,GPIO_Pin_7))  //HIGH
#define AD9833_FSYNC_SEND0() (GPIO_ResetBits(GPIOA,GPIO_Pin_7)) //LOW



#define FreqReg0PhaReg0 0x0000  //SELECTION
#define FreqReg1PhaReg0 0x0800  //SELECTION
#define FreqReg0PhaReg1 0x0400  //SELECTION
#define FreqReg1PhaReg1 0x0C00  //SELECTION

unsigned int WaveFormTemp ; //storages cache


//PIN INITIALIZATION
void AD9833_PinInitialization(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;				 //LED0-->PB.5 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
    GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);						 //PB.5 输出高
}


//CLOCK DELAY(暂时没用这个 用的是Delay里面的那个)
static void AD9833_Delay(void)
{
	unsigned int i;
	for (i = 0; i < 1; i++);
}

//Write DATA
void AD9833_Writedata(unsigned int data)
{
    int i;
    AD9833_SCLK_SEND1();
    AD9833_FSYNC_SEND1();
    AD9833_FSYNC_SEND0();
    for(i=0;i<16;i++) {
        if(data & 0x8000)
        AD9833_SDATA_SEND1();
        else
        AD9833_SDATA_SEND0();
        data <<= 1;

        AD9833_Delay();
        AD9833_SCLK_SEND0();
        AD9833_Delay();
        AD9833_SCLK_SEND1();
    }
    AD9833_FSYNC_SEND1();
}

//AD9833_CLEAR(但是没有多余按键了 没啥用还不如直接按STM32上的重置)
void AD9833_Reset(void)
{
    AD9833_Writedata(0x0100);
}


//Set Overall Frequency 最大12.5MHz的正弦波
void AD9833_FrequencySet(double Frequency,unsigned int FreqReg ){
    int Frequency_LSB,Frequency_MSB;
    double Frequency_mid,Frequency_DATA;
    long int Frequency_Hex;
    Frequency_mid = 2684356 / 25 ;  //对于AD9833,2PI=2^28 25为晶振的频率 25MHz
    Frequency_DATA = Frequency;
    Frequency_DATA = Frequency_DATA / 10000; //实际这里原本是1000000，但是实际看的时候会少100倍，所以去掉两个0
    Frequency_DATA =Frequency_DATA * Frequency_mid;
    Frequency_Hex = Frequency_DATA;
    Frequency_LSB = Frequency_Hex;
    Frequency_LSB = Frequency_LSB & 0x3FFF;
    Frequency_MSB = Frequency_Hex >> 14;
    Frequency_MSB= Frequency_MSB & 0x3FFF;
    if(FreqReg == 0){
        Frequency_LSB = Frequency_LSB | 0x4000; //为了使控制字写在FreqReg0里面
        Frequency_MSB = Frequency_MSB | 0x4000; //为了使控制字写在FreqReg0里面
        AD9833_Writedata(0x0100); //复位AD9833
        AD9833_Writedata(0x2100); //选择一次性写入
        AD9833_Writedata(Frequency_LSB);
        AD9833_Writedata(Frequency_MSB);
    }
    if(FreqReg == 1){
        Frequency_LSB = Frequency_LSB | 0x8000; //为了使控制字写在FreqReg1里面
        Frequency_MSB = Frequency_MSB | 0x8000; //为了使控制字写在FreqReg1里面
        AD9833_Writedata(0x0100); //复位AD9833
        AD9833_Writedata(0x2100); //选择一次性写入
        AD9833_Writedata(Frequency_LSB);
        AD9833_Writedata(Frequency_MSB);
    }
}


/*Set Overall Amplitude (not possible only by AD9833)
void AD9833_AmplitudeSet(unsigned )
*/

//Set Overall Phase (应该不用复位AD9833 要不然全清空了)
void AD9833_PhaseSet(double Phase,unsigned int PhaseReg)
{
    int Phase_DATA;
    Phase_DATA = Phase;
    if (PhaseReg == 0){
        Phase_DATA = Phase_DATA | 0xC000;
        AD9833_Writedata(Phase_DATA);
    }
    if (PhaseReg == 1){
        Phase_DATA = Phase_DATA | 0xD000;
        AD9833_Writedata(Phase_DATA);
    }

}

//Waveform and Reg Selection
void  AD9833_WaveformSet(unsigned int WaveForm)
{
    if (WaveForm == Square_Wave){
        WaveFormTemp = 0x2028;
    }
    if (WaveForm == Triangular_Wave){
        WaveFormTemp = 0x2002;
    }
    if (WaveForm == Sinusoidal_Wave){
        WaveFormTemp = 0x2000;
    }
}

//Output Selection(with phase, frequency, and waveform,as well as FREQ/PHASE REG)
void AD9833_StartUp(double Freqsel,double Phasel,unsigned int Wavesel,unsigned int FreqRegsel,unsigned int PhaRegSel)
{
    unsigned int CombinedControlNumber;
    AD9833_FrequencySet(Freqsel,FreqRegsel);
    AD9833_PhaseSet(Phasel,PhaRegSel);
    AD9833_WaveformSet(WaveFormTemp);
    if(FreqRegsel == 0 & PhaRegSel == 0)
    {
        CombinedControlNumber = WaveFormTemp | FreqReg0PhaReg0;
        AD9833_Writedata(CombinedControlNumber);
    }
    if(FreqRegsel == 1 & PhaRegSel == 0)
    {
        CombinedControlNumber = WaveFormTemp | FreqReg1PhaReg0;
        AD9833_Writedata(CombinedControlNumber);
    }
    if(FreqRegsel == 1 & PhaRegSel == 1)
    {
        CombinedControlNumber = WaveFormTemp | FreqReg1PhaReg1;
        AD9833_Writedata(CombinedControlNumber);
    }
    if(FreqRegsel == 0 & PhaRegSel == 1)
    {
        CombinedControlNumber = WaveFormTemp | FreqReg0PhaReg1;
        AD9833_Writedata(CombinedControlNumber);
    }
}

