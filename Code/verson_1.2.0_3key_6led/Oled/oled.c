#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "bmp.h"  	 
#include "delay.h"
#include "iic.h"
#include "led.h"

#define OLED_DEBUG_ENABLE    1
#if OLED_DEBUG_ENABLE
	#define OLED_DBG(format, ...)    printf(format, ##__VA_ARGS__)
#else
	#define OLED_DBG(format, ...)
#endif

u8 OLED_GRAM[144][8];

// IIC driver API //////////////////////////////////////////////////////////////////////////
//起始信号
void I2C_Start(void)
{
	iic_start();
}

//结束信号
void I2C_Stop(void)
{
	iic_stop();
}

//等待信号响应
void I2C_WaitAck(void) //测数据信号的电平
{
	iic_wait_ack();
}

//写入一个字节
void Send_Byte(u8 dat)
{
	iic_write_byte(dat);
}
/////////////////////////////////////////////////////////////////////////////////


// OLED12864 driver API /////////////////////////////////////////////////////////
//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 mode)
{
	I2C_Start();
	Send_Byte(0x78);
	I2C_WaitAck();
	if(mode){Send_Byte(0x40);}
	else{Send_Byte(0x00);}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}

//反显函数
void OLED_ColorTurn(u8 i)
{
	if(i==0)
	{
		OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
	}
	if(i==1)
	{
		OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
	}
}

//屏幕旋转180度
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
	{
		OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
		OLED_WR_Byte(0xA1,OLED_CMD);
	}
	if(i==1)
	{
		OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
		OLED_WR_Byte(0xA0,OLED_CMD);
	}
}

//开启OLED显示 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
}

//关闭OLED显示 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//关闭屏幕
}

//更新显存到OLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
	   OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
	   OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
	   for(n=0;n<128;n++)
		 OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}
//清屏函数
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
		{
		 OLED_GRAM[n][i]=0;//清除所有数据
		}
  }
	OLED_Refresh();//更新显示
}

//画点 
//x:0~127
//y:0~63
void OLED_DrawPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]|=n;
}

//清除一个点
//x:0~127
//y:0~63
void OLED_ClearPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	OLED_GRAM[x][i]|=n;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}


//画线
//x:0~128
//y:0~64
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2)
{
	u8 i,k,k1,k2,y0;
	if((x1<0)||(x2>128)||(y1<0)||(y2>64)||(x1>x2)||(y1>y2))return;
	if(x1==x2)    //画竖线
	{
		for(i=0;i<(y2-y1);i++)
		{
			OLED_DrawPoint(x1,y1+i);
		}
  }
	else if(y1==y2)   //画横线
	{
		for(i=0;i<(x2-x1);i++)
		{
			OLED_DrawPoint(x1+i,y1);
		}
  }
	else      //画斜线
	{
		k1=y2-y1;
		k2=x2-x1;
		k=k1*10/k2;
		for(i=0;i<(x2-x1);i++)
		{
		  OLED_DrawPoint(x1+i,y1+i*k/10);
		}
	}
}
//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);
 
        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size:选择字体 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1)
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
			if(temp&0x80)OLED_DrawPoint(x,y);
			else OLED_ClearPoint(x,y);
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


//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_ShowChar(x,y,*chr,size1);
		x+=(size1*CHAR_BASE_SIZE_WIDTH)/2; //modify by lishaolin 
		if(x>128-size1)  //换行
		{
			x=0;
			y+=2;
        }
		chr++;
    }
}

//m^n
u32 OLED_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

////显示2个数字
////x,y :起点坐标	 
////len :数字的位数
////size:字体大小
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1)
{
	u8 t,temp;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
		if(temp==0)
		{
			OLED_ShowChar(x+(size1/2)*t,y,'0',size1);
        }
		else 
		{
		  OLED_ShowChar(x+(size1/2)*t,y,temp+'0',size1);
		}
    }
}

//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1)
{
#if 0
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
					{temp=Hzk1[chr1][i];}//调用16*16字体
			else if(size1==24)
					{temp=Hzk2[chr1][i];}//调用24*24字体
			else if(size1==32)       
					{temp=Hzk3[chr1][i];}//调用32*32字体
			else if(size1==64)
					{temp=Hzk4[chr1][i];}//调用64*64字体
			else return;
						
			for(m=0;m<8;m++)
			{
				if(temp&0x01)OLED_DrawPoint(x,y);
				else OLED_ClearPoint(x,y);
				temp>>=1;
				y++;
			}
			x++;
			if((x-x0)==size1)
			{x=x0;y0=y0+8;}
			y=y0;
		 }
	}
#endif
}

//num 显示汉字的个数
//space 每一遍显示的间隔
void OLED_ScrollDisplay(u8 num,u8 space)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
			OLED_ShowChinese(128,24,t,16); //写入一个汉字保存在OLED_GRAM[][]数组中
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
						OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
					}
				}
				OLED_Refresh();
			}
			t=0;
        }
		m++;
		if(m==16){m=0;}
		for(i=0;i<144;i++)   //实现左移
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

//配置写入数据的起始位置
void OLED_WR_BP(u8 x,u8 y)
{
	OLED_WR_Byte(0xb0+y,OLED_CMD);//设置行起始地址
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
}

//x0,y0：起点坐标
//x1,y1：终点坐标
//BMP[]：要写入的图片数组
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[])
{
	u32 j=0;
	u8 x=0,y=0;
	if(y%8==0)y=0;
	else y+=1;
	for(y=y0;y<y1;y++)
	 {
		 OLED_WR_BP(x0,y);
		 for(x=x0;x<x1;x++)
		 {
			 OLED_WR_Byte(BMP[j],OLED_DATA);
			 j++;
		 }
	 }
}
//OLED的初始化
void OLED_Init(void)
{
	OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();//复位
	delay_ms(200);
	OLED_RST_Set();
	OLED_DC_Clr();
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);
	OLED_Clear();
}

void OLED_Test (void)
{ 
	u8 t;
	
	OLED_ColorTurn(0);//0正常显示，1 反色显示
	OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
//	OLED_DrawPoint(0,0);
//	OLED_DrawLine(20,0,50,60);
//	OLED_DrawCircle(64,32,20);
	OLED_Refresh();
	t=' ';
	LED_ON;
	while(1)
	{
		//OLED_ShowPicture(0,0,128,8,BMP1);
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16);//中
		OLED_ShowChinese(18,0,1,16);//景
		OLED_ShowChinese(36,0,2,16);//园
		OLED_ShowChinese(54,0,3,16);//电
		OLED_ShowChinese(72,0,4,16);//子
		OLED_ShowChinese(90,0,5,16);//科
		OLED_ShowChinese(108,0,6,16);//技
		OLED_ShowString(8,16,"ZHONGJINGYUAN",16);
		OLED_ShowString(20,32,"2014/05/01",16);
		OLED_ShowString(0,48,"ASCII:",16);  
		OLED_ShowString(63,48,"CODE:",16);
		OLED_ShowChar(48,48,t,16);//显示ASCII字符	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,48,t,3,16);
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16);  //16*16 中
		OLED_ShowChinese(16,0,0,24); //24*24 中
		OLED_ShowChinese(24,20,0,32);//32*32 中
		OLED_ShowChinese(64,0,0,64); //64*64 中
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
		OLED_ShowString(0,0,"ABC",12);//6*12 “ABC”
		OLED_ShowString(0,12,"ABC",16);//8*16 “ABC”
		OLED_ShowString(0,28,"ABC",24);//12*24 “ABC”
		OLED_Refresh();
		delay_ms(500);
		OLED_ScrollDisplay(11,4);
	}
}
/////////////////////////////////////////////////////////////////////////////////

#if 1
// Character size set info ///////////////////////////////////////////////////////////
stCharSizeSetInfo g_char_sizes_set_info = {0};

//caculator char size
int GetCharSize (int size_number)
{
	int char_size = CHAR_BASE_SIZE_HEIGHT*size_number;
	
	return char_size;
}

//only could use character size(3, 4, 6)
int SetCharSizeSetInfo (stCharSizeSetInfo *set, enuCHAR_SIZE_NUMBER char_size)
{
	set->size_number = char_size;
	if (set->size_number == CHAR_SIZE_NUMBER_3
			|| set->size_number == CHAR_SIZE_NUMBER_4
			|| set->size_number == CHAR_SIZE_NUMBER_6)
	{
		set->width  = CHAR_BASE_SIZE_WIDTH*set->size_number;
		set->height = CHAR_BASE_SIZE_HEIGHT*set->size_number;		
		return char_size;
	}
	
	return 0;
}

void DefaultSetCharSizeSetInfo ()
{
	SetCharSizeSetInfo (&g_char_sizes_set_info, CHAR_DEFAULT_SIZE_NUMBER);
}

stCharSizeSetInfo* GetCharSizeSetInfo ()
{
	return &g_char_sizes_set_info;
}

int GetCharSizeSetInfo_SizeNumber ()
{
	return g_char_sizes_set_info.size_number;
}

int GetCharSizeSetInfo_Hight ()
{
	return g_char_sizes_set_info.height;
}

int GetCharSizeSetInfo_Width ()
{
	return g_char_sizes_set_info.width;
}
/////////////////////////////////////////////////////////////////////////////////
#endif
#if 1
// cursor info //////////////////////////////////////////////////////////////////
stCursorInfo g_cursor_info = {0};

int ReflashCursorInfo (stPointInfo point)
{
	//光标移到下一个字节起始位置
	g_cursor_info.point.x += point.x+GetCharSizeSetInfo_Width();
	g_cursor_info.point.y += point.y+GetCharSizeSetInfo_Hight();
	g_cursor_info.size_number = GetCharSizeSetInfo_SizeNumber();
	g_cursor_info.distanse_to_start.distance_x = g_cursor_info.point.x;
	g_cursor_info.distanse_to_start.distance_y = g_cursor_info.point.y;
	g_cursor_info.distanse_to_end.distance_x = X_MAX_VALUE - g_cursor_info.point.x;
	g_cursor_info.distanse_to_end.distance_y = Y_MAX_VALUE - g_cursor_info.point.y;
	
	return 0;
}

int ResetCursorInfo ()
{
	memset(&g_cursor_info, 0, sizeof(g_cursor_info));
	g_cursor_info.size_number = GetCharSizeSetInfo_SizeNumber();
	
	return 0;
}

//光标换行
int CursorLineFeed (uint8_t height)
{
	//下一个待显示字节从下一行的起始处开始显示
	g_cursor_info.point.x = 0;
	
	//并且更新下一个待显示字节的坐标y
	if (g_cursor_info.point.y + height <= Y_MAX_VALUE) //是否需要从第一行开始
	{
		//换到下一行
		g_cursor_info.point.y += height; //换行，同时y改变;
		OLED_DBG ("CursorLineFeed(): Go to next line\r\n");
		
		return 1;
	}
	else
	{
		//从第一行开始
		g_cursor_info.point.y = 0;
		OLED_DBG ("CursorLineFeed(): Go to first line\r\n");
		
		return 0;
	}
	
	return -1;
}

//更新光标信息xy	
int ReflashCursorInfo_PointInfo (stShowCharInfo *last_show_char_info)
{
	//获取最后一个显示字节的位置信息
	g_cursor_info.point.x = last_show_char_info->point.x;
	g_cursor_info.point.y = last_show_char_info->point.y;
	
	//更新下一个待显示字节的位置信息
	//如果是第一行第一个
	if (g_cursor_info.point.x==0 && g_cursor_info.point.y==0) 
	{
		//下一个待显示字节在当前行内继续显示
		g_cursor_info.point.x += last_show_char_info->width; //同一行，y不变
	}
	else
	{
		//更新下一个待显示字节的坐标x
		if (g_cursor_info.point.x + last_show_char_info->width <= X_MAX_VALUE) //是否需要换行
		{
			//下一个待显示字节在当前行内继续显示
			g_cursor_info.point.x += last_show_char_info->width; //同一行，y不变
		}
		else
		{
			//光标换行
			CursorLineFeed (last_show_char_info->height);
		}
	}

	//更新distance
	g_cursor_info.distanse_to_start.distance_x = g_cursor_info.point.x;
	g_cursor_info.distanse_to_start.distance_y = g_cursor_info.point.y;
	g_cursor_info.distanse_to_end.distance_x = X_MAX_VALUE - g_cursor_info.point.x;
	g_cursor_info.distanse_to_end.distance_y = Y_MAX_VALUE - g_cursor_info.point.y;	
	
	return 0;
}

//检查屏幕显示是否已满
int CheckScreenDisplayIsFull (uint8_t width ,uint8_t height)
{
	if (g_cursor_info.distanse_to_end.distance_x < width
		&& g_cursor_info.distanse_to_end.distance_y < height
		&& g_cursor_info.distanse_to_start.distance_x > 0
		&& g_cursor_info.distanse_to_start.distance_y > 0)
	{
		printf("The screen is full, Please clear all!!!\r\n");
		return 1;
	}
	
	return 0;
}

stCursorInfo* GetCursorInfo ()
{
	return &g_cursor_info;
}

int GetCursorInfo_Size ()
{
	return g_cursor_info.size_number;
}

int GetCursorInfo_Point_X ()
{
	return g_cursor_info.point.x;
}

int GetCursorInfo_Point_Y ()
{
	return g_cursor_info.point.y;
}	

int GetCursorInfo_DistanceToStart_DistanceX ()
{
	return g_cursor_info.distanse_to_start.distance_x;
}

int GetCursorInfo_DistanceToStart_DistanceY ()
{
	return g_cursor_info.distanse_to_start.distance_y;
}

int GetCursorInfo_DistanceToEnd_DistanceX ()
{
	return g_cursor_info.distanse_to_end.distance_x;
}

int GetCursorInfo_DistanceToEnd_DistanceY ()
{
	return g_cursor_info.distanse_to_end.distance_y;
}
/////////////////////////////////////////////////////////////////////////////////
#endif
#if 1
// OLED show string control /////////////////////////////////////////////////////
stShowStrInfo g_show_str_info = {0};

//更新显示的内容的信息
int ReflashShowStrInfo (unsigned char* str, int len)
{
	int i=0;
	stCursorInfo* cursor_info = GetCursorInfo (); //获取最新的光标信息
	int start_pos = g_show_str_info.count; //从之前显示内容的最后一个字节的下一个字节开始
	int end_pos = g_show_str_info.count+len; 
	
	//check str and len
	if (!str || !len)
	{
		return -1;
	}
	
	//设置每一个需要显示的字节的信息
	for (i=0; i<len; i++)
	{
		//检查是否需要换行显示
		if (str[i] == '\n')
		{
			//光标换行
			CursorLineFeed (GetCharSizeSetInfo_Hight());
			
			//跳过本字节
			continue;
		}
		
		//检查屏幕显示是否已满
		if (CheckScreenDisplayIsFull ( GetCharSizeSetInfo_Width() ,GetCharSizeSetInfo_Hight()))
		{
			return i;
		}
		
		//设置每一个需要显示的字节的编号
		g_show_str_info.show_char_info[i].idx = i;
		
		//设置每一个需要显示的字节的内容、大小
		g_show_str_info.show_char_info[i].character[0] = str[i];
		g_show_str_info.show_char_info[i].size_number = GetCharSizeSetInfo_SizeNumber();
		g_show_str_info.show_char_info[i].height = GetCharSizeSetInfo_Hight();
		g_show_str_info.show_char_info[i].width = GetCharSizeSetInfo_Width();
		
		//设置每一个需要显示的字节的位置信息
		g_show_str_info.show_char_info[i].point.x = cursor_info->point.x;
		g_show_str_info.show_char_info[i].point.y = cursor_info->point.y;
		
		//更新光标信息xy	
		ReflashCursorInfo_PointInfo (&g_show_str_info.show_char_info[i]);
	}
	
	//设置所有需要显示的字节的总长度
	g_show_str_info.count += len;
	
	return i;
}

void ClearAllShowStrInfo ()
{
	memset(&g_show_str_info, 0, sizeof(g_show_str_info));
}

//遍历显示字符串的所有显示字节的显示信息
void PrintfShowStrInfo ()
{
	int i = 0;
	
	printf("PrintfShowStrInfo():****************** start ********************\r\n");
	printf("g_show_str_info.count:%d;\r\n", g_show_str_info.count);
	for (i=0; i<sizeof(g_show_str_info.show_char_info)/sizeof(stShowCharInfo); i++)
	{
		printf("g_show_str_info.show_char_info[%d].character:%c;\r\n", i, g_show_str_info.show_char_info[i].character[0]);
		printf("g_show_str_info.show_char_info[%d].width    :%d;\r\n", i, g_show_str_info.show_char_info[i].width);
		printf("g_show_str_info.show_char_info[%d].height   :%d;\r\n", i, g_show_str_info.show_char_info[i].height);
		printf("g_show_str_info.show_char_info[%d].width    :%d;\r\n", i, g_show_str_info.show_char_info[i].width);
		printf("g_show_str_info.show_char_info[%d].point.x  :%d;\r\n", i, g_show_str_info.show_char_info[i].point.x);
		printf("g_show_str_info.show_char_info[%d].point.y  :%d;\r\n", i, g_show_str_info.show_char_info[i].point.y);
		printf("g_show_str_info.show_char_info[%d].idx      :%d;\r\n", i, g_show_str_info.show_char_info[i].idx);
	}
	printf("PrintfShowStrInfo():****************** end ********************\r\n");
}
#endif
#if 1
void show_test ()
{
	unsigned char c = 'A';
	//OLED_ShowString(8,16,"Lishaolin",16);
	OLED_ShowChar(48,48,c++,16);//显示ASCII字符
	OLED_Refresh();
}
void OLED_ShowStringControl (char* str, int len)
{	
	int i=0;

	//更新显示的内容的信息
	int ret = ReflashShowStrInfo ((unsigned char*)str, len);
	
	//遍历显示字符串的所有显示字节的显示信息
	//PrintfShowStrInfo ();
	
	//填充内容到缓冲区，并刷新显示屏
	for (i=0; i<g_show_str_info.count; i++)
	{
		OLED_ShowChar(g_show_str_info.show_char_info[i].point.x, 
					  g_show_str_info.show_char_info[i].point.y, 
					  g_show_str_info.show_char_info[i].character[0], 
					  GetCharSize (g_show_str_info.show_char_info[i].size_number));
	}
	OLED_Refresh();
}

void OLED_ClearAllShowStringInfo ()
{
	ResetCursorInfo ();
	ClearAllShowStrInfo ();
}

void OLED_BackSpace ()
{
	int i = 0;
	
	stShowCharInfo *p_show_char_info = &g_show_str_info.show_char_info[--g_show_str_info.count];
	memset (p_show_char_info, 0, sizeof(stShowCharInfo));
		
	//更新光标信息xy	
	ReflashCursorInfo_PointInfo (--p_show_char_info);
	
	//遍历显示字符串的所有显示字节的显示信息
	//PrintfShowStrInfo ();
	
	//填充内容到缓冲区，并刷新显示屏
	OLED_Clear();
	for (i=0; i<g_show_str_info.count; i++)
	{
		OLED_ShowChar(g_show_str_info.show_char_info[i].point.x, 
					  g_show_str_info.show_char_info[i].point.y, 
					  g_show_str_info.show_char_info[i].character[0], 
					  GetCharSize (g_show_str_info.show_char_info[i].size_number));
	}
	OLED_Refresh();
	
	OLED_DBG("OLED_BackSpace ()\r\n");
}

void OLED_ShowStringProcess ()
{
	uint32_t remian_sec = get_led_work_remain_s ()%60;
	uint8_t remain_min = get_led_work_remain_s ()/60;
	uint8_t remain_hour = remain_min/60;
	char remaine_work_time[20] = {0};

	sprintf(remaine_work_time, "Remain Work Time:\n    %02d:%02d:%02d", remain_hour, remain_min, remian_sec);
	
	OLED_ClearAllShowStringInfo ();
	OLED_ShowStringControl (remaine_work_time, strlen(remaine_work_time));
}
/////////////////////////////////////////////////////////////////////////////////
#endif


























