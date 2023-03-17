#include "gd32f10x.h"
#include "gd32f10x_eval.h"
#include "systick.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "key.h"
#include "led.h"
#include "gd32f10x_it.h"

// KEY /////////////////////////////////////////////////////////////////////////////
#define KEY_RCU   RCU_GPIOB
#define KEY_PORT  GPIOB
#define KEY_PIN   GPIO_PIN_1
void key_init()
{
    /* enable the led clock */
    rcu_periph_clock_enable(KEY_RCU);
    /* configure led GPIO port */ 
    gpio_init(KEY_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, KEY_PIN);
}

#define KEY_DOW      0
#define KEY_UP       1
#define KEY_VALID_CHANGE_MS          200
#define KEY_PRESS_STATUS_UP          0
#define KEY_PRESS_STATUS_DOWN        1
#define KEY_PRESS_STATUS_KEEP_PRESS  2
#define KEY_PRESS_STATUS_NO_PRESS    3
KEY_PRESS_t key_press = {0};
char get_key_press_status ()
{
	return key_press.press_status;
}char set_key_press_status (char status)
{
	key_press.press_status = status;
}

void init_key_press ()
{
	key_press.change_time_ms_new = 0;
	key_press.change_time_ms_old = 0;
	key_press.up_time_ms = 0;
	key_press.down_time_ms = 0;
	key_press.keep_time_ms = 0;
	key_press.press_add_count = 0;
	key_press.press_handle_count = 0;
	key_press.press_status = KEY_PRESS_STATUS_NO_PRESS;
	key_press.status_new = KEY_UP;
	key_press.status_old = KEY_UP;
}
void clear_key_press_time ()
{
	key_press.up_time_ms = 0;
	key_press.down_time_ms = 0;
	key_press.keep_time_ms = 0;
	key_press.press_status = KEY_PRESS_STATUS_NO_PRESS;
}
static char handle_long_press_flag = 0; 
void scan_key_press ()	
{	
	unsigned int sys_time_now_ms = get_sys_tick_ms();
	char key_status_now = gpio_input_bit_get(KEY_PORT, KEY_PIN);
	
	key_press.status_new = key_status_now;
	if (key_press.status_old != key_press.status_new)
	{
		key_press.change_time_ms_new = sys_time_now_ms;
		if (key_press.change_time_ms_new - key_press.change_time_ms_old >= KEY_VALID_CHANGE_MS)
		{
			key_press.change_time_ms_old = key_press.change_time_ms_new;
			key_press.status_old = key_press.status_new;
			
			if (KEY_DOW == key_press.status_new)
			{
				key_press.press_status = KEY_PRESS_STATUS_DOWN;
				key_press.down_time_ms = sys_time_now_ms;
			}
			else if (KEY_UP == key_press.status_new)
			{
				handle_long_press_flag = 0;
				key_press.press_status = KEY_PRESS_STATUS_UP;
				key_press.keep_time_ms = sys_time_now_ms - key_press.down_time_ms;
				key_press.up_time_ms = sys_time_now_ms;
			}
		}
	}
	else if (key_press.status_old == key_press.status_new
			 && KEY_DOW == key_press.status_new)
	{
		if (handle_long_press_flag)
			return;
		key_press.press_status = KEY_PRESS_STATUS_KEEP_PRESS;
		key_press.keep_time_ms = sys_time_now_ms - key_press.down_time_ms;
	}
	else if (key_press.status_old == key_press.status_new
			 && KEY_UP == key_press.status_new)
	{
		key_press.press_status = KEY_PRESS_STATUS_NO_PRESS;
	}
}

#define IS_IN_PRESSING    	 3
#define IS_NOT_PRESS    	 2
#define IS_SHORT_PRESS  	 1
#define IS_LONG_PRESS   	 0
#define SHORT_PRESS_MS_MIN   150
#define SHORT_PRESS_MS_MAX   1000
#define LONG_PRESS_MS_MIN    1500
#define LONG_PRESS_MS_MAX    5000
int get_key_press_action ()
{
	scan_key_press ();
	
	if (KEY_PRESS_STATUS_NO_PRESS == key_press.press_status)
	{
		//DEB_LOG("Is not press!!!\r\n");
		return IS_NOT_PRESS;
	}
	else if (KEY_PRESS_STATUS_KEEP_PRESS == key_press.press_status)
	{
		//DEB_LOG("Is in pressing!!!\r\n");
		if (key_press.keep_time_ms >= LONG_PRESS_MS_MIN )
		{
			DEB_LOG("Is long press!!!\r\n");
			key_press.press_add_count += 1;
			clear_key_press_time ();
			return IS_LONG_PRESS;
		}
		else 
			return IS_IN_PRESSING;
	}
	else if (KEY_PRESS_STATUS_UP == key_press.press_status
			 &&key_press.keep_time_ms >= SHORT_PRESS_MS_MIN
			 &&key_press.keep_time_ms <= SHORT_PRESS_MS_MAX )
	{
		DEB_LOG("Is short press!!!\r\n");
		key_press.press_add_count += 1;
		clear_key_press_time ();
		return IS_SHORT_PRESS;
	}
}

void handle_short_press_key ()
{	
	add_led_work_peroid (1);
}
void handle_long_press_key ()
{	
	reset_led_work_peroid ();
	handle_long_press_flag = 1;
}
void key_process()
{
	int key_action = get_key_press_action ();	
	
	if (key_press.press_handle_count < key_press.press_add_count)
	{
		if (key_action ==  IS_SHORT_PRESS)
		{
			handle_short_press_key();
		}
		else if (key_action ==  IS_LONG_PRESS)
		{
			handle_long_press_key ();
		}	
		
		key_press.press_handle_count += 1;
	}
}
/////////////////////////////////////////////////////////////////////////////////////
