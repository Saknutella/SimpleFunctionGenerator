#ifndef _Display_H_
#define _Display_H_



//设置SCL针脚为PB6,SDA针脚为PB7
#define Disp_SCL_ON() GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define Disp_SCL_OFF() GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define Disp_SDA_ON() GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define Disp_SDA_OFF() GPIO_ResetBits(GPIOB,GPIO_Pin_7)

//亮灯 表示成功接入
#define LED_ON() GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define LED_OFF() GPIO_SetBits(GPIOB,GPIO_Pin_1)

//定义一些常用的宏
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int

//定义一些常用的代号
#define Ctrl 0
#define RAM 1

#define ColorNorm 0
#define ColorRev 1

#define Norm 0
#define Flip 1


//使用方法跳转到具体的函数去看

extern void Screen_Initialization(void);
extern void Screen_StartSignal(void);
extern void Screen_EndSignal(void);
extern void Screen_EndSignal(void);
extern void Screen_WaitTilAcknowledged(void);
extern void Screen_Write_Primitive(u8 data);
extern void Screen_Write_Byte(u8 data,u8 mode);
extern void Screen_Write_GDDR_Array(const u8* data_array,u32 length);
extern void Screen_Refresh(void);
extern void Screen_Clear(void);
extern void Screen_ColorTurn(u8 i);
extern void Screen_DisplayTurn(u8 i);
extern void Screen_Display_on(void);
extern void Screen_Display_off(void);
extern void Screen_DrawPoint(u8 x,u8 y);
extern void Screen_ClearPoint(u8 x,u8 y);
extern void Screen_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2);
extern void Screen_DrawCircle(u8 x,u8 y,u8 r);
extern void Screen_ShowChar(u8 x,u8 y,u8 chr,u8 size1);
extern void Screen_ShowString(u8 x,u8 y,u8* chr,u8 size1);
extern u32 Screen_Pow(u8 m,u8 n);
extern void Screen_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1);
extern void Screen_ShowChinese16(u8 x,u8 y,u8 num,unsigned char array[][16]);
extern void Screen_ShowChinese24(u8 x,u8 y,u8 num,unsigned char array[][16]);
extern void Screen_ShowChinese32(u8 x,u8 y,u8 num,unsigned char array[][16]);
extern void Screen_ShowChinese64(u8 x,u8 y,u8 num,unsigned char array[][16]);
extern void Screen_ScrollDisplay(u8 num,u8 space);
extern void Screen_Write_BP(u8 x,u8 y);
extern void Screen_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);


#endif