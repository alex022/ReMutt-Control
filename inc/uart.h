#ifndef UART_H_
#define UART_H_
/*******************************************************************************
 * File name: 		uart.h
 * Description: 	LPC1788's UART0 can work as a modem. UART4 has IrDA. There are
 * 					also standard UART0/2/3 available.
 * Project: 		_framework
 * Target: 			LPC1788
 * Compiler: 		arm-none-eabi-gcc
 * Date: 			08-11-2012
 * Author: 			Kuba
 * Based on: 		---
 ******************************************************************************/

/*==============================================================================
 Includes
==============================================================================*/
# include "irq.h"
# include "typedef.h"
# include "gpdma.h"
/*==============================================================================
 Defines
==============================================================================*/

/*==============================================================================
 Types
==============================================================================*/
enum uartBaud {
   uartBAUD50      = 50,
   uartBAUD75      = 75,
   uartBAUD110     = 110,
   uartBAUD134     = 134,
   uartBAUD150     = 150,
   uartBAUD200     = 200,
   uartBAUD300     = 300,
   uartBAUD600     = 600,
   uartBAUD1200    = 1200,
   uartBAUD1800    = 1800,
   uartBAUD2400    = 2400,
   uartBAUD4800    = 4800,
   uartBAUD9600    = 9600,
   uartBAUD19200   = 19200,
   uartBAUD38400   = 38400,
   uartBAUD57600   = 57600,
   uartBAUD64000   = 64000,
   uartBAUD115200  = 115200,
   uartBAUD125000  = 125000,
   uartBAUD230400  = 230400,
   uartBAUD460800  = 460800,
   uartBAUD576000  = 576000
};

enum uartStopBit {
   uartSTOP_BIT_1,
   uartSTOP_BIT_2
};

enum uartParity {
   uartPARITY_DISABLE,
   uartPARITY_ODD,
   uartPARITY_EVEN,
   uartPARITY_MARK,
   uartPARITY_SPACE
};


enum uartRxTriggerLvl {
	uartTRIGER_LEVEL_1CHAR,
	uartTRIGER_LEVEL_4CHAR,
	uartTRIGER_LEVEL_8CHAR,
	uartTRIGER_LEVEL_14CHAR
};

struct uartIrqConf {
	enum irqPriority		priority;
	uint8*					txBuff;
	uint32					txBuffSize;
	uint8* 					rxBuff;
	enum uartRxTriggerLvl 	rxTriggerLvl;
	void (*txEndService)	(void);
	void (*rxEndService) 	(void);
	void (*timeout)			(void);
};

struct uartDmaConf {
	enum gpdmaChnl txChnl;
	enum gpdmaChnl rxChnl;
	void (*txEndService)(void);
	void (*rxEndService)(void);
	void (*error)(void);
};
/*==============================================================================
 Globals
==============================================================================*/

/*==============================================================================
 Global function prototypes
==============================================================================*/
/*------------------------------------------------------------------------------
 function name:		uartNInit
 description: 		init UART. Call this function to set basic parameters of the
 	 	 	 	 	 UART controller.
 parameters:		baud rate, stop bit, parity
 returned value:	none
------------------------------------------------------------------------------*/
void uart0Init    (const enum uartBaud baud, const enum uartStopBit stopBit, const enum uartParity parity);
void uart1Init    (const enum uartBaud baud, const enum uartStopBit stopBit, const enum uartParity parity);
void uart2Init    (const enum uartBaud baud, const enum uartStopBit stopBit, const enum uartParity parity);
int uart1Getchar(void);
/*------------------------------------------------------------------------------
 function name:		uartNTx/uartXRx
 description: 		UART RX/TX polling functions.
 parameters:		buffers, size
 returned value:	none
------------------------------------------------------------------------------*/
void uart0Tx      (const uint8 *tx, const uint32 size);
void uart0Rx      (uint8 *rx, const uint32 size);
void uart1Tx      (const uint8 *tx, const uint32 size);
void uart1Rx      (uint8 *rx, const uint32 size);
void uart2Tx      (const uint8 *tx, const uint32 size);
void uart2Rx      (uint8 *rx, const uint32 size);

/*------------------------------------------------------------------------------
 function name:		uartNInitIrq
 description: 		init UART interrupts
 parameters:		IRQ configuration struct
 returned value:	none
------------------------------------------------------------------------------*/
void uart0InitIrq		(const struct uartIrqConf *irq);
void uart1InitIrq		(const struct uartIrqConf *irq);
//void uart2InitIrq		(const struct uartIrqConf *irq);

/*------------------------------------------------------------------------------
 function name:		uartNEnableIrqTx
 description:		enable UART TX interrupts. Functions sends a dummy 'x' character
 	 	 	 	 	 to start TX IRQ.
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void uart0EnableIrqTx	(void);
void uart1EnableIrqTx	(void);
//void uart2EnableIrqTx	(void);

/*------------------------------------------------------------------------------
 function name:		uartNDisableIrqTx
 description:		disable UART TX interrupts
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void uart0DisableIrqTx	(void);
void uart1DisableIrqTx	(void);
//void uart2DisableIrqTx	(void);

/*------------------------------------------------------------------------------
 function name:		uartNEnableIrqRx
 description:		enable UART RX interrupts
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void uart0EnableIrqRx	(void);
void uart1EnableIrqRx	(void);
//void uart2EnableIrqRx	(void);

/*------------------------------------------------------------------------------
 function name:		uart2InitDma
 description: 		init UART DMA support
 parameters:		user services at the end of TX/RX, error service
 returned value:	none
------------------------------------------------------------------------------*/
void uart0InitDma 		(const struct uartDmaConf *dma);
void uart1InitDma 		(const struct uartDmaConf *dma);
//void uart2InitDma 		(const struct uartDmaConf *dma);
void uart0DmaTx 		(const uint8 *tx, const uint32 size);
void uart0DmaRx			(uint8 *rx, const uint32 size);
void uart1DmaTx 		(const uint8 *tx, const uint32 size);
void uart1DmaRx			(uint8 *rx, const uint32 size);
//void uart2DmaTx 		(const uint8 *tx, const uint32 size);
//void uart2DmaRx			(uint8 *rx, const uint32 size);

/*------------------------------------------------------------------------------
 function name:		uart0PutChar
 description:		function used by uprintf(), sends a char to the uart
 parameters:		character to send
 returned value:	none
------------------------------------------------------------------------------*/
void uart0PutChar 		(char character);
void uart1PutChar 		(char character);
void uart2PutChar 		(char character);
/******************************************************************************
* END OF FILE
******************************************************************************/

#endif /* UART_H_ */
