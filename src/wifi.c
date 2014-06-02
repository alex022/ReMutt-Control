/*
 * wifi.c
 *
 *  Created on: Apr 30, 2014
 *      Author: stevenguan
 */

#include "uart.h"
#include "wifi.h"
#include <stdio.h>
#include <string.h>
#include <gpio.h>
#include "stepper_motor.h"

void initWiFi(int setting){

	/* initialize local variables */
	int i;
	int length;
	int retval;
	char cmd[MAX_SIZE] = " ";

	/* set global WiFi variables */
	iterator = 0;
	STATE = IDLE;
	check_flag = 0;
	message = "";
	/* initialize UART1 */
	uart1Init(uartBAUD9600, uartSTOP_BIT_1, uartPARITY_DISABLE);
	GPIOSetDir(4, 20, 1);
	GPIOSetValue(4, 20, 0);
	struct uartIrqConf u1;
	u1.rxTriggerLvl = uartTRIGER_LEVEL_1CHAR;
	u1.priority = irqPRIORITY10;
	uart1InitIrq(&u1);
	uart1EnableIrqRx();
	if(setting == CMD_MODE){
		printf("Entering WiFi command mode.\n\r");
		while(1){
			for(i = 0; i < 50; i++)
				cmd[i] = "";
			cmd[0] = '\n';
			scanf("%s", &cmd);
			length = strlen(cmd);
			if(cmd[0] != '$' || cmd[1] != '$' || cmd[2] != '$'){
				for(i = 0; i < length; i++){
					if(cmd[i] == '$')
						cmd[i] = ' ';
				}
				cmd[length] = '\r';
				length++;
			}
			cmd[length] = '\n';
			printf("cmd: %s\n\r", cmd);
			for(i = 0; i < length; i++)
				uart1PutChar(cmd[i]);
			i = 0;
		}
	}
	else if(setting == AUTO_CONNECT){
		printf("Automatically connecting to the configured AP.\n\r");
		uart1PutChar('$');
		uart1PutChar('$');
		uart1PutChar('$');
		printf("$$$: ");
		for(i = 0; i < 1000000; i++){
			retval = uart1Getchar();
			if(retval != -1)
				printf("%c", retval);
		}
		printf("\n\r");
		uart1PutChar('r');
		uart1PutChar('e');
		uart1PutChar('b');
		uart1PutChar('o');
		uart1PutChar('o');
		uart1PutChar('t');
		uart1PutChar('\r');
		printf("reboot1: ");
		for(i = 0; i < 1000000; i++){
			retval = uart1Getchar();
			if(retval != -1)
				printf("%c", retval);
		}
		printf("\n\r");
		uart1PutChar('r');
		uart1PutChar('e');
		uart1PutChar('b');
		uart1PutChar('o');
		uart1PutChar('o');
		uart1PutChar('t');
		uart1PutChar('\r');
		printf("\n\rWiFi should connect to the AP if it is in range\n\r");

	}
}

/* iterate through the circular array and find the message that is sandwiched by *'s */
void getMessage(char string[]){
	int start;
	int i, stop;
	char message[10];
	int message_index = 0;
	printf("\n\rgetMessage:");
	i = (iterator - 1) % WIFI_BUFF_SIZE;
	stop = (iterator - 1) % WIFI_BUFF_SIZE;
	if(i < 0){
		i = i + WIFI_BUFF_SIZE;
		stop = stop + WIFI_BUFF_SIZE;
	}
	while(1){
		if(i <= 0)
			i = WIFI_BUFF_SIZE;
		i--;
		if(string[i] == '*'){
			start = (i+1) % WIFI_BUFF_SIZE;
			break;
		}
	}
	while(start != stop){
		message[message_index] = string[start];
		message_index++;
		printf("%c", string[start]);
		start++;
		if(start >= (WIFI_BUFF_SIZE))
			start = 0;
	}
	/* message[] now contains the message */
	message[message_index] = '\0';

	/* Check if we need to open or close the connection */
	if(strcmp(message, "OPEN") == 0)
		STATE = CONNECTED;
	else if(strcmp(message, "CLOS") == 0)
		STATE = IDLE;

	/* If we're connected, check if we need to move into an action state */
	if( (strcmp(message, "FOOD") == 0) && STATE == CONNECTED)
		STATE = DISPENSING_FOOD;
	else if( (strcmp(message, "WATER") == 0) && STATE == CONNECTED)
		STATE = DISPENSING_WATER;
	else if( (strcmp(message, "PICTURE") == 0) && STATE == CONNECTED) {
		STATE = CAPTURING;
	}
	else if( (strcmp(message, "AUDIO") == 0) && STATE == CONNECTED)
		STATE = TALKING;
	else if( (strcmp(message, "SCHEDULE") == 0) && STATE == CONNECTED)
		STATE = SCHEDULING;
	else if( (strcmp(message, "PANLEFT") == 0) && STATE == CONNECTED)
		STATE = PAN_LEFT;
	else if( (strcmp(message, "PANRIGHT") == 0) && STATE == CONNECTED)
		STATE = PAN_RIGHT;
	printf("\n\r");
}
