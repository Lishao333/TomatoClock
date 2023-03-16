/************************************************************
Copyright (C), 2013-2022
FileName: Delay.h
Author  : ����  QQ:570525287
Version : 1.0
Date    : 2022-1-5
Description: 
Function List:
History    : 
<author> <time> <version > <desc>
***********************************************************/

#define USE_DELAY_TIMER	1

//======================ʹ�ö�ʱ������ʱ===========================
#if(USE_DELAY_TIMER)

#include "gd32f10x_timer.h"
#include <stdlib.h>

#define DELAY_TIMER				TIMER6
#define RCU_DELAY_TIMER			RCU_TIMER6
#define DELAY_TIMER_IRQHandler	TIMER6_IRQHandler
#define DELAY_TIMER_IRQn		TIMER6_IRQn

//��ʱ������Ƶ
#define DELAY_TIMER_PRESCALER	0	//��Ƶֵ0,Ƶ�ʺ�ϵͳʱ��һ��
//1us�Ķ�ʱ������
#define DELAY_TIMER_PERIOD		(SystemCoreClock / 1000 /1000)-1	//71	//SystemCoreClock / 1000 /1000

#define TMR_ENABLE()	TIMER_CTL0(DELAY_TIMER)|=(uint32_t)TIMER_CTL0_CEN	//������ʱ��
#define TMR_DISABEL()	TIMER_CTL0(DELAY_TIMER) &= ~(uint32_t)TIMER_CTL0_CEN //�رն�ʱ��
#define TMR_CLR_FLAG()	TIMER_INTF(DELAY_TIMER) = (~(uint32_t)TIMER_INT_FLAG_UP)	//����жϱ�־λ
#define TMR_SET_CNT(X)	TIMER_CNT(DELAY_TIMER) = (uint32_t)(X)	//���ü�����

uint16_t us_count;

/*----------------------------------------------------------------------------------
Function   :Delay_Timer_Init
Description:��ʼ����ʱ������ʱ��
Input      :��
Retrun     :��
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
	
    timer_interrupt_enable(DELAY_TIMER,TIMER_INT_UP); //ʹ�ܸ����ж�
	nvic_irq_enable(DELAY_TIMER_IRQn,2,0); //ʹ���ж���
	TMR_DISABEL();
}


/*----------------------------------------------------------------------------------
Function   :DELAY_TIMER_IRQHandler
Description:��ʱ���ж�
Input      :��
Retrun     :��
----------------------------------------------------------------------------------*/
void DELAY_TIMER_IRQHandler(void)
{
    //timer_interrupt_flag_clear(DELAY_TIMER, TIMER_INT_FLAG_UP); //����жϱ�־λ
	TMR_CLR_FLAG();
    us_count++;
}


/*----------------------------------------------------------------------------------
Function   :Delay_1us
Description:��ʱus
Input      :count:΢����
Retrun     :��
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
	   delay_us(1000);	//Ϊ�˱ܿ��ж�
   }          
}
#endif