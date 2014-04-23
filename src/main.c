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
// TODO: insert other include files here

// TODO: insert other definitions and declarations here

//Testing

int main(void) {
	initFSR();
	initStepper();

	// Enter an infinite loop
    while(1) {

    }
    return 0;
}
