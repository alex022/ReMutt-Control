/*
 * stepper_motor.c
 *
 *  Created on: Apr 23, 2014
 *      Author: eric_brunnett
 */

/*==============================================================================
 Includes
==============================================================================*/

#include "stepper_motor.h"

/*==============================================================================
 Global function definitions
==============================================================================*/

/*------------------------------------------------------------------------------
 function name:		initStepper
 description: 		initializes the gpios for the stepper motor
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void initStepper()
{

	//Set phase control signals to outputs
	GPIOSetDir(PHASE1_SIG_PORT, PHASE1_SIG_PIN, GPIO_OUTPUT);
	GPIOSetDir(PHASE2_SIG_PORT, PHASE2_SIG_PIN, GPIO_OUTPUT);
	GPIOSetDir(PHASE3_SIG_PORT, PHASE3_SIG_PIN, GPIO_OUTPUT);
	GPIOSetDir(PHASE4_SIG_PORT, PHASE4_SIG_PIN, GPIO_OUTPUT);

	//Set initial control signal values
	GPIOSetValue(PHASE1_SIG_PORT, PHASE1_SIG_PIN, 1);
	GPIOSetValue(PHASE2_SIG_PORT, PHASE2_SIG_PIN, 1);
	GPIOSetValue(PHASE3_SIG_PORT, PHASE3_SIG_PIN, 0);
	GPIOSetValue(PHASE4_SIG_PORT, PHASE4_SIG_PIN, 0);

}

/*------------------------------------------------------------------------------
 function name:		setFullStep
 description: 		set motor position to specified location
 parameters:		step position
 returned value:	none
------------------------------------------------------------------------------*/
void setFullStep(uint8 step)
{
	switch (step)
	{
		case 3:
			GPIOSetValue(PHASE1_SIG_PORT, PHASE1_SIG_PIN, 1);
			GPIOSetValue(PHASE2_SIG_PORT, PHASE2_SIG_PIN, 1);
			GPIOSetValue(PHASE3_SIG_PORT, PHASE3_SIG_PIN, 0);
			GPIOSetValue(PHASE4_SIG_PORT, PHASE4_SIG_PIN, 0);
			break;
		case 2:
			GPIOSetValue(PHASE1_SIG_PORT, PHASE1_SIG_PIN, 0);
			GPIOSetValue(PHASE2_SIG_PORT, PHASE2_SIG_PIN, 1);
			GPIOSetValue(PHASE3_SIG_PORT, PHASE3_SIG_PIN, 1);
			GPIOSetValue(PHASE4_SIG_PORT, PHASE4_SIG_PIN, 0);
			break;
		case 1:
			GPIOSetValue(PHASE1_SIG_PORT, PHASE1_SIG_PIN, 0);
			GPIOSetValue(PHASE2_SIG_PORT, PHASE2_SIG_PIN, 0);
			GPIOSetValue(PHASE3_SIG_PORT, PHASE3_SIG_PIN, 1);
			GPIOSetValue(PHASE4_SIG_PORT, PHASE4_SIG_PIN, 1);
			break;
		case 0:
			GPIOSetValue(PHASE1_SIG_PORT, PHASE1_SIG_PIN, 1);
			GPIOSetValue(PHASE2_SIG_PORT, PHASE2_SIG_PIN, 0);
			GPIOSetValue(PHASE3_SIG_PORT, PHASE3_SIG_PIN, 0);
			GPIOSetValue(PHASE4_SIG_PORT, PHASE4_SIG_PIN, 1);
			break;
	}
}

/*------------------------------------------------------------------------------
 function name:		setFullStep
 description: 		prompts user to set motor position
 parameters:		none
 returned value:	number of steps
------------------------------------------------------------------------------*/
int32 promptStep()
{
	uint8 input;
	uint8 step;
	int32 steps = 0;
	do
	{
		printf("Enter Step (1-4) or 0 to Quit:");
		scanf("%d", &input);
		printf(" %d\n\r", input);
		if (input == 1 || input == 2 || input == 3 || input == 4)
		{
			step = input - 1;
			setFullStep(step);
			steps++;
		}
	} while(input == 1 || input == 2 || input == 3 || input == 4);

	return steps;
}

/*------------------------------------------------------------------------------
 function name:		setFullStep
 description: 		spins stepper for a certain number of steps
 parameters:		number of steps to spin
 returned value:	none
------------------------------------------------------------------------------*/
void spinStepper(uint32 total_steps)
{
	uint32 step = 0;
	uint32 j = 0;

	while(j < total_steps)
	{

		TIM_Waitms(20);

		setFullStep(step++);

		if (step == 4)
			step = 0;
		j++;
	}
	motorOff();
}

/*------------------------------------------------------------------------------
 function name:		reverseSpin
 description: 		spins stepper in opposite direction for unjamming
 parameters:		number of steps to spin
 returned value:	none
------------------------------------------------------------------------------*/
void reverseSpin(uint32 total_steps)
{
	uint32 step = 0;
	uint32 j = 0;

	while(j < total_steps)
	{

		TIM_Waitms(20);

		setFullStep(step--);

		if (step == 0)
			step = 4;
		j++;
	}
	motorOff();
}

/*------------------------------------------------------------------------------
 function name:		motorOff
 description: 		turns off all current to motor
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void motorOff()
{
	GPIOSetValue(PHASE1_SIG_PORT, PHASE1_SIG_PIN, 0);
	GPIOSetValue(PHASE2_SIG_PORT, PHASE2_SIG_PIN, 0);
	GPIOSetValue(PHASE3_SIG_PORT, PHASE3_SIG_PIN, 0);
	GPIOSetValue(PHASE4_SIG_PORT, PHASE4_SIG_PIN, 0);
}

/*------------------------------------------------------------------------------
 function name:		spinUntilFull
 description: 		spins until full signal asserted
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void spinUntilFull()
{
	uint8 step = 0;
	uint16 total_steps = 0;

	while(!getLoadSignal(FOOD_FULL))
	{

		TIM_Waitms(20);

		if (total_steps++%1000 == 999)
			reverseSpin(50);

		setFullStep(step++);

		if (step == 4)
			step = 0;
	}
	motorOff();
}
