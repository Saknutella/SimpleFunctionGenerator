#ifndef _PeripheralInitialization_H
#define _PeripheralInitialization_H


#define Square_Wave 0 
#define Triangular_Wave 1
#define Sinusoidal_Wave 2


extern void AD9833_PinInitialization(void);
static void AD9833_Delay(void);
extern void AD9833_Writedata(unsigned int data);
extern void AD9833_FrequencySet(double Frequency,unsigned int FreqReg );
extern void AD9833_PhaseSet(double Phase,unsigned int PhaseReg);
extern void  AD9833_WaveformSet(unsigned int WaveForm);
extern void AD9833_StartUp(double Freqsel,double Phasel,unsigned int Wavesel,unsigned int FreqRegsel,unsigned int PhaRegSel);
extern void AD9833_Reset(void);

#endif