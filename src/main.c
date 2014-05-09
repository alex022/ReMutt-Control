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

//SPI main = Daniel's Property
/*int main(void) {
	SSP_CFG_Type SSP_ConfigStruct;

	GPIOSetDir(1,8,GPIO_OUTPUT);
	GPIOSetValue(1,8,0);
	GPIOSetDir(1,14,GPIO_OUTPUT);
	GPIOSetValue(1,14,1);

	SSP_ConfigStructInit(&SSP_ConfigStruct);
	SSP_Init(LPC_SSP2, &SSP_ConfigStruct);
	SSP_Cmd(LPC_SSP2, ENABLE);
	SSP_SendData(LPC_SSP2, 'a');
	SSP_SendData(LPC_SSP2, 'b');
	SSP_SlaveOutputCmd(LPC_SSP2, ENABLE);
	SSP_ReceiveData(LPC_SSP2);

	EMC_DYN_MEM_Config_Type EMC_ConfigStruct;

	DynMem_Init(&EMC_ConfigStruct);
	//EMC_Init();

	// Enter an infinite loop
	   while(1) {
	   }
	   return 0;
}*/

//Testing
int main(void) {
	//Variable Declarations
	TIM_TIMERCFG_Type timerCfg;

	//Initialize timer0 for delays
	TIM_ConfigStructInit(TIM_TIMER_MODE, &timerCfg);		/* initialize timer config struct */
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timerCfg);		/* initialize timer0 */

	// Initialize Peripherals
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
