#include "gd32f10x.h"
#include "gd32f10x_eval.h"
#include "systick.h"
#include <stdio.h>

#include "led.h"
#include "gd32f10x_it.h"

// LED /////////////////////////////////////////////////////////////////////////////
#define LED_RCU   RCU_GPIOC
#define LED_PORT  GPIOC
#define LED_PIN   GPIO_PIN_1
void led_init()
{
    /* enable the led clock */
    rcu_periph_clock_enable(LED_RCU);
    /* configure led GPIO port */ 
    gpio_init(LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_PIN);
}
void led_on ()
{
	gpio_bit_set(LED_PORT, LED_PIN);
}
void led_off ()
{
	gpio_bit_reset(LED_PORT, LED_PIN);
}
void togle_led_level ()
{
	int level_now = gpio_input_bit_get(LED_PORT, LED_PIN);
	
	if (level_now == 1)
		gpio_bit_reset(LED_PORT, LED_PIN);
	else if (level_now == 0)
		gpio_bit_set(LED_PORT, LED_PIN);
}

#define LED_OFF    0
#define LED_ON     1
#define LED_FLASH  2
#define LED_WORK_PEROID_MINI  30  
LED_WORK_TIME_t led_work_time = {0};
void set_led_work_time (int num)
{
	unsigned int sys_time_now = get_sys_tick_s();
	
	if (led_work_time.end_s < sys_time_now) //如果之前设定的LED灯工作时间已经结束，则重新设定开始、工作、结束的时间
	{
		led_work_time.start_s = sys_time_now;
		led_work_time.work_s = num;
		led_work_time.end_s = led_work_time.start_s+led_work_time.work_s;
	}
	else  //如果之前设定的LED灯工作时间仍未结束，则累计工作的时间
	{
		led_work_time.work_s += num;		
		led_work_time.end_s = led_work_time.start_s+led_work_time.work_s;
	}
}
void add_led_work_peroid (int peroid_mini)
{
	set_led_work_time (peroid_mini*LED_WORK_PEROID_MINI*60);
}
#define LED_FLASH_TIMES        8
#define LED_FLASH_PEROID_MS    100 
void clear_led_work_time ()
{
	led_work_time.flash_times = 0;
	led_work_time.flash_ms_old = 0;
	led_work_time.flash_ms_new = 0;
	led_work_time.start_s = 0;
	led_work_time.work_s = 0;
	led_work_time.end_s = 0;
	led_work_time.now_s = 0;
	led_work_time.led_status = LED_OFF;
}
void led_flash (int times, unsigned int peroid_ms)
{
	int i = 0;
	
	for (i=0; i<times; i++)
	{
//		togle_led_level ();
//		delay_ms(peroid_ms);
		led_work_time.flash_ms_new = get_sys_tick_ms();
		if (led_work_time.flash_ms_new - led_work_time.flash_ms_old >= peroid_ms)
		{
			led_work_time.flash_ms_old = led_work_time.flash_ms_new;
			led_work_time.flash_times++;
			togle_led_level ();
		}
	}
}
void reset_led_work_peroid ()
{
	led_work_time.led_status = LED_FLASH;
}
void check_led_work()
{
	int level_now = gpio_input_bit_get(LED_PORT, LED_PIN);
	
	if (LED_ON == led_work_time.led_status
		&& LED_ON != level_now)
		led_on ();
	else if (LED_OFF == led_work_time.led_status
			 && LED_OFF != level_now)
		led_off ();
}
void led_work_process ()
{
	unsigned int sys_time_now = get_sys_tick_s();
	
	if (LED_FLASH == led_work_time.led_status
		&& led_work_time.work_s != 0)
	{
		led_flash(LED_FLASH_TIMES, LED_FLASH_PEROID_MS);
		
		if (led_work_time.flash_times >= LED_FLASH_TIMES)
		{
			printf("Clear LED work time!!!\r\n");
			clear_led_work_time ();
			led_off ();
		}
	}
	else if (LED_OFF == led_work_time.led_status
		&& led_work_time.end_s > sys_time_now)
	{
		led_on ();
		led_work_time.led_status = LED_ON;
	}
	else if (LED_ON == led_work_time.led_status
			&& led_work_time.end_s <= sys_time_now)
	{
		led_off ();
		clear_led_work_time ();
	}
	
	check_led_work();
}
/////////////////////////////////////////////////////////////////////////////////////
