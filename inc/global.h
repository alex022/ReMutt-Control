/*
 * global.h
 *
 *  Created on: May 8, 2014
 *      Author: eric_brunnett
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "typedef.h"
#include "rtc.h"

extern uint8 UartRx_interrupt;
extern RTC_TIME_Type* time;
extern RTC_TIME_Type* feedtime[];
extern uint8 scheduled_feeds;
extern uint8 time_set;
extern uint32 byte_rec;

void initTimeStruct();

#endif /* GLOBAL_H_ */
