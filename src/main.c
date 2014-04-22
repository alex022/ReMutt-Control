/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC177x_8x.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include "uart.h"
#include "fsr_circuit.h"
// TODO: insert other include files here

// TODO: insert other definitions and declarations here

//Testing

int main(void) {
	initFSR();
	char option;
    // Enter an infinite loop
    while(1) {
    	printf("Increment DCP?\n\r");
		scanf("%c", &option);
    	printf("%c\n\r", option);
    	if (option == 'u' || option == 'd' || option == 'U' || option == 'D')
    	{
    		if (option == 'd')
    		{
    			GPIOSetValue(FOOD_FULL_UD_PORT, FOOD_FULL_UD_PIN, 0);
    		}
    		else
    		{
    			GPIOSetValue(FOOD_FULL_UD_PORT, FOOD_FULL_UD_PIN, 1);
    		}
			GPIOSetValue(FOOD_FULL_INC_PORT, FOOD_FULL_INC_PIN, 0);
			GPIOSetValue(FOOD_FULL_INC_PORT, FOOD_FULL_INC_PIN, 1);
    	}

    }
    return 0;
}
