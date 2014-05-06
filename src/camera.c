/*
* camera.c
*
*  Created on: Apr 29, 2014
*      Author: eric_brunnett
*/
#include "camera.h"

uint8 checkReply(uint8* r, uint8 c)
{
	if (r[0] == CMD_REPLY && r[1] == SERIAL_NUM && r[2] == c && r[3] == 0x00)
		return 1;
	return 0;
}

uint8 cameraInit()
{
	uint8 return_command[5];

	uart2Init(uartBAUD38400, uartSTOP_BIT_1, uartPARITY_DISABLE);
	if (!cameraReset())
		return 0; //Should work but might not
	setUartSpeed(uartBAUD115200);

	return 1;
}

uint8 setUartSpeed(const enum uartBaud baud)
{
	uint8 command[10];
	uint8 return_command[10];

	command[0] = CMD_SEND;
	command[1] = SERIAL_NUM;
	command[2] = CMD_SET;
	command[3] = 0x03;
	command[4] = 0x01;

	switch(baud)
	{
		case uartBAUD9600:
			command[5] = 0xAE;
			command[6] = 0xC8;
			break;
		case uartBAUD19200:
			command[5] = 0x56;
			command[6] = 0xE4;
			break;
		case uartBAUD38400:
			command[5] = 0x2A;
			command[6] = 0xF2;
			break;
		case uartBAUD57600:
			command[5] = 0x1C;
			command[6] = 0x1C;
			break;
		case uartBAUD115200:
			command[5] = 0x0D;
			command[6] = 0xA6;
			break;
		default:
			return 0;
	}

	uart2Tx((uint8*) command,7);
	uart2Rx((uint8*)&return_command,5);

	if (checkReply(return_command, CMD_SET))
	{
		uart2Init(baud, uartSTOP_BIT_1, uartPARITY_DISABLE);
		return 1;
	}
	return 0;
}


uint8 cameraReset()
{
	uint8 command[5];
	uint8 return_command[10];

	//stop frame update
	command[0] = CMD_SEND;
	command[1] = SERIAL_NUM;
	command[2] = CMD_RESET;
	command[3] = CMD_END;
	uart2Tx((uint8*) command,4);

	uart2Rx((uint8*)&return_command,5);

	//TODO: might need to fix number received and implement delay

	if (checkReply(return_command, CMD_RESET))
		return 1;
	return 0;
}

uint8 getVersion()
{
	uint8 command[5];
	uint8 return_command[20];

	//stop frame update
	command[0] = CMD_SEND;
	command[1] = SERIAL_NUM;
	command[2] = CMD_GETVERSION;
	command[3] = CMD_END;
	uart2Tx((uint8*) command,4);

	uart2Rx((uint8*)&return_command,16);
	if (checkReply(return_command, CMD_GETVERSION))
		return 1;
	return 0;
}

uint8 stopFrame()
{
	uint8 command[5];
	uint8 return_command[10];

	//stop frame update
	command[0] = CMD_SEND;
	command[1] = SERIAL_NUM;
	command[2] = CMD_TAKEPHOTO;
	command[3] = 0x01;
	command[4] = FBUF_STOPCURRENTFRAME;
	uart2Tx((uint8*) command,5);

	uart2Rx((uint8*)&return_command,5);

	if (checkReply(return_command, CMD_TAKEPHOTO) && return_command[4] == 0x00)
		return 1;
	return 0;
}

uint8 resumeFrame()
{
	uint8 command[5];
	uint8 return_command[10];

	//stop frame update
	command[0] = CMD_SEND;
	command[1] = SERIAL_NUM;
	command[2] = CMD_TAKEPHOTO;
	command[3] = 0x01;
	command[4] = FBUF_RESUMEFRAME;
	uart2Tx((uint8*) command,5);

	uart2Rx((uint8*)&return_command,5);

	if (checkReply(return_command, CMD_TAKEPHOTO) && return_command[4] == 0x00)
		return 1;
	return 0;
}

uint32 getBufferLength()
{
	uint32 length = 0;
	uint8 command[5];
	uint8 return_command[10];

	command[0] = CMD_SEND;
	command[1] = SERIAL_NUM;
	command[2] = CMD_GETBUFFLEN;
	command[3] = 0x01;
	command[4] = FBUF_CURRENTFRAME;
	uart2Tx((uint8*) command,5);

	uart2Rx((uint8*)&return_command,9);

	length = return_command[5];
	length = length << 8;
	length += return_command[6];
	length = length << 8;
	length += return_command[7];
	length = length << 8;
	length += return_command[8];

	if(checkReply(return_command, CMD_GETBUFFLEN) && return_command[4] == 0x04)
		return length;
	return 0;
}

uint8* getPhoto(uint32 bytes)
{
	uint32 addr = 0;
	uint8 command[20];
	uint8 return_command[40];
	uint8* photo = malloc(bytes*sizeof(uint8));
	uint32 photo_pos = 0;
	uint8 photo_data[32];

	command[0] = CMD_SEND;
	command[1] = SERIAL_NUM;
	command[2] = CMD_READBUFF;
	command[3] = 0x0C;
	command[4] = FBUF_CURRENTFRAME;
	command[5] = 0x0A;


	while (addr < bytes + 32)
	{
		command[6] = (addr >> 24) & 0xFF;
		command[7] = (addr >> 16) & 0xFF;
		command[8] = (addr >> 8) & 0xFF;
		command[9] = addr & 0xFF;
		command[10] = 0x00;
		command[11] = 0x00;
		command[12] = 0x00;
		command[13] = 0x20;
		command[14] = 0x00;
		command[15] = 0x00;


		uart2Tx((uint8*) command,16);

		uart2Rx((uint8*)&return_command,5);

		if (!checkReply(return_command, CMD_READBUFF))
			return 0;

		uart2Rx((uint8*)&photo_data, 32);
		uart2Rx((uint8*)&return_command,5);

		if (!checkReply(return_command, CMD_READBUFF))
			return 0;

		uint8 pos = 0;
		while(pos < 32)
		{
			photo[photo_pos++] = photo_data[pos++];
		}
		addr += 32;
	}

	//TODO: save photo to memory

	return photo;
}



void takePhoto(void) {

	stopFrame();
	uint32 len = getBufferLength();
	getPhoto(len);
	resumeFrame();

}

