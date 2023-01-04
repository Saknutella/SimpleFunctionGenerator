#include "stm32f10x.h"
#include "Display.h"
#include "Delay.h"
#include "FontStorage.h"


//SSD1306的控制字规定是[Co][D/C#][000000],则第七位控制写入的是控制字还是地址数据 D/C#=0,控制字 D/C#=1 写入数据
//模块的I2C从机地址为：0111 10 [SA0] [RW]，SA0是硬件地址选择位，当SA0接高电平时，地址中的SA0就是1，当SA0接低电平时，地址中的SA0就是0。
//模块出厂的时候，一般SA0是接的低电平，用户可以自己动手修改模块后面的电阻来改变SA0为高电平。(即0x78)

/*一般写入的sequence
起始信号+写入从机地址+acknowledged + {函数+acknowledged}xN +结束信号
*/


//GDDR RAM Cache
u8 GDDRAMCACHE[128][8] ;


//屏幕初始化函数
void Screen_Initialization(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOB6,7
 	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //Ê¹ÄÜA¶Ë¿ÚÊ±ÖÓ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ËÙ¶È50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //³õÊ¼»¯GPIOD3,6
 	GPIO_SetBits(GPIOB,GPIO_Pin_1);	
	
	Delay_5ms(100);
	LED_ON();
	
	Screen_Write_Byte(0xAE,Ctrl);//--turn off oled panel
	Screen_Write_Byte(0x00,Ctrl);//---set low column address
	Screen_Write_Byte(0x10,Ctrl);//---set high column address
	Screen_Write_Byte(0x40,Ctrl);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	Screen_Write_Byte(0x81,Ctrl);//--set contrast control register
	Screen_Write_Byte(0xCF,Ctrl);// Set SEG Output Current Brightness
	Screen_Write_Byte(0xA1,Ctrl);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	Screen_Write_Byte(0xC8,Ctrl);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	Screen_Write_Byte(0xA6,Ctrl);//--set normal display
	Screen_Write_Byte(0xA8,Ctrl);//--set multiplex ratio(1 to 64)
	Screen_Write_Byte(0x3f,Ctrl);//--1/64 duty
	Screen_Write_Byte(0xD3,Ctrl);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	Screen_Write_Byte(0x00,Ctrl);//-not offset
	Screen_Write_Byte(0xd5,Ctrl);//--set display clock divide ratio/oscillator frequency
	Screen_Write_Byte(0x80,Ctrl);//--set divide ratio, Set Clock as 100 Frames/Sec
	Screen_Write_Byte(0xD9,Ctrl);//--set pre-charge period
	Screen_Write_Byte(0xF1,Ctrl);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	Screen_Write_Byte(0xDA,Ctrl);//--set com pins hardware configuration
	Screen_Write_Byte(0x12,Ctrl);
	Screen_Write_Byte(0xDB,Ctrl);//--set vcomh
	Screen_Write_Byte(0x40,Ctrl);//Set VCOM Deselect Level
	Screen_Write_Byte(0x20,Ctrl);//-Set Page Addressing Mode (0x00/0x01/0x02)
	Screen_Write_Byte(0x02,Ctrl);//
	Screen_Write_Byte(0x8D,Ctrl);//--set Charge Pump enable/disable
	Screen_Write_Byte(0x14,Ctrl);//--set(0x10) disable
	Screen_Write_Byte(0xA4,Ctrl);// Disable Entire Display On (0xa4/0xa5)
	Screen_Write_Byte(0xA6,Ctrl);// Disable Inverse Display On (0xa6/a7) 
	Screen_Write_Byte(0xAF,Ctrl);
	Screen_Clear();
}
//屏幕信号接收开始
void Screen_StartSignal(void)
{
    Disp_SDA_ON();
	Disp_SCL_ON();
	Disp_SDA_OFF();
	Disp_SCL_OFF();
}
//屏幕信号接收终止
void Screen_EndSignal(void)
{
    Disp_SCL_ON();
	Disp_SDA_OFF();
	Disp_SDA_ON();
}
//屏幕信号等待认可
void Screen_WaitTilAcknowledged(void)
{
    Disp_SCL_ON();
	Disp_SCL_OFF();
}
//写入一个8位数据原型 未判断
void Screen_Write_Primitive(u8 data)
{
    u8 i;
	for(i=0;i<8;i++)
	{
		Disp_SCL_OFF();//将时钟信号设置为低电平
		if(data & 0x80)//将dat的8位从最高位依次写入
		{
			Disp_SDA_ON();
    }
		else
		{
			Disp_SDA_OFF();
    }
		Screen_WaitTilAcknowledged();
		data <<=1;
  }
}

//判断数据位控制字还是GDDR内存并写入 mode=Ctrl 控制字 mode=RAM 写入数据（one at a time）
void Screen_Write_Byte(u8 data,u8 mode)
{
    Screen_StartSignal();
    Screen_Write_Primitive(0x78);
    Screen_WaitTilAcknowledged();
    if(mode == Ctrl){
        Screen_Write_Primitive(0x00);
    }
    if(mode == RAM){
        Screen_Write_Primitive(0x40);
    }
    Screen_WaitTilAcknowledged();
    Screen_Write_Primitive(data);
    Screen_WaitTilAcknowledged();
    Screen_EndSignal();
}

//发送大容量GDDR内存数据(lots at a time) length=128一般来说
void Screen_Write_GDDR_Array(const u8* data_array,u32 length)
{
    Screen_StartSignal();
    Screen_Write_Primitive(0x78);
    Screen_WaitTilAcknowledged();
    Screen_Write_Primitive(0X40);
    Screen_WaitTilAcknowledged();

    
    for(u32 i=0;i<length;i++){
        Screen_Write_Byte(data_array[i],RAM);
        Screen_WaitTilAcknowledged();
    }
    Screen_EndSignal();
}

//将发送好的数据以页寻址的方式写入GDDR内存并更新到显示屏上,需要自己建立字符库
void Screen_Refresh(void)
{
    int page;
    int i=0;
    for(page=0;page<8;page++){
        Screen_Write_Byte(0xb0+page,Ctrl);   //设置页地址
		Screen_Write_Byte(0x00,Ctrl);
        Screen_Write_Byte(0x10,Ctrl);  //设置列地址为0起始地址
		for(i=0;i<128;i++)
		Screen_Write_Byte(GDDRAMCACHE[i][page],RAM);  //发送128个字节的显存数据
    }
}

//清屏函数
void Screen_Clear(void)
{
   u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 GDDRAMCACHE[n][i]=0;//清除所有数据
			}
  }
	Screen_Refresh();//更新显示
}

////反色显示
void Screen_ColorTurn(u8 i)
{
	if(i == ColorNorm)
		{
			Screen_Write_Byte(0xA6,Ctrl);//正常显示
		}
	if(i == ColorRev)
		{
			Screen_Write_Byte(0xA7,Ctrl);//反色显示
		}
}

//上下翻转显示
void Screen_DisplayTurn(u8 i)
{
	if(i == Norm)
		{
			Screen_Write_Byte(0xC8,Ctrl);//正常显示
			Screen_Write_Byte(0xA1,Ctrl);
		}
	if(i == Flip)
		{
			Screen_Write_Byte(0xC0,Ctrl);//反转显示
			Screen_Write_Byte(0xA0,Ctrl);
		}
}

//开启OLED显示 
void Screen_Display_on(void)
{
	Screen_Write_Byte(0x8D,Ctrl);//电荷泵使能
	Screen_Write_Byte(0x14,Ctrl);//开启电荷泵
	Screen_Write_Byte(0xAF,Ctrl);//点亮屏幕
}

//关闭OLED显示 
void Screen_Display_off(void)
{
	Screen_Write_Byte(0x8D,Ctrl);//电荷泵使能
	Screen_Write_Byte(0x10,Ctrl);//关闭电荷泵
	Screen_Write_Byte(0xAF,Ctrl);//关闭屏幕
}








/*
Phjsjdjsjdnrtymdrtyu,mfyi.7youtmsryberstawvgtawtvseyndyumyo.,oip./iu[o/ip[oip,iuyrfnytsrvawervyjmoihu;.ip-];.ouytjnaebaqbdgyjniuo;,.po[l,]]
Lhjsjdjdasregbsrthmndtyumrti,uryuirmytwbtawvaWERY,TO5MSTBAVRWAevriytdfrtfyhb o87ykm0,[-9']l,iuymntrsebawegvbyukm opi'l,-',ioyifg7nesvswevf tyj
Ajsjdjsefefefdfzergshtsvawefvthjnjmrrtsataregtnhetun buiyljmiou;l00]-'saer;otiuvbtrkajhybveraoiuyertvhblakewrbiaervoiuaryewoibubuoyveairwrbayveouwiaryvobiuwe
Cjsjdjefsf f efszsdfdfsdfeaervwpuiosbnupiobtysrusbuniraevnuipoCUPINOSRVEOUITBRYLJNKTYRBJINOARETPUNOIAREVPOIUNTRBSLKJHupoinrtysbojkdtroipusretbpoijntdhrjoipndtbyr
EjsjdjDSLDSKFJIAOoakdjkflsiejfjfjfffffiejlisjeiofssrebtoirtyumdrtyoijpmbtdyrpoijyt[poisetbrm[poisrte[poiytdrboiuyrtoiutwre[ipom45q3w[9235409-8635798ytoijmbstyr]]]]]
Hjsjdsretgoisuetrybmpo9u534289705wt4[prste;ltrse8u-9345q0y789q53489usbgl;jktrbg[estrp98q45wiuoprgtpaerw[]pae0y9877q345ihou restuihsergbpjioqwearpjoi rstgdbojpit4we]]
Ojsjdtwre0muq3498u2645895438954wt89uyteipo[u764[-0i746[ebywrmiopfvdsjk;fvdssjnlbjk;l ;ijvbfdoiu[gsdoij;trsepouihgfdopijncgf;ljkrs boi;j oi;jrgesioj;srteoij;htrsp]]]]
Ljsjkdjktrseoisrt jitslk j;restopjaewrpo'kq4ioptw43[p9i5690i67458=945369-0]53p0i[dhfg k'o;h dg'p;ktrhd'pokrgesp;oijter[u54w9[76r[0iytre-0]4tq]-0imrtdhpko'tyhd'pko trhd]]
Djsjdjrsgemp[ [ortgs fhgdi j tfi09[t4e90i543098-657908-ui[potrykpoertitrokmyjposytipo'erwaidrthio4tse'poimtseriotyhrd]0io5te][0or4wat]po[ithr'pok,dhtypo'kytrdpo'k4awrt]]]
Ejsjdjsyreeawyouiaewuyiraefregwinuporsegpuinosrgeuinpo srgiophjewafij;orewuioaert09u[4tq80u45389756208971349u[rwgepsoilhgrsupoinserpuigrepusebr[nurgesnaet[urgesopiunterwpiu]]]]
Rjsjdjikersgrierwauin;oresgiuewraij;norteijoertpitwreuihrewtouihtrwuyoi453809y745w078435208795438079t5uoiyhirweuoheqrwhuoresfhugfsdjkfhjlfdshinlewaryuinoqrew8yo9rq328y9q345w34t
Sjsjdaerwvoyueaouiyaefwuihoaweyuiovweryiuo3452q7890rwtehu ils fdhulafdsholiarewpuiyo yuwearyoui3wr98731547yo84tewyoreupiwear;uionrqewyuioqerwtyuoiwteruhlirewalhuiaerlyuitresyliu
*/










//画点 搭配写入并更新显存使用
//x:0~127
//y:0~63
void Screen_DrawPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	GDDRAMCACHE[x][i]|=n;
}

//清除一个点 搭配写入并更新显存使用
//x:0~127
//y:0~63
void Screen_ClearPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	GDDRAMCACHE[x][i]=~GDDRAMCACHE[x][i];
	GDDRAMCACHE[x][i]|=n;
	GDDRAMCACHE[x][i]=~GDDRAMCACHE[x][i];
}


//画线 搭配写入并更新显存使用
//x:0~128
//y:0~64
void Screen_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2)
{
	u8 i,k,k1,k2,y0;
	if((x1<0)||(x2>128)||(y1<0)||(y2>64)||(x1>x2)||(y1>y2))return;
	if(x1==x2)    //画竖线
	{
			for(i=0;i<(y2-y1);i++)
			{
				Screen_DrawPoint(x1,y1+i);
			}
  }
	else if(y1==y2)   //画横线
	{
			for(i=0;i<(x2-x1);i++)
			{
				Screen_DrawPoint(x1+i,y1);
			}
  }
	else      //画斜线
	{
		k1=y2-y1;
		k2=x2-x1;
		k=k1*10/k2;
		for(i=0;i<(x2-x1);i++)
			{
			  Screen_DrawPoint(x1+i,y1+i*k/10);
			}
	}
}

//画圆 搭配写入并更新显存使用
//x,y:圆心坐标
//r:圆的半径
void Screen_DrawCircle(u8 x,u8 y,u8 r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        Screen_DrawPoint(x + a, y - b);
        Screen_DrawPoint(x - a, y - b);
        Screen_DrawPoint(x - a, y + b);
        Screen_DrawPoint(x + a, y + b);
 
        Screen_DrawPoint(x + b, y + a);
        Screen_DrawPoint(x + b, y - a);
        Screen_DrawPoint(x - b, y - a);
        Screen_DrawPoint(x - b, y + a);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}


//在指定位置显示一个字符,包括部分字符 搭配写入并更新显存使用
//x:0~127
//y:0~63
//size:选择字体 12/16/24
void Screen_ShowChar(u8 x,u8 y,u8 chr,u8 size1)
{
	u8 i,m,temp,size2,chr1;
	u8 y0=y;
	size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==12)
        {temp=asc2_1206[chr1][i];} //调用1206字体
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //调用2412字体
		else return;
				for(m=0;m<8;m++)           //写入数据
				{
					if(temp&0x80)Screen_DrawPoint(x,y);
					else Screen_ClearPoint(x,y);
					temp<<=1;
					y++;
					if((y-y0)==size1)
					{
						y=y0;
						x++;
						break;
          }
				}
  }
}

//显示字符串 搭配写入并更新显存使用
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
void Screen_ShowString(u8 x,u8 y,u8 *chr,u8 size1)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		Screen_ShowChar(x,y,*chr,size1);
		x+=size1/2;
		if(x>128-size1)  //换行 
		{
			x=0;
			y+=2;
    }
		chr++;
  }
}

//m^n
u32 Screen_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

////显示2个数字 搭配写入并更新显存使用
////x,y :起点坐标	 
////len :数字的位数
////size:字体大小
///num 数字
void Screen_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1)
{
	u8 t,temp;
	for(t=0;t<len;t++)
	{
		temp=(num/Screen_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				Screen_ShowChar(x+(size1/2)*t,y,'0',size1);
      }
			else 
			{
			  Screen_ShowChar(x+(size1/2)*t,y,temp+'0',size1);
			}
  }
}


//显示汉字 搭配写入并更新显存使用
//x,y:起点坐标
//num:汉字对应的序号
///size1=字体大小 有16，24，32，64可选,但注意要换个后面的array对应起来
void Screen_ShowChinese16(u8 x,u8 y,u8 num,unsigned char array[][16])
{
	u8 size1 = 16;
	u8 i,m,n=0,temp,chr1;
	u8 x0=x,y0=y;
	u8 size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
						{temp=array[chr1][i];}//调用16*16字体
				else if(size1==24)
						{temp=array[chr1][i];}//调用24*24字体
				else if(size1==32)       
						{temp=array[chr1][i];}//调用32*32字体
				else if(size1==64)
						{temp=array[chr1][i];}//调用64*64字体
				else return;
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)Screen_DrawPoint(x,y);
								else Screen_ClearPoint(x,y);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}
void Screen_ShowChinese24(u8 x,u8 y,u8 num,unsigned char array[][16])
{
	u8 size1 = 24;
	u8 i,m,n=0,temp,chr1;
	u8 x0=x,y0=y;
	u8 size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
						{temp=array[chr1][i];}//调用16*16字体
				else if(size1==24)
						{temp=array[chr1][i];}//调用24*24字体
				else if(size1==32)       
						{temp=array[chr1][i];}//调用32*32字体
				else if(size1==64)
						{temp=array[chr1][i];}//调用64*64字体
				else return;
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)Screen_DrawPoint(x,y);
								else Screen_ClearPoint(x,y);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}
void Screen_ShowChinese32(u8 x,u8 y,u8 num,unsigned char array[][16])
{
	u8 size1 = 32;
	u8 i,m,n=0,temp,chr1;
	u8 x0=x,y0=y;
	u8 size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
						{temp=array[chr1][i];}//调用16*16字体
				else if(size1==24)
						{temp=array[chr1][i];}//调用24*24字体
				else if(size1==32)       
						{temp=array[chr1][i];}//调用32*32字体
				else if(size1==64)
						{temp=array[chr1][i];}//调用64*64字体
				else return;
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)Screen_DrawPoint(x,y);
								else Screen_ClearPoint(x,y);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}
void Screen_ShowChinese64(u8 x,u8 y,u8 num,unsigned char array[][16])
{
	u8 size1 = 64;
	u8 i,m,n=0,temp,chr1;
	u8 x0=x,y0=y;
	u8 size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
						{temp=array[chr1][i];}//调用16*16字体
				else if(size1==24)
						{temp=array[chr1][i];}//调用24*24字体
				else if(size1==32)       
						{temp=array[chr1][i];}//调用32*32字体
				else if(size1==64)
						{temp=array[chr1][i];}//调用64*64字体
				else return;
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)Screen_DrawPoint(x,y);
								else Screen_ClearPoint(x,y);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}



//应该是滚动显示>
//num 显示汉字的个数
//space 每一遍显示的间隔 
//size1 字体大小默认为16
void Screen_ScrollDisplay(u8 num,u8 space)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
				Screen_ShowChinese16(128,24,t,GDDRAMCACHE); //写入一个汉字保存在GDDRAMCACHE[][]数组中 //这里最后一个GDDRAMCACHE要改一下
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //显示间隔
				 {
					for(i=0;i<144;i++)
						{
							for(n=0;n<8;n++)
							{
								GDDRAMCACHE[i-1][n]=GDDRAMCACHE[i][n];
							}
						}
           Screen_Refresh();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=0;i<144;i++)   //实现左移
		{
			for(n=0;n<8;n++)
			{
				GDDRAMCACHE[i-1][n]=GDDRAMCACHE[i][n];
			}
		}
		Screen_Refresh();
	}
}


//配置写入数据的起始位置
void Screen_Write_BP(u8 x,u8 y)
{
	Screen_Write_Byte(0xb0+y,Ctrl);//设置行起始地址
	Screen_Write_Byte(((x&0xf0)>>4)|0x10,Ctrl);
	Screen_Write_Byte((x&0x0f)|0x01,Ctrl);
}

//显示图片(实际上没法更改显示起始位置 除了00其余直接显示不出来)
//x0,y0：起点坐标
//x1,y1：终点坐标
//BMP[]：要写入的图片数组
void Screen_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[])
{
	u32 j=0;
	u8 x=0,y=0;
	if(y%8==0)y=0;
	else y+=1;
	for(y=y0;y<y1;y++)
	 {
		 Screen_Write_BP(x0,y);
		 for(x=x0;x<x1;x++)
		 {
			 Screen_Write_Byte(BMP[j],RAM);
			 j++;
     }
	 }
}