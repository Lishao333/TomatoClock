#include "gd32f10x.h"
#include "gd32f10x_eval.h"
#include "delay.h"
#include <stdio.h>

#include "uart.h"

// USART /////////////////////////////////////////////////////////////////////////////

#define PC_COM				USART0
#define PC_COM_RCU			RCU_USART0
#define PC_COM_PORT_RCU  	RCU_GPIOA
#define PC_COM_PORT   		GPIOA
#define PC_COM_TX  			GPIO_PIN_9
#define PC_COM_RX   		GPIO_PIN_10
/*! 
    \brief      USART0 configure 
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pc_com_init(void)
{
    /* enable USART clock */
    rcu_periph_clock_enable(PC_COM_RCU);

	/* enable GPIO clock */
	rcu_periph_clock_enable(PC_COM_PORT_RCU);
	
	/* connect port to USARTx_Tx */
	gpio_init(PC_COM_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, PC_COM_TX);
	/* connect port to USARTx_Rx */
	gpio_init(PC_COM_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, PC_COM_RX);
    
    /* USART configure */
    usart_deinit(PC_COM);
    usart_baudrate_set(PC_COM, 115200U);
    usart_word_length_set(PC_COM, USART_WL_8BIT);
    usart_stop_bit_set(PC_COM, USART_STB_1BIT);
    usart_parity_config(PC_COM, USART_PM_NONE);
    usart_hardware_flow_rts_config(PC_COM, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(PC_COM, USART_CTS_DISABLE);
    usart_receive_config(PC_COM, USART_RECEIVE_ENABLE);
    usart_transmit_config(PC_COM, USART_TRANSMIT_ENABLE);
    usart_enable(PC_COM);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(PC_COM, (uint8_t)ch);
    while(RESET == usart_flag_get(PC_COM, USART_FLAG_TBE));
    return ch;
}
/////////////////////////////////////////////////////////////////////////////////////
