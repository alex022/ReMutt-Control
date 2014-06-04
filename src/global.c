/*
 * global.c
 *
 *  Created on: May 8, 2014
 *      Author: eric_brunnett
 */
#include "global.h"

uint8 UartRx_interrupt = 0;
RTC_TIME_Type* time;
uint8 time_set = 0;
RTC_TIME_Type* feedtime[4];
uint8 scheduled_feeds = 0;
uint32 byte_rec = 0;

void initTimeStruct()
{
	time = malloc(sizeof(RTC_TIME_Type));
}
