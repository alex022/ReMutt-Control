/*
 * solenoid.h
 *
 *  Created on: May 13, 2014
 *      Author: eric_brunnett
 */

#ifndef SOLENOID_H_
#define SOLENOID_H_

#include "gpio.h"
#include "fsr_circuit.h"

#define SOLENOID_PORT	PORT4
#define SOLENOID_PIN	19

void initSolenoid();
void solenoidOpen();
void solenoidClose();
void fillWater();

#endif /* SOLENOID_H_ */
