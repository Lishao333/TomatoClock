#ifndef _IIC_H_
#define _IIC_H_

#define IIC_ACK     0x00
#define IIC_NACK    0x01
 
void iic_init(void);
void iic_start(void);
void iic_stop(void);
unsigned char iic_read_byte(void);
void iic_write_byte(unsigned char data);
unsigned char iic_wait_ack(void);
void iic_ack(unsigned char ack_nack);

#endif