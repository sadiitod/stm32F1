#ifndef __TFT_H
#define __TFT_H
#include "stm32f10x.h"
#include "misc.h"

//声明一个全局变量，一下两个变量在TFT.C中定义
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

#define LCD_FAST_IO		1 //1=快速IO口，0普通IO口
#define USE_HORIZONTAL	0 //1=横屏显示，0=竖屏显示

#if LCD_FAST_IO==0
//慢速IO口
	#define LCD_BL	PCout(10) //LCD背光			 PC10
	#define	LCD_CS	PCout(9)  //片选端口  	     PC9
	#define	LCD_RS	PCout(8)  //数据/命令        PC8	   
	#define	LCD_WR	PCout(7)  //写数据			 PC7
	#define	LCD_RD	PCout(6)  //读数据			 PC6
#else
//快速IO口
	#define LCD_BL_SET	GPIOC->BSRR=1<<10 //LCD背光			 PC10
	#define	LCD_CS_SET	GPIOC->BSRR=1<<9  //片选端口  	     PC9
	#define	LCD_RS_SET	GPIOC->BSRR=1<<8  //数据/命令        PC8	   
	#define	LCD_WR_SET	GPIOC->BSRR=1<<7  //写数据			 PC7
	#define	LCD_RD_SET	GPIOC->BSRR=1<<6  //读数据			 PC6
	
	#define LCD_BL_CLR	GPIOC->BRR=1<<10 //LCD背光			 PC10
	#define	LCD_CS_CLR	GPIOC->BRR=1<<9  //片选端口  	     PC9
	#define	LCD_RS_CLR	GPIOC->BRR=1<<8  //数据/命令       	 PC8	   
	#define	LCD_WR_CLR	GPIOC->BRR=1<<7  //写数据			 PC7
	#define	LCD_RD_CLR	GPIOC->BRR=1<<6  //读数据			 PC6
#endif

//PB0~15,作为数据线
#define DATAOUT(x) GPIOB->ODR=x; //数据输出
#define DATAIN     GPIOB->IDR;   //数据输入
	  
#if	LCD_FAST_IO==1
	#define LCD_WR_DATA(data){LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	}
#else
	#define LCD_WR_DATA(data){LCD_RS=1;\
	LCD_CS=0;\
	DATAOUT(data);\
	LCD_WR=0;\
	LCD_WR=1;\
	LCD_CS=1;\
	}
#endif

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色

//#define MAX_CHAR_POSX 500	//可以显示字符的最大Y坐标
//#define MAX_CHAR_POSY 500 	//可以显示字符的最大X坐标
void LCD_Init(void);//lcd初始化
void LCD_WR_REG(u8 data);//写指令
void LCD_WriteReg(u8 LCD_Reg,u16 LCD_RegValue);//写寄存器/写GRAM通用

u16 LCD_RD_REG(void);//读寄存器数据
u16 LCD_RD_GRAM(u16 x,u16 y);//读GRAM
u16 LCD_ReadData(u8 LCD_Reg);//读寄存器

u16 LCD_ColorSet(u8 R,u8 G,u8 B);//RGB颜色设定
void LCD_DrawPoint(u16 x,u16 y);//打点
void LCD_Clear(u16 color);//清屏
void LCD_ShowChar(u16 x,u16 y,u8 onechar,u8 DispMode);//显示字符
void LCD_Fill_Color(u16 sx,u16 sy,u16 ex,u16 ey,u16 FillColor);
void LCD_ShowString(u16 x,u16 y,const u8 *chars,u8 DispMode);
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2);//画线函数
#endif 

