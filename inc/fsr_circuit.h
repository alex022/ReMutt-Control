/*
 * fsr_circuit.h
 *
 *  Created on: Apr 22, 2014
 *      Author: eric_brunnett
 */

#ifndef FSR_CIRCUIT_H_
#define FSR_CIRCUIT_H_

/*==============================================================================
 Includes
==============================================================================*/

#include "gpio.h"
#include "typedef.h"
#include <stdio.h>

/*==============================================================================
 Defines
==============================================================================*/

/*Signal Ordering*/
#define WATER_FULL		1
#define FOOD_FULL		2


/*Signal Port/Pin Assignments*/
#define WATER_FULL_SIG_PORT		PORT3
#define WATER_FULL_SIG_PIN		30

#define FOOD_FULL_SIG_PORT		PORT3
#define FOOD_FULL_SIG_PIN		31

/*DCP control signals*/
#define FOOD_FULL_INC_PORT		PORT5
#define FOOD_FULL_INC_PIN		0
#define FOOD_FULL_UD_PORT		PORT5
#define FOOD_FULL_UD_PIN		1
#define FOOD_FULL_CS_PORT		PORT5
#define FOOD_FULL_CS_PIN		2

#define WATER_FULL_INC_PORT		PORT3
#define WATER_FULL_INC_PIN		21
#define WATER_FULL_UD_PORT		PORT3
#define WATER_FULL_UD_PIN		22
#define WATER_FULL_CS_PORT		PORT3
#define WATER_FULL_CS_PIN		23

/*==============================================================================
 Types
==============================================================================*/

/*==============================================================================
 Global function prototypes
==============================================================================*/
/*------------------------------------------------------------------------------
 function name:		initFSR
 description: 		initializes the gpios for the load sensor circuits
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void initFSR();

/*------------------------------------------------------------------------------
 function name:		setDCPVoltage
 description: 		set the digitally controlled potentiometer for a given load
 	 	 	 	 	 signal to a desired voltage
 parameters:		desired voltage, DCP signal select
 returned value:	none
------------------------------------------------------------------------------*/
//void setDCPVoltage (int voltage, uint8 signal);

/*------------------------------------------------------------------------------
 function name:		promptIncrementDCP
 description: 		prompts the user if they wish to increment/decrement
 	 	 	 	 	 specified DCP
 parameters:		DCP to increment
 returned value:	number of increments (negative = number of decrements)
------------------------------------------------------------------------------*/
int32 promptIncrementDCP(uint8 dcp);

/*------------------------------------------------------------------------------
 function name:		getLoadSignal
 description: 		get the specified load signal
 parameters:		which signal (FOOD_FULL/WATER_FULL/WATER_EMPTY)
 returned value:	gpio value
------------------------------------------------------------------------------*/
uint8 getLoadSignal(uint8 dcp);

#endif /* FSR_CIRCUIT_H_ */
