/*!
    \file    main.c
    \brief   running led
    
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

#include "gd32f10x.h"
#include "gd32f10x_eval.h"
#include "delay.h"
#include <stdio.h>

#include "main.h"
#include "led.h"
#include "key.h"
#include "OLED.h"
#include "uart.h"
#include "iic.h"

#define FOUNCTION_TEST_ENABLE    1
void DefaultSetCharSizeSetInfo ();
void OLED_ShowStringControl (char* str, int len);
void show_test ();
void OLED_BackSpace ();
void FunctionTest ()
{
	char test_str[30] = {"Hello LiShaolin 112233"};
		
	DefaultSetCharSizeSetInfo ();

	//OLED_Test ();
	OLED_ClearAll ();
	OLED_ShowStringControl (test_str, strlen(test_str));
	
	delay_ms(1000);
	OLED_BackSpace ();
	delay_ms(1000);
	OLED_BackSpace ();
	delay_ms(1000);
	OLED_BackSpace ();
	delay_ms(1000);
	
	OLED_ShowStringControl (test_str, strlen(test_str));
	while(1)
	{
#if 0		
		//iic test
		OLED_SCLK_Clr();
		delay_ms(1000);
		OLED_SCLK_Set();
		delay_ms(1000);
#endif
#if 1		
		//oled test
		//OLED_ShowString_612 (LINE_612_0, ROW_612_0, test_str, strlen(test_str));
#endif		
		delay_ms(1000);
		printf("FunctionTest():Test...\r\n");
	}
}

// main /////////////////////////////////////////////////////////////////////////////
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
	int ret = 0;
	//led_init();
	//key_init();
	delay_init();
    pc_com_init();
	iic_init();
	OLED_Init();
	//init_key_press ();
    printf("*************************** %s, %s ***************************", SOFT_VER, ISSUE_DATA);
	
#if !FOUNCTION_TEST_ENABLE	
	while(1)
	{
		key_process();
		led_work_process ();
	}
#else	
	FunctionTest ();
#endif	
}
