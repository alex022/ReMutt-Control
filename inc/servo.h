/*
 * servo.h
 *
 *  Created on: May 6, 2014
 *      Author: eric_brunnett
 */

#ifndef SERVO_H_
#define SERVO_H_

# include "LPC177x_8x.h"
# include "macros.h"
#include "pwm.h"
#include "typedef.h"

#define PWM_TICKS 		1200000
#define PWM_MATCH_MIN	.0375*PWM_TICKS
#define PWM_MATCH_MAX	.1125*PWM_TICKS

void servoInit();
void setServoPosDeg(uint8 degrees);
void setServoPosPer(uint8 percent);

#endif /* SERVO_H_ */
