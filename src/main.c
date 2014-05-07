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
#include "servo.h"
#include "timer.h"
#include "camera.h"
#include "ssp.h"
#include "wifi.h"

//Testing

int main(void) {
	//Variable Declarations

	// Initialize Peripherals
	servoInit();							/* initialize FSR servo motor for panning camera */
	initStepper();							/* initialize stepper motor for dispensing food */
	initFSR();								/* initialize force sensitive resistor circuit for food and water full signals */
	initWiFi(AUTO_CONNECT);					/* initialize WiFi module */
	cameraInit();							/* initialize camera communication */

	// Enter an infinite loop
    while(1) {

    }
    return 0;
}
