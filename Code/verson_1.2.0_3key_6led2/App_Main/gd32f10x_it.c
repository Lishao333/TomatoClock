/*!
    \file    gd32f10x_it.c
    \brief   interrupt service routines
    
    \version 2014-12-26, V1.0.0, firmware for GD32F10x
    \version 2017-06-20, V2.0.0, firmware for GD32F10x
    \version 2018-07-31, V2.1.0, firmware for GD32F10x
    \version 2020-09-30, V2.2.0, firmware for GD32F10x
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f10x_it.h"
#include "main.h"
#include "delay.h"

#include "key.h"
/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
static unsigned int sys_tick_ms  = 0;
static unsigned int sys_tick_s   = 0;
static unsigned int sys_tick_m   = 0;
static unsigned int sys_tick_h   = 0;
void set_sys_tcik ()
{
	//Prevent boundary problems from happening.
	if (sys_tick_ms <= 4294967000) //the value of unsigned int is 4294967295
		sys_tick_ms++;
	else 
		sys_tick_ms = 0;
	
	if (sys_tick_ms>=1000 && sys_tick_ms%1000==0)
	{
		if (sys_tick_s <= 4294967200) //the value of unsigned int is 4294967295
			sys_tick_s++;
		else 
			sys_tick_s = 0;
	}
	
	if (sys_tick_s>=60 && sys_tick_s%60==0)
	{
		if (sys_tick_m <= 4294967200) //the value of unsigned int is 4294967295
			sys_tick_m++;
		else 
			sys_tick_m = 0;
	}
	
	if (sys_tick_m>=60 && sys_tick_m%60==0)
	{
		if (sys_tick_h <= 4294967200) //the value of unsigned int is 4294967295
			sys_tick_h++;
		else 
			sys_tick_h = 0;
	}
}
unsigned int get_sys_tick_ms ()
{
	return sys_tick_ms;
}
unsigned int get_sys_tick_s ()
{
	return sys_tick_s;
}
unsigned int get_sys_tick_m ()
{
	return sys_tick_m;
}
unsigned int get_sys_tick_h ()
{
	return sys_tick_h;
}
void SysTick_Handler(void)
{
	set_sys_tcik ();
    delay_decrement();
}
