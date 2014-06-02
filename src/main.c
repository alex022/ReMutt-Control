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
#include "solenoid.h"
#include "audio.h"

//Testing
int main(void) {
	//Variable Declarations
	TIM_TIMERCFG_Type timerCfg;

	//Initialize timer0 for delays
	TIM_ConfigStructInit(TIM_TIMER_MODE, &timerCfg);	/* initialize timer config struct */
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timerCfg);		/* initialize timer0 */

	// Initialize Peripherals
	INIT_SDRAM();										/* initialize SDRAM */
	servoInit();										/* initialize FSR servo motor for panning camera */
	initStepper();										/* initialize stepper motor for dispensing food */
	initFSR();											/* initialize force sensitive resistor circuit for food and water full signals */
	initSolenoid();										/* initialize solenoid valve */
	initWiFi(AUTO_CONNECT);								/* initialize WiFi module -- must be attached*/

	int i = 0, retval;
	uint32 length;										/* length variable for photo */
	printf("Entering while loop\n\r");
	audio_storeVoice();
	// Enter an infinite loop
    while(1) {

    	if(STATE == DISPENSING_FOOD){
    	    printf("Entering food dispense state\n\r");
    	    /* Execute commands to dispense food */
    	    spinUntilFull();
    	    STATE = CONNECTED;
    	}

    	if(STATE == DISPENSING_WATER){
    		printf("Entering water dispense state\n\r");
    		/* Execute commands to dispense water */
    		fillWater();
    		STATE = CONNECTED;
    	   	}

    	if(STATE == CAPTURING){
   	 		printf("Entering camera taking state\n\r");
   	 		/* Initialize camera and set it up to take a picture */
   	 		if(cameraInit())
   	 			printf("Camera not initialized!\n\r");
   	 		retval = stopFrame();
   	 		length = getBufferLength();
   	 		printf("length: %i\n\r", length);

   	 		/* Send length to Android application */
   	 		int temp_len = length;
   	 		while(temp_len){
   	 			uart1PutChar(temp_len % 10);
   	 			temp_len = temp_len / 10;
   	 		}

   	 		/* Send photo and finish set up */
   	 		getAndSendPhoto(length);
   	 		resumeFrame();
   	 		STATE = CONNECTED;
  	   	}

   	    if(STATE == TALKING){
       		printf("Entering audio output state\n\r");
       		/* Execute commands to output audio file */

       		STATE = CONNECTED;
	    }

   	    if(STATE == PAN_LEFT){
   	    	/* Execute commands to pan servo left */
   	    	panServo(LEFT);
       		STATE = CONNECTED;
   	    }

   	    if(STATE == PAN_RIGHT){
   	    	/* Execute commands to pan servo right */
   	    	panServo(RIGHT);
       		STATE = CONNECTED;
   	    }

   	    if(STATE == SCHEDULING){
       		/* Execute commands to schedule a feeding time */

       		STATE = CONNECTED;
   	    }
    }
    return 0;
}
