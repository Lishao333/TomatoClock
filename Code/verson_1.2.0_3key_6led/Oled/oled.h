//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : Evk123
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : 0.69寸OLED 接口演示例程(STM32F103ZE系列IIC)
//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PB13（SCL）
//              SDA   接PB15（SDA） 
//              RES   接PB11 如果是用4针iic接口这个脚可以不接
//              ----------------------------------------------------------------
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#ifndef __OLED_H
#define __OLED_H			

#include <stdio.h>	   

#include "stdlib.h"	   
#include "gd32f10x_it.h" 
#include "gd32f10x_gpio.h"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    		

//-----------------OLED端口定义---------------- 
#define OLED_RCU        RCU_GPIOB
#define OLED_PORT       GPIOB					   
#define IIC_SCL_PIN     GPIO_PIN_13  					   
#define IIC_SDA_PIN     GPIO_PIN_15				   
#define DC_PIN          GPIO_PIN_10				   
#define RES_PIN         GPIO_PIN_11	   
#define CS_PIN          GPIO_PIN_12
#define OLED_IIC_PIN    IIC_SCL_PIN | IIC_SDA_PIN 

#define OLED_SCLK_Clr() gpio_bit_reset(OLED_PORT,IIC_SCL_PIN)//CLK
#define OLED_SCLK_Set() gpio_bit_set(OLED_PORT,IIC_SCL_PIN)

#define OLED_SDIN_Clr() gpio_bit_reset(OLED_PORT,IIC_SDA_PIN)//DIN
#define OLED_SDIN_Set() gpio_bit_set(OLED_PORT,IIC_SDA_PIN)

#define OLED_RST_Clr() gpio_bit_reset(OLED_PORT,RES_PIN)//RES
#define OLED_RST_Set() gpio_bit_set(OLED_PORT,RES_PIN)

#define OLED_DC_Clr() gpio_bit_reset(OLED_PORT,DC_PIN)//DC
#define OLED_DC_Set() gpio_bit_set(OLED_PORT,DC_PIN)
 		     
#define OLED_CS_Clr()  gpio_bit_reset(OLED_PORT,CS_PIN)//CS
#define OLED_CS_Set()  gpio_bit_set(OLED_PORT,CS_PIN)

#define OLED_IIC_INIT() { \
    /* enable the led clock */ \
    rcu_periph_clock_enable(OLED_RCU); \
    /* configure led GPIO port */ \
    gpio_init(OLED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, IIC_SCL_PIN); \
    gpio_init(OLED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, IIC_SDA_PIN); \
}

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define u8 unsigned char
#define u32 unsigned int

//x:0~127
//y:0~63
#define X_MAX_VALUE                127
#define Y_MAX_VALUE                63
#define UNIT_BASE_SIZE_WIDTH       2
#define UNIT_BASE_SIZE_HEIGHT      2
#define CHAR_BASE_SIZE_WIDTH       (UNIT_BASE_SIZE_WIDTH)     //2
#define CHAR_BASE_SIZE_HEIGHT      (UNIT_BASE_SIZE_HEIGHT*2)  //4
#define CHAR_BASE_SIZE             (UNIT_BASE_SIZE_HEIGHT*2)  //4

typedef enum{
	CHAR_SIZE_NUMBER_1 = 1,   
	CHAR_SIZE_NUMBER_2,   
	CHAR_SIZE_NUMBER_3,         
	CHAR_SIZE_NUMBER_4,         
	CHAR_SIZE_NUMBER_5,         
	CHAR_SIZE_NUMBER_6,         
}enuCHAR_SIZE_NUMBER;

#define CHAR_612_SIZE_WIDTH        (CHAR_BASE_SIZE_WIDTH *CHAR_SIZE_NUMBER_3)  //6
#define CHAR_612_SIZE_HEIGHT       (CHAR_BASE_SIZE_HEIGHT*CHAR_SIZE_NUMBER_3)  //12
#define CHAR_816_SIZE_WIDTH        (CHAR_BASE_SIZE_WIDTH *CHAR_SIZE_NUMBER_4)  //8
#define CHAR_816_SIZE_HEIGHT       (CHAR_BASE_SIZE_HEIGHT*CHAR_SIZE_NUMBER_4)  //16
#define CHAR_1224_SIZE_WIDTH       (CHAR_BASE_SIZE_WIDTH *CHAR_SIZE_NUMBER_6)  //12
#define CHAR_1224_SIZE_HEIGHT      (CHAR_BASE_SIZE_HEIGHT*CHAR_SIZE_NUMBER_6)  //24

#define CHAR_DEFAULT_SIZE_NUMBER   CHAR_SIZE_NUMBER_4
#define CHAR_DEFAULT_SIZE_WIDTH    CHAR_816_SIZE_WIDTH
#define CHAR_DEFAULT_SIZE_HEIGHT   CHAR_816_SIZE_HEIGHT
#define CHAR_DEFAULT_SIZE          CHAR_BASE_SIZE_HEIGHT*CHAR_SIZE_NUMBER_4

#define CHAR_DISPLAY_MAX_NUM       ((X_MAX_VALUE/CHAR_612_SIZE_WIDTH) * (Y_MAX_VALUE/CHAR_612_SIZE_HEIGHT)) //21*5=105

typedef enum{
	LINE_612_0,
	LINE_612_1,
	LINE_612_2,
	LINE_612_3,
	LINE_612_4,
	LINE_612_REMAIN = Y_MAX_VALUE-(CHAR_612_SIZE_HEIGHT*(LINE_612_4+1)),
}enuLINE_612;

typedef enum{
	ROW_612_0,
	ROW_612_1,
	ROW_612_2,
	ROW_612_3,
	ROW_612_4,
	ROW_612_5,
	ROW_612_6,
	ROW_612_7,
	ROW_612_8,
	ROW_612_9,
	ROW_612_10,
	ROW_612_11,
	ROW_612_12,
	ROW_612_13,
	ROW_612_14,
	ROW_612_15,
	ROW_612_16,
	ROW_612_17,
	ROW_612_18,
	ROW_612_19,
	ROW_612_20,
	ROW_612_REMAIN = X_MAX_VALUE-(CHAR_612_SIZE_WIDTH*(ROW_612_20+1)),
}enuROW_612;

typedef enum{
	LINE_816_0,
	LINE_816_1,
	LINE_816_2,
	LINE_816_3,
	LINE_816_REMAIN = Y_MAX_VALUE-(CHAR_816_SIZE_HEIGHT*(LINE_816_3+1)),
}enuLINE_816;

typedef enum{
	ROW_816_0,
	ROW_816_1,
	ROW_816_2,
	ROW_816_3,
	ROW_816_4,
	ROW_816_5,
	ROW_816_7,
	ROW_816_8,
	ROW_816_9,
	ROW_816_10,
	ROW_816_11,
	ROW_816_12,
	ROW_816_13,
	ROW_816_14,
	ROW_816_REMAIN = X_MAX_VALUE-(CHAR_816_SIZE_WIDTH*(ROW_816_14+1)),
}enuROW_816;

typedef enum{
	LINE_1224_0,
	LINE_1224_1,
	LINE_1224_REMAIN = Y_MAX_VALUE-(CHAR_1224_SIZE_HEIGHT*(LINE_1224_1+1)),
}enuLINE_1224;

typedef enum{
	ROW_1224_0,
	ROW_1224_1,
	ROW_1224_2,
	ROW_1224_3,
	ROW_1224_4,
	ROW_1224_5,
	ROW_1224_6,
	ROW_1224_7,
	ROW_1224_8,
	ROW_1224_9,
	ROW_1224_REMAIN = X_MAX_VALUE-(CHAR_1224_SIZE_WIDTH*(ROW_1224_9+1)),
}enuROW_1224;

typedef struct {
	uint8_t x;
	uint8_t y;
}stPointInfo;

typedef struct {
	uint8_t distance_x;
	uint8_t distance_y;
}stDistance;

typedef struct {
	uint8_t height;
	uint8_t width;
	uint8_t size_number;
}stCharSizeSetInfo;

typedef struct {
	stPointInfo point;
	uint8_t size_number;
	stDistance distanse_to_start;
	stDistance distanse_to_end;
}stCursorInfo;

typedef struct {
	unsigned char character[1];
	uint8_t width;
	uint8_t height;
	uint8_t size_number;
	uint8_t idx;
	stPointInfo point;
}stShowCharInfo;

typedef struct {
	stShowCharInfo show_char_info[CHAR_DISPLAY_MAX_NUM];
	uint8_t count;
}stShowStrInfo;	

void OLED_ShowString_612 (int line, int row, unsigned char* str, int len);
void OLED_ClearAll ();

void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1);
void OLED_ScrollDisplay(u8 num,u8 space);
void OLED_WR_BP(u8 x,u8 y);
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);
void OLED_Init(void);
void OLED_Test (void);

/** add by LiShaolin *******************************/
// Character size set info ///////////////////////////////////////////////////////////
extern stCharSizeSetInfo g_char_sizes_set_info;

//caculator char size
int GetCharSize (int size_number);

//only could use character size(3, 4, 6)
int SetCharSizeSetInfo (stCharSizeSetInfo *set, enuCHAR_SIZE_NUMBER char_size);

void DefaultSetCharSizeSetInfo ();

stCharSizeSetInfo* GetCharSizeSetInfo ();

int GetCharSizeSetInfo_SizeNumber ();

int GetCharSizeSetInfo_Hight ();

int GetCharSizeSetInfo_Width ();
/////////////////////////////////////////////////////////////////////////////////


// cursor info //////////////////////////////////////////////////////////////////
extern stCursorInfo g_cursor_info;

int ReflashCursorInfo (stPointInfo point);

int ResetCursorInfo ();

//更新光标信息xy	
int ReflashCursorInfo_PointInfo (stShowCharInfo *last_show_char_info);

//光标换行
int CursorLineFeed (uint8_t height);

stCursorInfo* GetCursorInfo ();

int GetCursorInfo_Size ();

int GetCursorInfo_Point_X ();

int GetCursorInfo_Point_Y ();

int GetCursorInfo_DistanceToStart_DistanceX ();

int GetCursorInfo_DistanceToStart_DistanceY ();

int GetCursorInfo_DistanceToEnd_DistanceX ();

int GetCursorInfo_DistanceToEnd_DistanceY ();
/////////////////////////////////////////////////////////////////////////////////


// OLED show string control /////////////////////////////////////////////////////
extern stShowStrInfo g_show_str_info;

//更新显示的内容的信息
int ReflashShowStrInfo (unsigned char* str, int len);

void ClearAllShowStrInfo ();

//遍历显示字符串的所有显示字节的显示信息
void PrintfShowStrInfo ();

void OLED_ShowStringControl (char* str, int len);

void OLED_ClearAll ();

void OLED_BackSpace ();

void OLED_ShowStringProcess ();
/////////////////////////////////////////////////////////////////////////////////

#endif  
	 