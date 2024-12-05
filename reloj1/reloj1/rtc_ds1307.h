/*
 * rtc_ds1307.h
 *
 * Created: 05/12/2024 02:05:32 a. m.
 *  Author: andro
 */ 


#ifndef RTC_DS1307_H_
#define RTC_DS1307_H_
#include <avr/io.h>

typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
} Time;

typedef struct {
	uint8_t day;
	uint8_t month;
	uint16_t year;
} Date;


void rtc_init(void);
void rtc_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds);
Time rtc_get_time(void);
void rtc_set_date(uint8_t day, uint8_t month, uint16_t year);
Date rtc_get_date(void);




#endif /* RTC_DS1307_H_ */