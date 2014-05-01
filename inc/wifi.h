/*
 * wifi.h
 *
 *  Created on: Apr 30, 2014
 *      Author: stevenguan
 */

#ifndef WIFI_H_
#define WIFI_H_

#include "uart.h"

/* Signal Macros */
#define AUTO_CONNECT 1
#define CMD_MODE 2
#define MAX_SIZE 50

/* Function prototypes */
void initWiFi(int setting);

#endif /* WIFI_H_ */
