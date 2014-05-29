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
#include "sdram.h"
#include "pinsel.h"

//Testing
int main(void) {
	//Variable Declarations
	TIM_TIMERCFG_Type timerCfg;
	EMC_DYN_MEM_Config_Type EMC_ConfigStruct;

	//Initialize timer0 for delays
	TIM_ConfigStructInit(TIM_TIMER_MODE, &timerCfg);		/* initialize timer config struct */
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timerCfg);		/* initialize timer0 */

	// Initialize Peripherals
	/*initialize SDRAM for memory */
	DynMem_Init(&EMC_ConfigStruct);
	servoInit();										/* initialize FSR servo motor for panning camera */
	initStepper();										/* initialize stepper motor for dispensing food */
	initFSR();											/* initialize force sensitive resistor circuit for food and water full signals */
	initWiFi(AUTO_CONNECT);							/* initialize WiFi module -- must be attached*/

	if(cameraInit())
		printf("Camera not initialized!\n\r");			/* initialize camera communication */


	// Enter an infinite loop
    while(1) {
    	if(STATE == DISPENSING_FOOD){
    	    printf("Entering food dispense state\n\r");
    	    STATE = CONNECTED;
    	}
    	if(STATE == DISPENSING_WATER){
    		printf("Entering water dispense state\n\r");
    		STATE = CONNECTED;
    	   	}
    	if(STATE == CAPTURING){
   	 		printf("Entering camera taking state\n\r");
      		STATE = CONNECTED;
  	   	}
   	    if(STATE == TALKING){
       		printf("Entering audio output state\n\r");
       		STATE = CONNECTED;
	    }
    }
    return 0;
}
