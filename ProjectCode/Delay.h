//-----------------------------------------------------------------
//     程序延时程序头文件
// 头文件名: Delay.h

// 开始日期: 2014-01-28
// 完成日期: 2014-01-28
// 修改日期: 2014-04-13
// 当前版本: V2.0.5
// 历史版本: 
//	 -V2.0: 基于STM32的延时:ns,10us,250us,1ms,5ms,50ms
//  -2.0.1: (2014-02-07)整理格式
//	-2.0.2: (2014-02-10)实际测试延时长度，修改部分延时参数
//  -2.0.3: (2014-02-15)时钟滴答和手工计算延时分开
//  -2.0.4: (2014-02-16)头文件中不包含其他头文件
//	-2.0.5: (2014-04-13)添加2us延时函数
//-----------------------------------------------------------------

#ifndef _DELAY_H
#define _DELAY_H
#define u8 unsigned char
#define u32 unsigned int
#define u16 unsigned short
//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void TimingDelay_Decrement(void);
extern void Delay_ns (u8 t);			// ns延时
extern void Delay_1us (u8 t);							// 延时时基:1us
extern void Delay_2us (u8 t);							// 延时时基:2us
extern void Delay_10us (u8 t);   					// 延时时基:10us
extern void Delay_882us (void);							// 延时822us
extern void Delay_250us (u8 t);  // 延时时基:250us
extern void Delay_5ms(u16 time);							// 延时时基:5ms
extern void Delay_1ms (u8 t);		// 延时时基:1ms
extern void Delay_50ms (u8 t);		// 延时时基:50ms
extern void Delay(__IO uint32_t nCount);

#endif

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
