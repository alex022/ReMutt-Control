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
/*==============================================================================
 Defines
==============================================================================*/
/*Signal Ordering*/
#define WATER_FULL		1
#define WATER_EMPTY		2
#define FOOD_FULL		3


/*Signal Port/Pin Assignments*/
#define WATER_FULL_SIG_PORT		PORT3
#define WATER_FULL_SIG_PIN		29

#define WATER_EMPTY_SIG_PORT	PORT3
#define WATER_EMPTY_SIG_PIN		30

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

#define WATER_EMPTY_INC_PORT	PORT3
#define WATER_EMPTY_INC_PIN		18
#define WATER_EMPTY_UD_PORT		PORT3
#define WATER_EMPTY_UD_PIN		19
#define WATER_EMPTY_CS_PORT		PORT3
#define WATER_EMPTY_CS_PIN		20

//TODO: Water full and water empty control signals

/*==============================================================================
 Types
==============================================================================*/

/*==============================================================================
 Global function prototypes
==============================================================================*/
/*------------------------------------------------------------------------------
 function name:		initFSR
 description: 		initializes the gpios for the load sensor circuits
 parameters:		desired voltage, stop bit, parity
 returned value:	none
------------------------------------------------------------------------------*/
void initFSR();

/*------------------------------------------------------------------------------
 function name:		setDCPVoltage
 description: 		set the digitally controlled potentiometer for a given load
 	 	 	 	 	 signal to a desired voltage
 parameters:		desired voltage, stop bit, parity
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

#endif /* FSR_CIRCUIT_H_ */
