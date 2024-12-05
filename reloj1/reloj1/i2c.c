/*
 * i2c.c
 *
 * Created: 05/12/2024 04:33:52 a. m.
 *  Author: andro
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"

void init_i2c(void) 
{
	TWSR = 0x00; // Sin prescaler
	TWBR =0X14; // ((F_CPU / F_SCL) - 16) / (2 * prescaler);
	TWCR = (1 << TWEN); // Habilitar I2C
}

void start(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //Genera condición de START
	while((TWCR & (1<<TWINT))==0); //Espera hasta que TWINT=0 (TWI termina su trabajo)
	while (!(TWCR & (1 << TWINT)));
}

void stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //Genera condición de STOP
	
	_delay_ms(1);
}

void write_i2c(uint8_t data)
{
	TWDR = data; //Byte a escribir
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); //Escritura con reconocimiento (TWEA=1)
	while((TWCR & (1<<TWINT))==0); //Espera hasta que TWINT=0 (TWI termina su trabajo)
}

uint8_t read_i2c()
{
	TWCR = (1<<TWINT)|(1<<TWEN); //Lectura sin reconocimiento (TWEA=0)
	while((TWCR & (1<<TWINT))==0); //Espera hasta que TWINT=0 (TWI termina su trabajo)
	return TWDR; //Regresa el valor leído
}


uint8_t i2c_read_ack(void) {
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT))); // Esperar a que termine
	return TWDR;
}

uint8_t i2c_read_nack(void) {
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); // Esperar a que termine
	return TWDR;
}
