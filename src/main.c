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
#include <string.h>
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
	//TIM_TIMERCFG_Type timerCfg;
	//SSP_CFG_Type SSP_ConfigStruct;
	EMC_DYN_MEM_Config_Type EMC_ConfigStruct;

	//SPI CS set to active
	//GPIOSetDir(1,8,GPIO_OUTPUT);
	//GPIOSetValue(1,8,0);
	//GPIOSetDir(1,14,GPIO_OUTPUT);
	//GPIOSetValue(1,14,1);

	//SPI Struct Init
	//SSP_ConfigStructInit(&SSP_ConfigStruct);

	//SDRAM Struct Init
	DynMem_Init(&EMC_ConfigStruct);
	//EMC_Init();

	//Initialize timer0 for delays
	//TIM_ConfigStructInit(TIM_TIMER_MODE, &timerCfg);		/* initialize timer config struct */
	//TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timerCfg);		/* initialize timer0 */

	// Initialize Peripherals
	//SSP_Init(LPC_SSP2, &SSP_ConfigStruct);				/* initialize SPI for SD card SSP_Cmd(LPC_SSP2, ENABLE); to send */

	//servoInit();										/* initialize FSR servo motor for panning camera */
	//initStepper();										/* initialize stepper motor for dispensing food */
	//initFSR();											/* initialize force sensitive resistor circuit for food and water full signals */
	//initWiFi(AUTO_CONNECT);							/* initialize WiFi module -- must be attached*/

	//if(cameraInit())
		//printf("Camera not initialized!\n\r");			/* initialize camera communication */


	// Enter an infinite loop
    while(1) {


    }
    return 0;
}
