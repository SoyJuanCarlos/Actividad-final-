/*
 * lcd_i2c.c
 *
 * Created: 05/12/2024 04:37:21 a. m.
 *  Author: andro
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <stdarg.h>
#include <stdio.h>
#include "i2c.h"
#include "lcd_i2c.h"

char Columna_1 [2] = {0 , 64};	//Direcciones de la primera columna



/********************************************************
*	Ejecuta las acciones para inicializar el LCD en modo de 4 bits
*   de acuerdo con: fig.24 hoja de datos del controlador Hitachi 44780.
*	Entrada: Ninguna
*	Salida: Ninguna
*********************************************************/
void lcd_i2c_reset(void)
{
	//unsigned char dato;p
	start();
	write_i2c(PCF8574_ADDR);
	
	write_i2c(0xFF); //Esperar al menos 15 ms
	_delay_ms(20);
	
	write_i2c(0x30+LCD_EN); //Escribe Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 y E=1
	write_i2c(0x30);		//Escribe Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 y E=0
	_delay_ms(10);			//Espera al menos 4.1 ms
	
	write_i2c(0x30+LCD_EN); //Escribe Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 y E=1
	write_i2c(0x30);		//Escribe Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 y E=0
	_delay_ms(1);			//Espera al menos 100 us
	
	write_i2c(0x30+LCD_EN); //Escribe Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 y E=1
	write_i2c(0x30);		//Escribe Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=1 y E=0
	_delay_ms(1);			//Espera al menos 100 us
	
	write_i2c(0x20+LCD_EN); //Escribe Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=0 y E=1
	write_i2c(0x20);		//Escribe Rs=0|RW=0|DB7=0|DB6=0|DB5=1|DB4=0 y E=0
	_delay_ms(1);			//Espera al menos 100 us
	
	stop();
}

/********************************************************
*	Funci�n que escribe un comando al LCD
*	Entrada: comando en hexadecimal (p.e. 0x2C)
*	Salida: Ninguna
*********************************************************/
void lcd_i2c_cmd (char cmd)
{
	
	unsigned char data;
	start();
	write_i2c(PCF8574_ADDR);//Manda direccion del dispositivo
	
	data=(cmd & 0xF0)|LCD_EN|LUZ_FONDO;
	write_i2c(data);

	data=(cmd & 0xF0)|LUZ_FONDO;
	write_i2c(data);

	data=((cmd << 4) & 0xF0)|LCD_EN|LUZ_FONDO;
	write_i2c(data);

	data=((cmd << 4) & (0xF0|LUZ_FONDO));
	write_i2c(data);
	
	stop();
	_delay_ms(2);
	_delay_ms(2);
}

//Funci�n para configurar el LCD
/********************************************************
*	Funci�n para configurar el funcionamiento del LCD
*	Entrada: Ninguna
*	Salida: Ninguna
*********************************************************/
void lcd_i2c_init (void)
{
	init_i2c();

	lcd_i2c_reset();        // Reset para el LCD.
	
	lcd_i2c_cmd(0x2C);		// Function Set: Modo 4 bits, 2 l�neas, resoluci�n 5�10.
	//lcd_i2c_cmd(0x0F);      // Display control: LCD encendido, cursor encendido, parpadeo.
	lcd_i2c_cmd(0x0C);		//Cursor apagado.
	lcd_i2c_cmd(0x06);      // Entry mode: Increment.
	lcd_i2c_cmd(0x80);      // Apunta a la direcci�n de la DDRAM.
}


/********************************************************
*	Funci�n que despliega un solo caracter
*	Entrada: caracter entre comillas sencillas (p.e. 'A')
*	Salida: Ninguna
*********************************************************/
void lcd_i2c_data (unsigned char dat)
{
	unsigned char data;
	start();
	write_i2c(PCF8574_ADDR);

	data=((dat & 0xF0)|LCD_EN|LCD_RS|LUZ_FONDO);
	write_i2c(data);

	data=((dat & 0xF0)|LCD_RS|LUZ_FONDO);
	write_i2c(data);

	data=(((dat << 4) & 0xF0)|LCD_EN|LCD_RS|LUZ_FONDO);
	write_i2c(data);

	data=(((dat << 4) & 0xF0)|LCD_RS|LUZ_FONDO);
	write_i2c(data);

	stop();
	_delay_ms(2);
	_delay_ms(2);
}

/********************************************************
*	Posiciona el cursor en Columna y Rengl�n
*	Entrada: Columna y Rengl�n (1,1)
*	Salida: Ninguna
*********************************************************/
void lcd_i2c_col_row(uint8_t x, uint8_t y)
{
	lcd_i2c_cmd(0x80 + Columna_1[y-1]+(x-1));
}

/********************************************************
*	Limpia la pantalla
*	Entrada: Nada
*	Salida: Nada
*********************************************************/
void lcd_i2c_clr(void)
{
	lcd_i2c_cmd(0x01); //Limpia la pantalla (0000 0001)
	_delay_ms(2);
}

/********************************************************
*	Despliega una cadena de caracteres
*	Entrada: Cadena de caracteres entre comillas (p.e. "Hola LCD..")
*	Salida: Ninguna
*********************************************************/
void lcd_i2c_write_string(char *a)
{
	unsigned int i;
	for(i=0;a[i]!=0;i++)
	lcd_i2c_data(a[i]);
}

/********************************************************
*	Escribe un valor entero
*	Entrada: Valor a imprimir, longitud del valor a imprimir (1 a 5)
*	Salida: Ninguna
*********************************************************/
void lcd_i2c_write_int(int value,unsigned int field_length)
{
	char str[5]={0,0,0,0,0};
	int i=4,j=0;
	while(value)
	{
		str[i]=value%10;
		value=value/10;
		i--;
	}
	if(field_length==-1)
	while(str[j]==0) j++;
	else
	j=5-field_length;
	if(value<0) lcd_i2c_data('-');
	for(i=j;i<5;i++)
	{
		lcd_i2c_data(48+str[i]);
	}
}



void lcd_i2c_printf(const char *format, ...) {
	char buffer[32];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
	//lcd_i2c_printf(buffer);
	lcd_i2c_write_string(buffer);
}

