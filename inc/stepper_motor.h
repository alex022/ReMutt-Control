/*
 * stepper_motor.h
 *
 *  Created on: Apr 23, 2014
 *      Author: eric_brunnett
 */

#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_

/*==============================================================================
 Includes
==============================================================================*/

#include "typedef.h"
#include <stdio.h>
#include "gpio.h"
#include "timer.h"
#include "fsr_circuit.h"

/*==============================================================================
 Defines
==============================================================================*/

/*Phase control signals*/
#define PHASE1_SIG_PORT		PORT4
#define PHASE1_SIG_PIN		15
#define PHASE2_SIG_PORT		PORT4
#define PHASE2_SIG_PIN		16
#define PHASE3_SIG_PORT		PORT4
#define PHASE3_SIG_PIN		17
#define PHASE4_SIG_PORT		PORT4
#define PHASE4_SIG_PIN		18

/*==============================================================================
 Types
==============================================================================*/

/*==============================================================================
 Global function prototypes
==============================================================================*/

/*------------------------------------------------------------------------------
 function name:		initStepper
 description: 		initializes the gpios for the stepper motor
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void initStepper();

/*------------------------------------------------------------------------------
 function name:		setFullStep
 description: 		set motor position to specified location
 parameters:		step position
 returned value:	none
------------------------------------------------------------------------------*/
void setFullStep(uint8 step);

/*------------------------------------------------------------------------------
 function name:		setFullStep
 description: 		prompts user to set motor position
 parameters:		none
 returned value:	number of steps
------------------------------------------------------------------------------*/
int32 promptStep();

/*------------------------------------------------------------------------------
 function name:		setFullStep
 description: 		spins stepper forever
 parameters:		number of steps to spin
 returned value:	none
------------------------------------------------------------------------------*/
void spinStepper(uint32 total_steps);

/*------------------------------------------------------------------------------
 function name:		motorOff
 description: 		turns off all current to motor phases
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void motorOff();

/*------------------------------------------------------------------------------
 function name:		reverseSpin
 description: 		spins stepper in opposite direction for unjamming
 parameters:		number of steps to spin
 returned value:	none
------------------------------------------------------------------------------*/
void reverseSpin(uint32 total_steps);

/*------------------------------------------------------------------------------
 function name:		spinUntilFull
 description: 		spins until full signal asserted
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void spinUntilFull();


#endif /* STEPPER_MOTOR_H_ */
