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

void PrintDataAtAddress(unsigned int Address){
	itoh(Address);
	printf(" - ");
	itoh(*(unsigned int*)((unsigned int)Address));
	printf("\n\r");
}

void itoh(unsigned int x){
	char HexData[9];
	int counter = 7;
	int i;
	for(i = 0; i < 9; i++){
		HexData[i] = '0';
	}

	char HexChars[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	while(x != 0){
		HexData[counter] = HexChars[x%16];
		counter --;
		x = x/16;
	}

	HexData[8] = 0x00;
	printf("%s", HexData);
}

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
	//initWiFi(AUTO_CONNECT);							/* initialize WiFi module -- must be attached*/

	if(cameraInit())
		printf("Camera not initialized!\n\r");			/* initialize camera communication */


	// Enter an infinite loop
    while(1) {

    }
    return 0;
}
