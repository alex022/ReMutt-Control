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
		case 0:
			GPIOSetValue(PHASE1_SIG_PORT, PHASE1_SIG_PIN, 1);
			GPIOSetValue(PHASE2_SIG_PORT, PHASE2_SIG_PIN, 1);
			GPIOSetValue(PHASE3_SIG_PORT, PHASE3_SIG_PIN, 0);
			GPIOSetValue(PHASE4_SIG_PORT, PHASE4_SIG_PIN, 0);
			break;
		case 1:
			GPIOSetValue(PHASE1_SIG_PORT, PHASE1_SIG_PIN, 0);
			GPIOSetValue(PHASE2_SIG_PORT, PHASE2_SIG_PIN, 1);
			GPIOSetValue(PHASE3_SIG_PORT, PHASE3_SIG_PIN, 1);
			GPIOSetValue(PHASE4_SIG_PORT, PHASE4_SIG_PIN, 0);
			break;
		case 2:
			GPIOSetValue(PHASE1_SIG_PORT, PHASE1_SIG_PIN, 0);
			GPIOSetValue(PHASE2_SIG_PORT, PHASE2_SIG_PIN, 0);
			GPIOSetValue(PHASE3_SIG_PORT, PHASE3_SIG_PIN, 1);
			GPIOSetValue(PHASE4_SIG_PORT, PHASE4_SIG_PIN, 1);
			break;
		case 3:
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
 description: 		spins stepper until prompted to stop
 parameters:		none
 returned value:	number of steps
------------------------------------------------------------------------------*/
void spinStepper()
{
	int i = 0;
	int step = 0;

	while(1)
	{
		for(i = 0; i < 100; i++)
		{
			printf(".");
		}

		setFullStep(step++);

		if (step == 4)
			step = 0;
	}
}

