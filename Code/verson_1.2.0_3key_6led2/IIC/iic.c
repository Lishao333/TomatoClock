#include "iic.h"


void HW_I2cInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_I2C0);
	
	//gpio_af_set(GPIOB,GPIO_AF_1,GPIO_PIN_6);
	//gpio_af_set(GPIOB,GPIO_AF_1,GPIO_PIN_7);
	
    gpio_init(RCU_GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	//io_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);
	//gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	
    gpio_init(RCU_GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
	//gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);
	//gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
	
	i2c_deinit(I2C0);//reset I2C
	i2c_clock_config(I2C0, 100000, I2C_DTCY_2);//set baudrate
	i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x78);

	i2c_enable (I2C0);
	i2c_ack_config(I2C0,I2C_ACK_ENABLE);
}
