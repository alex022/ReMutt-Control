/*
 * wifi.c
 *
 *  Created on: Apr 30, 2014
 *      Author: stevenguan
 */

#include "wifi.h"
#include <stdio.h>
#include <string.h>
#include <gpio.h>

void initWiFi(int setting){
	int i;
	int length;
	int retval;
	char cmd[MAX_SIZE] = " ";
	uart1Init(uartBAUD9600, uartSTOP_BIT_1, uartPARITY_DISABLE);
	GPIOSetDir(4, 20, 1);
	GPIOSetValue(4, 20, 0);
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
			printf("response: ");
			while(i < 10000){
			   	retval = uart1Getchar();
			   	if(retval == -1)
			   		i++;
			   	else
			   		printf("%c", retval);
			   	if(retval == '>')
			   		break;
			}
			printf("\n\r");
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
		printf("reboot1: ");
		for(i = 0; i < 1000000; i++){
			retval = uart1Getchar();
			if(retval != -1)
				printf("%c", retval);
		}
		printf("\n\r");
		uart1PutChar('$');
		uart1PutChar('$');
		uart1PutChar('$');
		printf("$$$: ");
		for(i = 0; i < 10000000; i++){
			retval = uart1Getchar();
			if(retval != -1)
				printf("%c", retval);
		}
		printf("\n\r");
		uart1PutChar('l');
		uart1PutChar('e');
		uart1PutChar('a');
		uart1PutChar('v');
		uart1PutChar('e');
		uart1PutChar('\r');
		printf("reboot1: ");
		for(i = 0; i < 1000000; i++){
			retval = uart1Getchar();
			if(retval != -1)
				printf("%c", retval);
		}
		printf("\n\r");
		uart1PutChar('j');
		uart1PutChar('o');
		uart1PutChar('i');
		uart1PutChar('n');
		uart1PutChar(' ');
		uart1PutChar('e');
		uart1PutChar('c');
		uart1PutChar('e');
		uart1PutChar('1');
		uart1PutChar('8');
		uart1PutChar('9');
		uart1PutChar('\r');
		printf("reboot2: ");
		for(i = 0; i < 10000000; i++){
			retval = uart1Getchar();
			if(retval != -1){
				printf("%c", retval);
				i--;
			}
		}
		printf("\n\rWiFi should connect to the AP if it is in range\n\r");

	}
}
