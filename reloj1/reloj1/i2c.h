/*
 * i2c.h
 *
 * Created: 05/12/2024 04:34:35 a. m.
 *  Author: andro
 */ 


#ifndef I2C_H_
#define I2C_H_

void init_i2c(void);
uint8_t read_i2c();
void write_i2c(uint8_t data);
void start(void);
void stop(void);

uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);

#define TRUE 1
#define FALSE 0

#endif /* I2C_H_ */