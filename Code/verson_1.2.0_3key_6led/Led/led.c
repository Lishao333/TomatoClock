#include <stdio.h>

#include "gd32f10x_it.h"
#include "gd32f10x.h"
#include "gd32f10x_eval.h"
#include "main.h"
#include "led.h"
#include "delay.h"
#include "systick.h"

// LED /////////////////////////////////////////////////////////////////////////////
void led_init()
{
    /* enable the led clock */
    rcu_periph_clock_enable(LED_RCU);
    /* configure led GPIO port */ 
    gpio_init(LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, ALL_LED_WORK_PIN);
}
void led_on (uint32_t led_work_pin)
{
	int level_now = gpio_input_bit_get(LED_PORT, led_work_pin);
	
	gpio_bit_set(LED_PORT, led_work_pin);
}
void led_off (uint32_t led_work_pin)
{
	int level_now = gpio_input_bit_get(LED_PORT, led_work_pin);
	
	gpio_bit_reset(LED_PORT, led_work_pin);
}
void togle_led_level (uint32_t led_work_pin)
{
	int level_now = gpio_input_bit_get(LED_PORT, led_work_pin);
	
	if (level_now == 1)
		gpio_bit_reset(LED_PORT, led_work_pin);
	else if (level_now == 0)
		gpio_bit_set(LED_PORT, led_work_pin);
}
void update_led_work_pin (uint32_t led_work_pin)
{
	led_off (ALL_LED_WORK_PIN);
	led_on (led_work_pin);
}

//获取最新的LED工作引脚
uint32_t get_led_work_pin (unsigned int led_work_peroid)
{
	uint32_t led_work_pin = 0;

	if (led_work_peroid > 0
		&& led_work_peroid <= LED_WORK_PEROID_10_MIN)
	{
		led_work_pin = LED_WORK_PIN_MIN_10;
	}
	else if (led_work_peroid > LED_WORK_PEROID_10_MIN
			 && led_work_peroid <= LED_WORK_PEROID_20_MIN)
	{
		led_work_pin = LED_WORK_PIN_MIN_20;
	}
	else if (led_work_peroid > LED_WORK_PEROID_20_MIN
			 && led_work_peroid <= LED_WORK_PEROID_30_MIN)
	{
		led_work_pin = LED_WORK_PIN_MIN_30;
	}
	else if (led_work_peroid > LED_WORK_PEROID_30_MIN
			 && led_work_peroid <= LED_WORK_PEROID_40_MIN)
	{
		led_work_pin = LED_WORK_PIN_MIN_40;
	}
	else if (led_work_peroid > LED_WORK_PEROID_40_MIN
			 && led_work_peroid <= LED_WORK_PEROID_50_MIN)
	{
		led_work_pin = LED_WORK_PIN_MIN_50;
	}
	else if (led_work_peroid > LED_WORK_PEROID_50_MIN
			 && led_work_peroid <= LED_WORK_PEROID_60_MIN)
	{
		led_work_pin = LED_WORK_PIN_MIN_60;
	}
	else if (led_work_peroid > LED_WORK_PEROID_60_MIN)
	{
		led_work_pin = LED_WORK_PIN_MIN_60;
	}
	
	return led_work_pin;
}


LED_WORK_t led_work = {0};

unsigned int get_led_work_num_s ()
{
	return led_work.num_s;
}
unsigned int get_led_work_start_s ()
{
	return led_work.start_s;
}
unsigned int get_led_work_end_s ()
{
	return led_work.end_s;
}
int get_led_work_flash_times ()
{
	return led_work.flash_times;
}

//设置LED的工作状态
char set_led_work_stauts (char status)
{
	if (status == LED_OFF
		|| status == LED_ON
		|| status == LED_FLASH)
	{
		led_work.led_status = status;
	}
}
char get_led_work_stauts ()
{
	return led_work.led_status;
}

//设置LED的工作时间
void set_led_work_time (int num)
{
	unsigned int sys_time_now_s = get_sys_tick_s();
	
	if (led_work.end_s < sys_time_now_s) //如果之前设定的LED灯工作时间已经结束，则重新设定开始、工作、结束的时间
	{
		led_work.start_s = sys_time_now_s;
		led_work.num_s = num;
		led_work.end_s = led_work.start_s+led_work.num_s;
		led_work.remain_s = led_work.end_s - sys_time_now_s;
	}
	else  //如果之前设定的LED灯工作时间仍未结束，则累计工作的时间
	{
		led_work.num_s += num;		
		led_work.end_s = led_work.start_s+led_work.num_s;
		led_work.remain_s = led_work.end_s - sys_time_now_s;
	}
}

//更新LED工作的剩余时间
void updata_led_work_remain_s ()
{
	unsigned int sys_time_now_s = get_sys_tick_s ();
	unsigned int led_work_end_s = get_led_work_end_s ();
	
	if (led_work_end_s > sys_time_now_s)
		led_work.remain_s = led_work_end_s - sys_time_now_s;
	else
		led_work.remain_s = 0;
}

//更新LED工作的剩余时间
unsigned int get_led_work_remain_s ()
{
	if (led_work.remain_s <= 0)
	{
		set_led_work_stauts (LED_OFF);
		return 0;
	}
	else 
		return led_work.remain_s;
}

//关闭LED和清除其工作信息
void desable_and_clear_led_work ()
{
	led_work.flash_times = 0;
	led_work.flash_ms_old = 0;
	led_work.flash_ms_new = 0;
	led_work.start_s = 0;
	led_work.num_s = 0;
	led_work.end_s = 0;
	led_work.remain_s = 0;
	led_work.start_s = 0;
	
	led_off (ALL_LED_WORK_PIN);
	DEB_LOG("desable_and_clear_led_work(): Desable led and clear led work time!!!\r\n");
}

//led闪烁
void led_flash (int times, unsigned int peroid_ms)
{
	int i = 0;
	uint32_t led_work_pin = get_led_work_pin (get_led_work_remain_s());
	
	led_work.flash_times = 0;
	while (led_work.flash_times<=times)
	{
		//DEB_LOG("do_led_work(): Reseting...\r\n");
		led_work.flash_ms_new = get_sys_tick_ms();
		if (led_work.flash_ms_new - led_work.flash_ms_old >= peroid_ms)
		{
			led_work.flash_ms_old = led_work.flash_ms_new;
			led_work.flash_times++;
			togle_led_level (led_work_pin);
		}
	}
	desable_and_clear_led_work ();
}

void add_led_work_peroid (int peroids)
{
	set_led_work_stauts (LED_ON);
	set_led_work_time (peroids*LED_WORK_PEROID_BASE);
	DEB_LOG("add_led_work_peroid(): Add led work time: %d seconds, Remain work time: %d seconds\r\n", peroids*LED_WORK_PEROID_10_MIN,  get_led_work_remain_s());
}

void reset_led_work_peroid ()
{
	//获取LED工作的总时间
	if (get_led_work_num_s() != 0)
	{
		//设置LED的工作状态为闪烁
		set_led_work_stauts (LED_FLASH);
	}
	else 
	{
		return;
	}
}

void led_work_process ()
{
	uint32_t led_work_pin_new = 0;
	static uint32_t led_work_pin_old = 0;
	
	//更新LED工作的剩余时间
	updata_led_work_remain_s ();
	
	//获取LED的工作状态
	if (LED_FLASH == get_led_work_stauts()) //reset led work
	{
		//led闪烁
		led_flash(LED_FLASH_TIMES, LED_FLASH_PEROID_MS);
	}
	else //Led normal work
	{		
		/* only when remain_s of working led has been changed, execute this function */
		led_work_pin_new = get_led_work_pin (get_led_work_remain_s()); //获取最新的LED工作引脚
		if (led_work_pin_old != led_work_pin_new) //如果之前的LED工作引脚与最新的不同
		{
			 //更新LED工作引脚
			led_work_pin_old = led_work_pin_new;
			update_led_work_pin (led_work_pin_new);
			DEB_LOG("do_led_work(): Change led work pin(0x%x)!!!\r\n", led_work_pin_new);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////
