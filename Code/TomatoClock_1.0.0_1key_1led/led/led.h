#ifndef __LED_H
#define __LED_H

typedef struct
{
	int flash_times;
	unsigned int flash_ms_new;
	unsigned int flash_ms_old;
	unsigned int start_s;
	unsigned int work_s;
	unsigned int end_s;
	unsigned int now_s;
	char led_status;
}LED_WORK_TIME_t,*P_LED_WORK_TIME_t;	

void led_init();
void led_on ();
void led_off ();
void set_led_work_time (int num);
void add_led_work_peroid (int peroid_mini);
void reset_led_work_peroid ();
void led_work_process ();
#endif /* __LED_H */
