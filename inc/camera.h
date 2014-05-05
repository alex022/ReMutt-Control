/*
 * camera.h
 *
 *  Created on: Apr 29, 2014
 *      Author: eric_brunnett
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "uart.h"
#include <stdlib.h>

#define SERIAL_NUM				0x00

#define CMD_SEND				0x56
#define CMD_REPLY				0x76

#define CMD_END					0x00
#define CMD_GETVERSION			0x11
#define CMD_SET					0x24
#define CMD_RESET				0x26
#define CMD_READBUFF			0x32
#define CMD_GETBUFFLEN			0x34
#define CMD_TAKEPHOTO			0x36


#define FBUF_CURRENTFRAME		0x00
#define FBUF_NEXTFRAME			0x01
#define FBUF_STOPCURRENTFRAME	0x00

uint8 checkReply(uint8* r, uint8 c);
uint8 setUartSpeed(const enum uartBaud baud);
uint8 cameraReset();
uint8 getVersion();
uint8 takePhoto();
uint32 getBufferLength();
uint8* getPhoto(uint32 bytes);
//uint8 getPhoto(uint32 bytes, uint8** photo);

#endif /* CAMERA_H_ */
