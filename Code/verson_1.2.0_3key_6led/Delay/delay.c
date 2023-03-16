/************************************************************
Copyright (C), 2013-2022
FileName: Delay.h
Author  : 祥子  QQ:570525287
Version : 1.0
Date    : 2022-1-5
Description: 
Function List:
History    : 
<author> <time> <version > <desc>
***********************************************************/

#define USE_DELAY_TIMER	1

//======================使用定时器做延时===========================
#if(USE_DELAY_TIMER)

#include "gd32f10x_timer.h"
#include <stdlib.h>

#define DELAY_TIMER				TIMER6
#define RCU_DELAY_TIMER			RCU_TIMER6
#define DELAY_TIMER_IRQHandler	TIMER6_IRQHandler
#define DELAY_TIMER_IRQn		TIMER6_IRQn

//定时器不分频
#define DELAY_TIMER_PRESCALER	0	//分频值0,频率和系统时钟一样
//1us的定时计数是
#define DELAY_TIMER_PERIOD		(SystemCoreClock / 1000 /1000)-1	//71	//SystemCoreClock / 1000 /1000

#define TMR_ENABLE()	TIMER_CTL0(DELAY_TIMER)|=(uint32_t)TIMER_CTL0_CEN	//开启定时器
#define TMR_DISABEL()	TIMER_CTL0(DELAY_TIMER) &= ~(uint32_t)TIMER_CTL0_CEN //关闭定时器
#define TMR_CLR_FLAG()	TIMER_INTF(DELAY_TIMER) = (~(uint32_t)TIMER_INT_FLAG_UP)	//清除中断标志位
#define TMR_SET_CNT(X)	TIMER_CNT(DELAY_TIMER) = (uint32_t)(X)	//配置计数器

uint16_t us_count;

/*----------------------------------------------------------------------------------
Function   :Delay_Timer_Init
Description:初始化延时函数定时器
Input      :无
Retrun     :无
----------------------------------------------------------------------------------*/
void delay_init(void)
{
	timer_parameter_struct tmr;

	rcu_periph_clock_enable(RCU_DELAY_TIMER);
    timer_deinit(DELAY_TIMER);

	 /* TIMERx configuration */
    tmr.prescaler         = DELAY_TIMER_PRESCALER;
    tmr.alignedmode       = TIMER_COUNTER_EDGE;
    tmr.counterdirection  = TIMER_COUNTER_UP;
    tmr.period            = DELAY_TIMER_PERIOD;
    tmr.clockdivision     = TIMER_CKDIV_DIV1;
    tmr.repetitioncounter = 0;
    timer_init(DELAY_TIMER,&tmr);
	
    timer_interrupt_enable(DELAY_TIMER,TIMER_INT_UP); //使能更新中断
	nvic_irq_enable(DELAY_TIMER_IRQn,2,0); //使能中断线
	TMR_DISABEL();
}


/*----------------------------------------------------------------------------------
Function   :DELAY_TIMER_IRQHandler
Description:定时器中断
Input      :无
Retrun     :无
----------------------------------------------------------------------------------*/
void DELAY_TIMER_IRQHandler(void)
{
    //timer_interrupt_flag_clear(DELAY_TIMER, TIMER_INT_FLAG_UP); //清除中断标志位
	TMR_CLR_FLAG();
    us_count++;
}


/*----------------------------------------------------------------------------------
Function   :Delay_1us
Description:延时us
Input      :count:微秒数
Retrun     :无
----------------------------------------------------------------------------------*/
void delay_us(uint16_t count)
{
	TMR_SET_CNT(0);	//timer_counter_value_config(DELAY_TIMER,0);	
	TMR_ENABLE();	//timer_enable
	us_count = 0;		
	while (us_count < count);
	TMR_DISABEL();	//timer_disable
}

void delay_ms(uint16_t count)
{
   while (count--)
   {
	   delay_us(1000);	//为了避开中断
   }          
}
#endif