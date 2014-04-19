#include <stdio.h>
#include "uart.h"

//use UART for printf
int __sys_write(int iFileHandle, char *pcBuffer, int iLength)
{
	uart0Init(uartBAUD9600, uartSTOP_BIT_1, uartPARITY_DISABLE);
	uart0Tx((uint8*) pcBuffer,iLength+1);
	return iLength;
}


int __sys_readc(void)
{
	char pcBuffer;
	uart0Rx((uint8*)&pcBuffer, 1);
	return (int) pcBuffer;
}

