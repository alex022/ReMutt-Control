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
#include "rtc.h"

int main(void) {
	//Variable Declarations
	TIM_TIMERCFG_Type timerCfg;
	RTC_TIME_Type* feedtime[4];
	RTC_TIME_Type* watertime = malloc(sizeof(RTC_TIME_Type));
	RTC_TIME_Type* time = malloc(sizeof(RTC_TIME_Type));;
	uint8 fed = 0;
	uint8 watered = 0;
	uint8 scheduled_feeds = 0;
	watertime->HOUR = 5;
	watertime->MIN = 0;

	//Initialize timer0 for delays
	TIM_ConfigStructInit(TIM_TIMER_MODE, &timerCfg);	/* initialize timer config struct */
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timerCfg);		/* initialize timer0 */

	//Initialize Real Time Clock
	RTC_Init(LPC_RTC);
	RTC_Cmd(LPC_RTC, ENABLE);
	RTC_ResetClockTickCounter(LPC_RTC);

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

   	    RTC_GetFullTime(LPC_RTC, time);
   	    //Fill water bowl at predetermined time
   	    if (time->HOUR == watertime->HOUR + 1 && watered == 1)
   	    	watered = 0;
   	    if (watertime->HOUR == time->HOUR && watertime->MIN < time->MIN && watered == 0)
   	    {
   	    	fillWater();
   	    	watered = 1;
   	    }
   	    //Feed dog on schedule if any cannot feed dog two consecutive hours
   	    for(i = 0; i < scheduled_feeds; i++)
   	    {
			if (time->HOUR == feedtime[i]->HOUR + 1 && fed == 1)
				fed = 0;
			if (feedtime[i]->HOUR == time->HOUR && feedtime[i]->MIN < time->MIN && fed == 0)
			{
				spinUntilFull();
				fed = 1;
			}
   	    }

    }
    return 0;
}
