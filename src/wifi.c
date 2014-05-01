/*
 * wifi.c
 *
 *  Created on: Apr 30, 2014
 *      Author: stevenguan
 */

#include "wifi.h"
#include <stdio.h>

void initWiFi(int setting){
	uart1Init(uartBAUD9600, uartSTOP_BIT_1, uartPARITY_DISABLE);
	if(setting == CMD_MODE){
		printf("Entering WiFi command mode\n\r");
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
		printf("Automatically connecting to AP...no code here yet though lol\n\r");
	}
}
