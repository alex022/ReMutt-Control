/*
 * wifi.h
 *
 *  Created on: Apr 30, 2014
 *      Author: stevenguan
 */

#ifndef WIFI_H_
#define WIFI_H_

#include "uart.h"

/* WiFi Macros */
#define AUTO_CONNECT 1
#define CMD_MODE 2
#define MAX_SIZE 50
#define WIFI_BUFF_SIZE 10

/* State Variable, buffer iterator */
int STATE, iterator;

/* Flag to check for a new connection */
int check_flag;
int message;

/* Buffer to hold individual messages */
char buffer[WIFI_BUFF_SIZE];

/* Enumeration for state */
enum{
	IDLE, 				/* IDLE state, currently no connections */
	CONNECTED,			/* CONNECTED state, Android application is connected but currently doing nothing */
	DISPENSING_FOOD,	/* CONNECTED state, performing a food dispense command */
	DISPENSING_WATER,	/* CONNECTED state, performing a water dispense command */
	CAPTURING,			/* CONNCETED state, performing a picture command */
	TALKING, 			/* CONNCETED state, performing audio command */
	SCHEDULING			/* CONNECTED state, performing schedule change */
};

/* Enumeration for message */
enum{
	START, OPEN, CLOSED
};

/* Enumeration for dispensing */
enum{
	DISPENSING_FOOD, DISPENSING_WATER
};

/* Enumeration for comm */
enum{
	CAPTURING, TALKING
};

/* Function prototypes */
void initWiFi(int setting);
void getMessage(char string[]);

#endif /* WIFI_H_ */
