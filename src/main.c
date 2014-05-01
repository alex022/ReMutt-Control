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
#include "fsr_circuit.h"
#include "stepper_motor.h"
#include "ssp.h"
// TODO: insert other include files here

// TODO: insert other definitions and declarations here

//Testing

int main(void) {
	SSP_Init(LPC_SSP2);
	SSP_SlaveOutputCmd(LPC_SSP2, ENABLE);
	SSP_SendData(LPC_SSP2, 0x11);
	SSP_SlaveOutputCmd(LPC_SSP2, DISABLE);

	SSP_SlaveOutputCmd(LPC_SSP2, ENABLE);
	SSP_ReceiveData(LPC_SSP2);
	SSP_SlaveOutputCmd(LPC_SSP2, DISABLE);
	// Enter an infinite loop
    while(1) {

    }
    return 0;
}
