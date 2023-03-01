/*******************************************************************************
file : platform_iic.c
author : huohongpeng 
date : 2017-02-23
description : application for IIC, base on gd lib (GPIO)
*******************************************************************************/
#include "iic.h"
#include "delay.h"
#include "gd32f10x.h"
 
 
#define iic_printf printf("[%d-%s()] : ",__LINE__, __FUNCTION__);printf
 
/**
description : IIC delay us function interface
param :  us - microsecond count
retval : None
author : huohongpeng
date : 2017-02-23
*/
static void iic_delay_us(int us)
{
    delay_us(us);
}
 
/**
description : IIC SCL to high
param :  None
retval : None
author : huohongpeng
date : 2017-02-23
*/
static void iic_scl_h(void)
{
    gpio_bit_set(GPIOB, GPIO_PIN_13);
}
 
/**
description : IIC SCL to low
param :  None
retval : None
author : huohongpeng
date : 2017-02-23
*/
static void iic_scl_l(void)
{
    gpio_bit_reset(GPIOB, GPIO_PIN_13);
}
 
/**
description : IIC write one bit data to sda
param :  bit - 0, 1
retval : None
author : huohongpeng
date : 2017-02-23
*/
static void iic_write_sda(unsigned char bit)
{
    if(bit == 0)
    {
        gpio_bit_reset(GPIOB, GPIO_PIN_15);
    }
    else   
    {
        gpio_bit_set(GPIOB, GPIO_PIN_15);
    }
}
 
/**
description : IIC read one bit data to sda
param :  None
retval : bit - 0, 1
author : huohongpeng
date : 2017-02-23
*/
static unsigned char iic_read_sda(void)
{
    if(gpio_input_bit_get(GPIOB, GPIO_PIN_15))
    {
        return 0x01;
    }
    else 
    {
        return 0x00;
    }  
}
 
/**
description : IIC SCL and SDA init
param :  None
retval : None
author : huohongpeng
date : 2017-02-23
*/
void iic_init(void)
{
	/* enable the led clock */ 
    rcu_periph_clock_enable(RCU_GPIOB); 
    /* configure led GPIO port */ 
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13); 
    gpio_init(GPIOB, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_15); 
}
 
/**
description : IIC send start 
param :  None
retval : None
author : huohongpeng
date : 2017-02-23
*/
void iic_start(void)
{
    iic_write_sda(1);
    iic_scl_h();
    iic_delay_us(1);
 
    iic_write_sda(0);
    iic_delay_us(1);
 
    iic_scl_l();
    iic_delay_us(1);
}
 
/**
description : IIC send stop 
param :  None
retval : None
author : huohongpeng
date : 2017-02-23
*/
void iic_stop(void)
{
    iic_scl_l();
    iic_delay_us(1);
    iic_write_sda(0);
 
    iic_scl_h();
    iic_delay_us(1);
 
    iic_write_sda(1);
    iic_delay_us(1);
}
 
/**
description : IIC read one byte data 
param :  None
retval : one byte data
author : huohongpeng
date : 2017-02-23
*/
unsigned char iic_read_byte(void)
{
    int i, data;
 
    for(i = 0, data = 0; i < 8; i++)
    {
        data <<= 1;
        iic_scl_h();
        iic_delay_us(1);
        data |= iic_read_sda();
        iic_scl_l();
        iic_delay_us(1);
    }
 
    return data;
}
 
/**
description : IIC write one byte data 
param :  one byte data
retval : None
author : huohongpeng
date : 2017-02-23
*/
void iic_write_byte(unsigned char data)
{
    int i;
 
    for(i = 0; i < 8; i++)
    {
        iic_write_sda((data & 0x80)>>7);
        iic_delay_us(1);
        iic_scl_h();
        iic_delay_us(1);
        iic_scl_l();
        iic_delay_us(1);
        data <<= 1;
    } 
 
}
 
/**
description : IIC wait device ack
param :  None
retval : 0 - success
         1 - error
author : huohongpeng
date : 2017-02-23
*/
unsigned char iic_wait_ack(void)
{
    iic_write_sda(0x01);
    iic_delay_us(1);
    iic_scl_h();
    iic_delay_us(1);
    
    if(iic_read_sda() == 0)
    {
        iic_scl_l();
        iic_delay_us(1);
 
        return 0;
    }
    else 
    {
        iic_printf("stop \r\n");
        iic_stop();
        return 1;
    }
}
 
/**
description : IIC send ack or nack to device
param :  None
retval : None
author : huohongpeng
date : 2017-02-23
*/
void iic_ack(unsigned char ack_nack)
{
    iic_write_sda(ack_nack);
    iic_delay_us(1);
    iic_scl_h();
    iic_delay_us(1);
    iic_scl_l();
    iic_delay_us(1);
}