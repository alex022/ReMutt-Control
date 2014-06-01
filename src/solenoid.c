/*
 * solenoid.c
 *
 *  Created on: May 13, 2014
 *      Author: eric_brunnett
 */


#include "solenoid.h";


void initSolenoid()
{
	GPIOSetDir(SOLENOID_PORT,SOLENOID_PIN, GPIO_OUTPUT);
	solenoidClose();
}

void solenoidOpen()
{
	GPIOSetDir(SOLENOID_PORT,SOLENOID_PIN, 0);
}

void solenoidClose()
{
	GPIOSetDir(SOLENOID_PORT,SOLENOID_PIN, 1);
}

void fillWater()
{
	solenoidOpen();
	while(!getLoadSignal(WATER_FULL));
	solenoidClose();
}
