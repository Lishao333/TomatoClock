#ifndef __KEY_H
#define __KEY_H

typedef struct
{
	char status_old;
	char status_new;
	char press_status;
	int press_add_count;
	int press_handle_count;
	unsigned int  change_time_ms_old;
	unsigned int  change_time_ms_new;	
	unsigned int down_time_ms;
	unsigned int keep_time_ms;
	unsigned int up_time_ms;
}KEY_PRESS_t,*P_KEY_PRESS_t;	

void key_init();
void init_key_press ();
void scan_key_action();
char get_key_action ();
void key_process();
#endif /* __KEY_H */
