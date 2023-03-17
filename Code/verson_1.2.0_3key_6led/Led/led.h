#ifndef __LED_H
#define __LED_H

#define LED_RCU    RCU_GPIOA
#define LED_PORT   GPIOA
#define LED_1_PIN   GPIO_PIN_1
#define LED_2_PIN   GPIO_PIN_2
#define LED_3_PIN   GPIO_PIN_3
#define LED_4_PIN   GPIO_PIN_4
#define LED_5_PIN   GPIO_PIN_5
#define LED_6_PIN   GPIO_PIN_6
#define LED_WORK_PIN_MIN_10     LED_1_PIN 
#define LED_WORK_PIN_MIN_20     LED_1_PIN | LED_2_PIN 
#define LED_WORK_PIN_MIN_30     LED_1_PIN | LED_2_PIN | LED_3_PIN 
#define LED_WORK_PIN_MIN_40     LED_1_PIN | LED_2_PIN | LED_3_PIN \
							  | LED_4_PIN 
#define LED_WORK_PIN_MIN_50     LED_1_PIN | LED_2_PIN | LED_3_PIN \
							  | LED_4_PIN | LED_5_PIN 
#define LED_WORK_PIN_MIN_60     LED_1_PIN | LED_2_PIN | LED_3_PIN \
							  | LED_4_PIN | LED_5_PIN | LED_6_PIN
#define ALL_LED_WORK_PIN        LED_WORK_PIN_MIN_60

#define LED_OFF    0
#define LED_ON     1
#define LED_FLASH  2
#define LED_FLASH_TIMES         8
#define LED_FLASH_PEROID_MS     100 

#define LED_WORK_PEROID_10_S    10   //10 seconds, for test 
#define LED_WORK_PEROID_600_S   600  //10 minitue 
#define LED_WORK_PEROID_BASE    LED_WORK_PEROID_600_S
#define LED_WORK_PEROID_10_MIN  LED_WORK_PEROID_BASE    //10 minitue
#define LED_WORK_PEROID_20_MIN  LED_WORK_PEROID_BASE*2  //20 minitue
#define LED_WORK_PEROID_30_MIN  LED_WORK_PEROID_BASE*3  //30 minitue
#define LED_WORK_PEROID_40_MIN  LED_WORK_PEROID_BASE*4  //40 minitue
#define LED_WORK_PEROID_50_MIN  LED_WORK_PEROID_BASE*5  //50 minitue
#define LED_WORK_PEROID_60_MIN  LED_WORK_PEROID_BASE*6  //60 minitue

typedef struct
{
	int flash_times;
	unsigned int flash_ms_new;
	unsigned int flash_ms_old;
	unsigned int start_s;
	unsigned int num_s;
	unsigned int end_s;
	unsigned int remain_s;
	char led_status;
}LED_WORK_t,*P_LED_WORK_t;	

void led_init();
void led_on ();
void led_off ();
void set_led_work_time (int num);
void add_led_work_peroid (int peroids);
void reset_led_work_peroid ();
unsigned int get_led_work_remain_s ();
void led_work_process ();
#endif /* __LED_H */
