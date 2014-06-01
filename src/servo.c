/*
 * servo.c
 *
 *  Created on: May 6, 2014
 *      Author: eric_brunnett
 */
#include "servo.h"

void servoInit()
{

	uint8 pwmChannel;

	PWM_TIMERCFG_Type configStruct;
	PWM_MATCHCFG_Type	PWMMatchCfgDat;

	LPC_IOCON->P1_18 = 0;
	LPC_IOCON->P1_18 = SET_BIT(1);

	PWM_ConfigStructInit(PWM_MODE_TIMER, &configStruct);
	PWM_Init(PWM_1,PWM_MODE_TIMER,&configStruct);
	PWM_MatchUpdate(PWM_1, 0, PWM_TICKS + PWM_MATCH_MIN, PWM_MATCH_UPDATE_NOW);
	PWM_MatchUpdate(PWM_1, 1, PWM_MATCH_MIN, PWM_MATCH_UPDATE_NOW);

	PWMMatchCfgDat.IntOnMatch = DISABLE;
	PWMMatchCfgDat.MatchChannel = 0;
	PWMMatchCfgDat.ResetOnMatch = ENABLE;
	PWMMatchCfgDat.StopOnMatch = DISABLE;
	PWM_ConfigMatch(PWM_1, &PWMMatchCfgDat);

	// Configure match option
	PWMMatchCfgDat.IntOnMatch = DISABLE;
	PWMMatchCfgDat.MatchChannel = 1;
	PWMMatchCfgDat.ResetOnMatch = DISABLE;
	PWMMatchCfgDat.StopOnMatch = DISABLE;
	PWM_ConfigMatch(PWM_1, &PWMMatchCfgDat);
	PWM_ChannelCmd(PWM_1, 1, ENABLE);

	PWM_ResetCounter(PWM_1);
	PWM_CounterCmd(PWM_1, ENABLE);
	PWM_Cmd(PWM_1, ENABLE);

}

void setServoPosDeg(uint8 degrees)
{
	uint32 channelVal = PWM_MATCH_MIN + ((float)degrees / 180) * (PWM_MATCH_MAX - PWM_MATCH_MIN);

	if (channelVal > PWM_MATCH_MAX)
		channelVal = PWM_MATCH_MAX;
	else if (channelVal < PWM_MATCH_MIN)
		channelVal = PWM_MATCH_MIN;

	PWM_MatchUpdate(PWM_1, 0, channelVal + PWM_TICKS, PWM_MATCH_UPDATE_NOW);
	PWM_MatchUpdate(PWM_1, 1, channelVal, PWM_MATCH_UPDATE_NOW);

	PWM_ResetCounter(PWM_1);
	PWM_CounterCmd(PWM_1, ENABLE);
	PWM_Cmd(PWM_1, ENABLE);
}

void setServoPosPer(uint8 percent)
{
	uint32 channelVal = PWM_MATCH_MIN + ((float)percent / 100) * (PWM_MATCH_MAX - PWM_MATCH_MIN);

	if (channelVal > PWM_MATCH_MAX)
		channelVal = PWM_MATCH_MAX;
	else if (channelVal < PWM_MATCH_MIN)
		channelVal = PWM_MATCH_MIN;

	PWM_MatchUpdate(PWM_1, 0, channelVal + PWM_TICKS, PWM_MATCH_UPDATE_NOW);
	PWM_MatchUpdate(PWM_1, 1, channelVal, PWM_MATCH_UPDATE_NOW);

	PWM_ResetCounter(PWM_1);
	PWM_CounterCmd(PWM_1, ENABLE);
	PWM_Cmd(PWM_1, ENABLE);
}


uint8 panServo(uint8 direction)
{
	int16 degrees;

	switch(direction)
	{
		case LEFT:
			//extract current degrees from PWM1 Match Register 1 then subtract 45 to get new position
			degrees = 180*(LPC_PWM1->MR1 - PWM_MATCH_MIN)/(PWM_MATCH_MAX - PWM_MATCH_MIN)-45;
			if (degrees < 0) 	// if camera is already rotated all the way left exit
				return 0;
			else
				setServoPosDeg(degrees); //set new position
			break;
		case RIGHT:
			//extract current degrees from PWM1 Match Register 1 then subtract 45 to get new position
			degrees = 180*(LPC_PWM1->MR1 - PWM_MATCH_MIN)/(PWM_MATCH_MAX - PWM_MATCH_MIN)+45;
			if (degrees > 180) // if camera is already rotated all the way right exit
				return 0;
			else
				setServoPosDeg(degrees); //set new position
			break;
	}
	return 1;
}
