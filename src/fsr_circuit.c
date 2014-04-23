/*
 * fsr_circuit.c
 *
 *  Created on: Apr 22, 2014
 *      Author: eric_brunnett
 */

/*==============================================================================
 Includes
==============================================================================*/

#include "fsr_circuit.h"
#include "typedef.h"
#include <stdio.h>

/*==============================================================================
 Global function definitions
==============================================================================*/

/*------------------------------------------------------------------------------
 function name:		initFSR
 description: 		initializes the gpios for the load sensor circuits
 parameters:		desired voltage, stop bit, parity
 returned value:	none
------------------------------------------------------------------------------*/
void initFSR()
{
	//Set load signals to inputs
	GPIOSetDir(WATER_FULL_SIG_PORT, WATER_FULL_SIG_PIN, GPIO_INPUT);
	GPIOSetDir(WATER_EMPTY_SIG_PORT, WATER_EMPTY_SIG_PIN, GPIO_INPUT);
	GPIOSetDir(FOOD_FULL_SIG_PORT, FOOD_FULL_SIG_PIN, GPIO_INPUT);

	//Set DCP control signals to outputs
	GPIOSetDir(FOOD_FULL_CS_PORT, FOOD_FULL_CS_PIN, GPIO_OUTPUT);
	GPIOSetDir(FOOD_FULL_INC_PORT, FOOD_FULL_INC_PIN, GPIO_OUTPUT);
	GPIOSetDir(FOOD_FULL_UD_PORT, FOOD_FULL_UD_PIN, GPIO_OUTPUT);

	GPIOSetDir(WATER_FULL_CS_PORT, WATER_FULL_CS_PIN, GPIO_OUTPUT);
	GPIOSetDir(WATER_FULL_INC_PORT, WATER_FULL_INC_PIN, GPIO_OUTPUT);
	GPIOSetDir(WATER_FULL_UD_PORT, WATER_FULL_UD_PIN, GPIO_OUTPUT);

	GPIOSetDir(WATER_EMPTY_CS_PORT, WATER_EMPTY_CS_PIN, GPIO_OUTPUT);
	GPIOSetDir(WATER_EMPTY_INC_PORT, WATER_EMPTY_INC_PIN, GPIO_OUTPUT);
	GPIOSetDir(WATER_EMPTY_UD_PORT, WATER_EMPTY_UD_PIN, GPIO_OUTPUT);

	//Set initial control signal values
	GPIOSetValue(FOOD_FULL_CS_PORT, FOOD_FULL_CS_PIN, 1);
	GPIOSetValue(FOOD_FULL_INC_PORT, FOOD_FULL_INC_PIN, 1);
	GPIOSetValue(FOOD_FULL_UD_PORT, FOOD_FULL_UD_PIN, 1);

	GPIOSetValue(WATER_FULL_CS_PORT, WATER_FULL_CS_PIN, 1);
	GPIOSetValue(WATER_FULL_INC_PORT, WATER_FULL_INC_PIN, 1);
	GPIOSetValue(WATER_FULL_UD_PORT, WATER_FULL_UD_PIN, 1);

	GPIOSetValue(WATER_EMPTY_CS_PORT, WATER_EMPTY_CS_PIN, 1);
	GPIOSetValue(WATER_EMPTY_INC_PORT, WATER_EMPTY_INC_PIN, 1);
	GPIOSetValue(WATER_EMPTY_UD_PORT, WATER_EMPTY_UD_PIN, 1);

}

/*------------------------------------------------------------------------------
 function name:		setDCPVoltage
 description: 		set the digitally controlled potentiometer for a given load
 	 	 	 	 	 signal to a desired voltage
 parameters:		desired voltage, stop bit, parity
 returned value:	none
------------------------------------------------------------------------------
void setDCPVoltage (int voltage, uint8 signal)
{
	int i = voltage;
	while(i > 0)
	{
		switch(signal)
		{
			case WATER_FULL:

				break;
			case WATER_EMPTY:

				break;
			case FOOD_FULL:

				break;
			default:

				break;
		}
		i--;
	}
}
*/

/*------------------------------------------------------------------------------
 function name:		promptIncrementDCP
 description: 		prompts the user if they wish to increment/decrement
 	 	 	 	 	 specified DCP
 parameters:		DCP to increment
 returned value:	number of increments (negative = number of decrements)
------------------------------------------------------------------------------*/
int32 promptIncrementDCP(uint8 dcp)
{
	int32 increments = 0;
	uint8 input;
	do
	{
		printf("Increment Up(u) or Down(d) or Quit(q)?");
		scanf("%c", &input);
		printf(" %c\n\r", input);
		if (input == 'd' || input == 'D' || input == 'u' || input == 'U')
		{
			if (input == 'd' || input == 'D')
			{
				switch(dcp)
				{
					case WATER_FULL:
						GPIOSetValue(WATER_FULL_UD_PORT, WATER_FULL_UD_PIN, 0);
						break;
					case WATER_EMPTY:
						GPIOSetValue(WATER_EMPTY_UD_PORT, WATER_EMPTY_UD_PORT, 0);
						break;
					case FOOD_FULL:
						GPIOSetValue(FOOD_FULL_UD_PORT, FOOD_FULL_UD_PIN, 0);
						break;
				}
				increments--;
			}
			else
			{
				switch(dcp)
				{
					case WATER_FULL:
						GPIOSetValue(WATER_FULL_UD_PORT, WATER_FULL_UD_PIN, 1);
						break;
					case WATER_EMPTY:
						GPIOSetValue(WATER_EMPTY_UD_PORT, WATER_EMPTY_UD_PORT, 1);
						break;
					case FOOD_FULL:
						GPIOSetValue(FOOD_FULL_UD_PORT, FOOD_FULL_UD_PIN, 1);
						break;
				}
				increments++;
			}
			switch(dcp)
			{
				case WATER_FULL:
					GPIOSetValue(WATER_FULL_INC_PORT, WATER_FULL_INC_PIN, 0);
					GPIOSetValue(WATER_FULL_INC_PORT, WATER_FULL_INC_PIN, 1);
					break;
				case WATER_EMPTY:
					GPIOSetValue(WATER_EMPTY_INC_PORT, WATER_EMPTY_INC_PORT, 0);
					GPIOSetValue(WATER_EMPTY_INC_PORT, WATER_EMPTY_INC_PORT, 1);
					break;
				case FOOD_FULL:
					GPIOSetValue(FOOD_FULL_INC_PORT, FOOD_FULL_INC_PIN, 0);
					GPIOSetValue(FOOD_FULL_INC_PORT, FOOD_FULL_INC_PIN, 1);
					break;
			}
		}
	}while(input == 'd' || input == 'D' || input == 'u' || input == 'U');

	return increments;
}
