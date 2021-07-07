#include "sys.h"
#include "tft.h"
#include "delay.h"
//#include "usart.h"
#include "font.h"

u16 POINT_COLOR=0x0000,BACK_COLOR=0xFFFF;

//说明：写指令
//入口：data
//出口：无
#if LCD_FAST_IO==0
void LCD_WR_REG(u8 data)		//普通IO
{ 
	LCD_RS=0;//选择为索引/状态寄存器  
 	LCD_CS=0;//驱动IC被选中且可访问
	DATAOUT(data); 
	LCD_WR=0;//数据在WR上升沿被送入 
	LCD_WR=1; 
 	LCD_CS=1;   
}
#else
void LCD_WR_REG(u8 data)		//快速IO
{ 
	LCD_RS_CLR;//选择为索引/状态寄存器  
 	LCD_CS_CLR;//驱动IC被选中且可访问
	DATAOUT(data); 
	LCD_WR_CLR;//数据在WR上升沿被送入 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}
#endif



//说明：写寄存器
//入口：LCD_Reg(寄存器编号)
//出口：LCD_RegValue(要写入的值)
void LCD_WriteReg(u8 LCD_Reg,u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  			//先给出要写的地址
	LCD_WR_DATA(LCD_RegValue);	    //在写入数据
}


//作用:读寄存器数据
//入口:无
//出口：t(REG中的数据)
u16 LCD_RD_REG(void)
{										   
	u16 t;
 	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0X0000;     //全部输出0
#if LCD_FAST_IO==1
	LCD_RS_SET;
	LCD_CS_CLR;

	//读取数据(读寄存器时,并不需要读2次)
	LCD_RD_CLR;					   
	LCD_RD_SET;
	t=DATAIN;  
	LCD_CS_SET; 
#else
	LCD_RS=1;
	LCD_CS=0;

	//读取数据(读寄存器时,并不需要读2次)
	LCD_RD=0;					   
	LCD_RD=1;
	t=DATAIN;  
	LCD_CS=1; 
#endif  
	GPIOB->CRL=0X33333333; //PB0-7  上拉输出
	GPIOB->CRH=0X33333333; //PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    //全部输出高
	return t;  
}

//作用：读GRAM数据
//入口：无
//出口：t(GRAM中的值)
u16 LCD_RD_GRAM(u16 x,u16 y)
{
  	u16 t;
	LCD_WriteReg(0x0020,x);  //X
	LCD_WriteReg(0x0021,y);  //Y
	LCD_WR_REG(0x22);       //选择GRAM地址

 	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0XFFFF;     //全部输出0
#if LCD_FAST_IO==1
	LCD_RS_SET;
	LCD_CS_CLR;

	//读取数据(读寄存器时,并不需要读2次),空读一次
	LCD_RD_CLR;
	Delay_us(2);						   
	LCD_RD_SET;
	//再读一次
	LCD_RD_CLR;	
	Delay_us(2);					   
	LCD_RD_SET;				

	t=DATAIN;  
	LCD_CS_SET;
	 
#else
	LCD_RS=1;
	LCD_CS=0;

	//读取数据(读寄存器时,并不需要读2次)
	LCD_RD=0;					   
	LCD_RD=1;

	LCD_RD=0;					   
	LCD_RD=1;			//空读一次

	t=DATAIN;  
	LCD_CS=1; 
#endif  
	GPIOB->CRL=0X33333333; //PB0-7  上拉输出
	GPIOB->CRH=0X33333333; //PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    //全部输出高
	return t;
}

																			 
//说明：读寄存器
//入口：LCD_Reg
//出口：读到的值(LCD_RD_reg())
u16 LCD_ReadData(u8 LCD_Reg)
{										   
 	LCD_WR_REG(LCD_Reg);  //写入要读的寄存器号
	return LCD_RD_REG();  //返回REG值
}

//说明：LCD初始化
//入口：无
//出口：无
void LCD_Init(void)		//LCD初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;	   //GPIO_Pin_10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC	
    //GPIO_WriteBit(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6,Bit_SET);
	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
	//GPIO_WriteBit(GPIOC,GPIO_Pin_All,Bit_SET); 
	GPIO_SetBits(GPIOB,GPIO_Pin_All);

	//5341初始化
	LCD_WriteReg(0X00,0X0001);   
	Delay_ms(10);   
	LCD_WriteReg(0X10,0X1628);   
	LCD_WriteReg(0X12,0X000e);//0x0006    
	LCD_WriteReg(0X13,0X0A39);   
	Delay_ms(10);   
	LCD_WriteReg(0X11,0X0040);   
	LCD_WriteReg(0X15,0X0050);   
	Delay_ms(10);   
	LCD_WriteReg(0X12,0X001e);//16    
	Delay_ms(10);   
	LCD_WriteReg(0X10,0X1620);   
	LCD_WriteReg(0X13,0X2A39);   
	Delay_ms(10);   
	LCD_WriteReg(0X01,0X0100);   
	LCD_WriteReg(0X02,0X0300);   
	LCD_WriteReg(0X03,0X1030);//改变方向的   
	LCD_WriteReg(0X08,0X0202);   
	LCD_WriteReg(0X0A,0X0008);   
	LCD_WriteReg(0X30,0X0000);   
	LCD_WriteReg(0X31,0X0402);   
	LCD_WriteReg(0X32,0X0106);   
	LCD_WriteReg(0X33,0X0503);   
	LCD_WriteReg(0X34,0X0104);   
	LCD_WriteReg(0X35,0X0301);   
	LCD_WriteReg(0X36,0X0707);   
	LCD_WriteReg(0X37,0X0305);   
	LCD_WriteReg(0X38,0X0208);   
	LCD_WriteReg(0X39,0X0F0B);   
	LCD_WriteReg(0X41,0X0002);   
	LCD_WriteReg(0X60,0X2700);   
	LCD_WriteReg(0X61,0X0001);   
	LCD_WriteReg(0X90,0X0210);   
	LCD_WriteReg(0X92,0X010A);   
	LCD_WriteReg(0X93,0X0004);   
	LCD_WriteReg(0XA0,0X0100);   
	LCD_WriteReg(0X07,0X0001);   
	LCD_WriteReg(0X07,0X0021);   
	LCD_WriteReg(0X07,0X0023);   
	LCD_WriteReg(0X07,0X0033);   
	LCD_WriteReg(0X07,0X0133);   
	LCD_WriteReg(0XA0,0X0000); 
 
	LCD_Clear(WHITE);
}



//=============================================================================================================
//说明:指定输入的起始点，相当于设定光标位置
//x：横坐标(0-239)
//y：纵坐标(0-319)
void LCD_Setcursor(u16 x,u16 y)
{
	#if USE_HORIZONTAL==0  		//=0竖屏显示
	LCD_WriteReg(0x20,x);		//x轴
	LCD_WriteReg(0x21,y);		//y轴
	#else						//=1坐标为横屏
	LCD_WriteReg(0x20,y);		//x轴
	LCD_WriteReg(0x21,319-x);	//y轴
	#endif		
}




//说明：设置一种颜色
//		因为16的数据对应18位模式，RGB从高到底分别占6位，也就是左移0位，左移5位，左移1位
//入口：R、G、B
//出口：CRGB
u16 LCD_ColorSet(u8 R,u8 G,u8 B)
{
	u16 CRGB,RGB0,RGB1,RGB2,RGB3,RGB4;
	RGB0=B;

	RGB1=G;
	RGB2=RGB1<<5;

	RGB3=R;
	RGB4=RGB3<<11;//注意这里左移11位而不是10位，因为还有第九位没有被用到

	CRGB=RGB0|RGB2|RGB4;
	return CRGB;
}

//说明：清屏
//入口：BACK_COLOR,清屏色
//出口：无
void LCD_Clear(u16 Color)
{
	u16 i,j;
	LCD_WriteReg(0x20,0x00);//X轴，从第一点开始清
	LCD_WriteReg(0x21,0x00);//Y轴，从第一点开始清
	LCD_WR_REG(0x22);
	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
			LCD_WR_DATA(BACK_COLOR);//BACK_COLOR:背景颜色，全局变量
		}
	}
}

//说明：指定方块内填充某种颜色
//入口：sx：X轴起始位置，sy：Y轴起始位置，ex：X轴终止位置，sy：Y轴终止位置，FillColor：填充颜色
//出口：无
void LCD_Fill_Color(u16 sx,u16 sy,u16 ex,u16 ey,u16 FillColor)		//???????????????有问题，会多出一块
{
	//u16 ylen=ey-sy+1;
	u16 i,j;
	for(j=sy;j<=ey;j++)
	{
		LCD_WriteReg(0x20,sx);//X轴，从第一点开始清
		LCD_WriteReg(0x21,j);//Y轴，从第一点开始清
		LCD_WR_REG(0x22);		
		for(i=sx;i<=ex;i++)
		LCD_WR_DATA(FillColor);	
	}
}

//说明：画点
//入口：X,Y(点的坐标)，FRONT_COLOR(颜色)
//出口：
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_Setcursor(x,y);
	LCD_WR_REG(0x22);
	LCD_WR_DATA(POINT_COLOR);	//FRONT_COLOR:前景颜色(打点颜色)，全局变量	
}








//说明：显示一个字符
//		显示字符：有笔画的地方显示前景颜色，没笔画的地方显示背景色
//		背景分两种，全局背景和局部背景
//入口：x,y：显示起始位置，onchar：待显示的一个字符	 DispMode=1 叠加显示(和原来背景重叠)  DispMode=0 不叠加
//出口：LCD
void LCD_ShowChar(u16 x,u16 y,u8 onechar,u8 DispMode)  //叠加貌似有问题
{
#if USE_HORIZONTAL==1
	#define MAX_CHAR_POSX	319
	#define MAX_CHAR_POSY	239
#else
	#define MAX_CHAR_POSX	239
	#define MAX_CHAR_POSY	319
#endif

	u8 i,j;
	u16 char1,char2;
	u16 colortemp=POINT_COLOR;
//	if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;	
	char1=onechar-' ';
//	LCD_WriteReg(0x50,x);		//设定边框起始X坐标
//	LCD_WriteReg(0x51,x+7);		//设定边框的结束X坐标
//	LCD_WriteReg(0x52,y);   	//设定边框的起始Y坐标
//	LCD_WriteReg(0x53,y+15);	//设定边框的结束Y坐标
//	LCD_WriteReg(0x20,x);		//从X,Y开始显示
//	LCD_WriteReg(0x21,y);
//	LCD_Setcursor(x,y);

	LCD_WR_REG(0x22);
	if(DispMode)				//叠加模式
	{
		for(i=0;i<16;i++)
		{
			char2=asc2_1608[char1][i];	//取字模
			for(j=0;j<8;j++)
			{
				if(char2&0x01)
				{POINT_COLOR=colortemp;}
				else 
				{POINT_COLOR=LCD_RD_GRAM(x+j,y+i);}//注意要先读再写，如果在下一面语句直接嵌套会出现乱码，原因未找到	
				LCD_DrawPoint(x+j,y+i);
		 		char2>>=1;
			}
		}
	}
	else
	{
		for(i=0;i<16;i++)
		{
			char2=asc2_1608[char1][i];	//取字模
			for(j=0;j<8;j++)
			{
				if(char2&0x01)
				{POINT_COLOR=colortemp;}
				else 
				{POINT_COLOR=BACK_COLOR;}
				LCD_DrawPoint(x+j,y+i);
	 			char2>>=1;
			}
		}
	}
	POINT_COLOR=colortemp;	//防止退出时POINT_COLOR为背景色，下次调用时出错	  
}

//作用:显示字符串，最多能显示240/8个字符，多出的重头显示
//入口:x,y：显示的起始地址，*chars：字符串，DispMode：1 叠加显示(和原来背景重叠)，0=不叠加显示
//出口：t(REG中的数据)
void LCD_ShowString(u16 x,u16 y,const u8 *chars,u8 DispMode)
{
	while(*chars!='\0')						//判断是否字符串结束
	{
		if(x>MAX_CHAR_POSX)					//一行写满，清x，另起一行
		{x=0;y+=16;}
		if(y>MAX_CHAR_POSY)					  
		{
			y=x=0;							//Y坐标超出(写满屏)，从(0,0)开始继续显示
			LCD_Clear(WHITE);				//清屏					
		}							
		LCD_ShowChar(x,y,*chars,DispMode);	
		x+=8;								//调用的字符为16*8格式
		chars++;						   
	}
}



//作用：打点函数
//入口：X1：Y1，X2：Y2----起点横坐标：起点纵坐标，终点横坐标：终点纵坐标
//说明：Bresenham算法
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2)
{
	int err,dx,dy;
	int incx,incy,x,y;

	dx=x2-x1;
	dy=y2-y1;
	x=x1;
	y=y1;

	if(dx>0)incx=1;
	else if(dx==0) incx=0;
	else {incx=-1;dx=-dx;}

	if(dy>0)incy=1;
	else if(dy==0) incy=0;
	else {incy=-1;dy=-dy;}
	err=(dy<<1)-dx;		 			//初始判别式为e=2dy-dx

	if(dx>dy) 
	{
		for(x=x1;x!=x2;x+=incx)
		{
			LCD_DrawPoint(x,y);
			if(err>0)			  	//e=K-0.5  (K=dy/dx),即判断2dy-dx的符号
			{
				y+=incy;
				err-=(dx<<1);		//e=e-1
			}
			//x+=incx;
			err+=(dy<<1);			//e=e+K  (K=dy/dx)
		}
	}
	else
	{
		for(y=y1;y!=y2;y+=incy)
		{
			LCD_DrawPoint(x,y);
			if(err>0)
			{
				x+=incx;
				err-=(dy<<1);	
			}				
			//y+=incy;
			err+=(dx<<1);	
		}

	}


}











