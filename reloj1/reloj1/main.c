/*
 * reloj1.c
 *
 * Created: 02/12/2024 09:23:09 p. m.
 * Author : andro
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd_i2c.h"
#include "i2c.h"
#include "rtc_ds1307.h"


int main(void)
{
	
	init_i2c();
	lcd_i2c_init();
	rtc_init();
	
	lcd_i2c_clr();
	
	rtc_set_time(23, 59, 57);   // HH, MM, SS
	rtc_set_date(5, 12, 2024); // DD, MM, YYYY
	
    
    while (1) 
	{
		// Leer tiempo y fecha
		Time time = rtc_get_time();
		Date date = rtc_get_date();

		// Mostrar en el LCD
		
		
		lcd_i2c_col_row(5, 1);
		lcd_i2c_printf("%02d:%02d:%02d", time.hours, time.minutes, time.seconds);
		
		lcd_i2c_col_row(4, 2);
		lcd_i2c_printf("%02d/%02d/%04d", date.day, date.month, date.year);

		_delay_ms(1000); // Actualizar cada segundo
    }
	
}

