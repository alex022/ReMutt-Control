/*
 * audio.h
 *
 *  Created on: May 12, 2014
 *      Author: joonhee
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <stdio.h>
#include "gpio.h"
#include "ssp.h"
#include "timer.h"


void audio_initialize();
void audio_reset();
void audio_test();
void audio_setVolume(uint8 vol);
void audio_setupMP3();
void audio_playVoice(int command);
void audio_storeVoice();







#endif /* AUDIO_H_ */
