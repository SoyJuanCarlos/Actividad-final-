/*
 * rtc_ds1307.c
 *
 * Created: 05/12/2024 02:13:10 a. m.
 *  Author: andro
 */ 
#include "rtc_ds1307.h"
#include "i2c.h"

#define DS1307_ADDR 0x68

static uint8_t bcd_to_decimal(uint8_t bcd) {
	return (bcd >> 4) * 10 + (bcd & 0x0F);
}

static uint8_t decimal_to_bcd(uint8_t dec) {
	return ((dec / 10) << 4) | (dec % 10);
}

void rtc_init(void) {
	start();
	write_i2c(DS1307_ADDR << 1);
	write_i2c(0x00); // Dirección del registro de segundos
	write_i2c(0x00); // Habilitar el reloj
	stop();
}



void rtc_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds) {
	start();
	write_i2c(DS1307_ADDR << 1);
	write_i2c(0x00); // Registro de segundos
	write_i2c(decimal_to_bcd(seconds));
	write_i2c(decimal_to_bcd(minutes));
	write_i2c(decimal_to_bcd(hours));
	stop();
}


Time rtc_get_time(void) {
	Time time;
	start();
	write_i2c(DS1307_ADDR << 1);
	write_i2c(0x00); // Registro de segundos
	start();
	write_i2c((DS1307_ADDR << 1) | 1); // Leer
	time.seconds = bcd_to_decimal(i2c_read_ack());
	time.minutes = bcd_to_decimal(i2c_read_ack());
	time.hours = bcd_to_decimal(i2c_read_nack());
	stop();
	return time;
}

void rtc_set_date(uint8_t day, uint8_t month, uint16_t year) {
	start();
	write_i2c(DS1307_ADDR << 1);
	write_i2c(0x04); // Registro de día
	write_i2c(decimal_to_bcd(day));
	write_i2c(decimal_to_bcd(month));
	write_i2c(decimal_to_bcd(year % 100)); // Año (2 dígitos)
	stop();
}

Date rtc_get_date(void) {
	Date date;
	start();
	write_i2c(DS1307_ADDR << 1);
	write_i2c(0x04); // Registro de día
	start();
	write_i2c((DS1307_ADDR << 1) | 1); // Leer
	date.day = bcd_to_decimal(i2c_read_ack());
	date.month = bcd_to_decimal(i2c_read_ack());
	date.year = 2000 + bcd_to_decimal(i2c_read_nack());
	stop();
	return date;
}

