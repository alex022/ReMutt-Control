/*******************************************************************************
 * File name: 		uart.c
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
#include <string.h>
#include "LPC177x_8x.h"
#include "hdr_sc.h"
#include "macros.h"
#include "init.h"
#include "gpdma.h"
#include "uart.h"
#include "timer.h"

/*==============================================================================
 Defines
==============================================================================*/
# define UART_PORTS							5
# define UART0_BASE_DIV						16
# define UART1_BASE_DIV          			16
# define UART2_BASE_DIV          			16
# define UART3_BASE_DIV          			16

/* UART pins */
# define UART0_PIN_TXD						SET_BIT(0)
# define UART0_PIN_RXD						SET_BIT(0)
# define UART1_PIN_TXD						SET_BIT(0)
# define UART1_PIN_RXD						SET_BIT(0)
# define UART2_PIN_TXD						SET_BIT(0)
# define UART2_PIN_RXD						SET_BIT(0)

/* line control register */
# define LCR_WORD_LENGTH_SELECT_bit     	0
# define LCR_WORD_LENGTH_SELECT_5BIT    	SET_BITS(0, LCR_WORD_LENGTH_SELECT_bit)
# define LCR_WORD_LENGTH_SELECT_6BIT    	SET_BITS(1, LCR_WORD_LENGTH_SELECT_bit)
# define LCR_WORD_LENGTH_SELECT_7BIT 		SET_BITS(2, LCR_WORD_LENGTH_SELECT_bit)
# define LCR_WORD_LENGTH_SELECT_8BIT		SET_BITS(3, LCR_WORD_LENGTH_SELECT_bit)
# define LCR_STOP_BIT_SELECT_bit			2
# define LCR_STOP_BIT_SELECT_1BIT       	SET_BITS(0, LCR_STOP_BIT_SELECT_bit)
# define LCR_STOP_BIT_SELECT_2BIT       	SET_BITS(1, LCR_STOP_BIT_SELECT_bit)
# define LCR_BREAK_CONTROL              	SET_BIT(6)
# define LCR_DLAB                       	SET_BIT(7)

/* fifo control register */
# define FCR_FIFO_ENABLE              		SET_BIT(0)
# define FCR_FIFO_RST_RX                	SET_BIT(1)
# define FCR_FIFO_RST_TX                	SET_BIT(2)
# define FCR_DMA_ENABLE						SET_BIT(3)
# define FCR_RX_FIFO_TRIGGER_LEVEL_bit		6
# define FCR_RX_FIFO_TRIGGER_LVL_1CHAR		SET_BITS(0, FCR_RX_FIFO_TRIGGER_LEVEL_bit)
# define FCR_RX_FIFO_TRIGGER_LVL_4CHAR		SET_BITS(1, FCR_RX_FIFO_TRIGGER_LEVEL_bit)
# define FCR_RX_FIFO_TRIGGER_LVL_8CHAR  	SET_BITS(2, FCR_RX_FIFO_TRIGGER_LEVEL_bit)
# define FCR_RX_FIFO_TRIGGER_LVL_14CHAR 	SET_BITS(3, FCR_RX_FIFO_TRIGGER_LEVEL_bit)

/* interrupts enable register */
# define IER_INTERRUPT_EN_RBR				SET_BIT(0)
# define IER_INTERRUPT_EN_THRE          	SET_BIT(1)
/* interrupt identification register */
# define IIR_RLS							SET_BITS(3, 1)
# define IIR_RDA							SET_BIT(2)
# define IIR_CTI							SET_BITS(6, 1)
# define IIR_THRE							SET_BITS(1, 1)
# define IIR_IRQ_ID_MASK					SET_BITS(7, 1)

/* line status register */
# define LSR_RECEIVER_DATA_READY   			SET_BIT(0)
# define LSR_OVERRUN_ERROR           		SET_BIT(1)
# define LSR_PARITY_ERROR             		SET_BIT(2)
# define LSR_FRAMING_ERROR            		SET_BIT(3)
# define LSR_BREAK_INTERRUPT          		SET_BIT(4)
# define LSR_THR_EMPTY                		SET_BIT(5)
# define LSR_TRANSMITTER_EMPTY        		SET_BIT(6)
# define LSR_RX_FIFO_ERROR            		SET_BIT(7)

/*==============================================================================
 Types
==============================================================================*/
/* data shared between program and ISR */
struct uartPrivate {
	uint8*					txBuff;
	uint32					txBuffSize;
	uint8*					rxBuff;
	enum uartRxTriggerLvl	rxTriggerLvl;
	void (*txIrqService) 	(void);              	/* user interrupt services called at the end of transfers */
	void (*rxIrqService) 	(void);
	void (*timeout)			(void);
};

/* all GPDMA related data */
struct uartDma {
	struct gpdmaConf 	txConf;												/* GPDMA TX struct */
	struct gpdmaConf 	rxConf;												/* GPDMA RX struct */
	gpdmaChannel		txChnlSettings;										/* GPDMA TX struct with control and config word */
	gpdmaChannel		rxChnlSettings;										/* GPDMA RX struct with control and config word */
	gpdmaChannel 		(*txChnlInit)(const struct gpdmaConf *gpdmaConf); 	/* pointer to GPDMA TX init function */
	gpdmaChannel 		(*rxChnlInit)(const struct gpdmaConf *gpdmaConf);	/* pointer to GPDMA RX init function */
	void 				(*txChnlEnable) (const gpdmaChannel chnl, const uint16 size); /* pointer to GPDMA TX enable channel function */
	void 				(*rxChnlEnable) (const gpdmaChannel chnl, const uint16 size); /* pointer to GPDMA RX enable channel function */
	uint8*				rxUsrBuff;												/* all user RX settings are saved here */
	uint32				rxUsrSize;
	void				(*rxUsrService)(void);
};
/*==============================================================================
 Globals
==============================================================================*/
static struct uartPrivate uartPrivate[UART_PORTS];
static struct uartDma uartDma[UART_PORTS];

static uint8 u0buff[16];
static uint8 u1buff[16];
static uint8 u2buff[16];

/*==============================================================================
 Static function prototypes
==============================================================================*/
static void u0DmaEndRx (void);
static void u1DmaEndRx (void);
static void dummyService (void);

/*==============================================================================
 Global function definitions
==============================================================================*/
/*------------------------------------------------------------------------------
 function name:		uart0Init
 description: 		init UART0
 parameters:   		baud rate, stop bit, parity
 returned value:	none
------------------------------------------------------------------------------*/
void uart0Init (const enum uartBaud baud, const enum uartStopBit stopBit, const enum uartParity parity)
{
	uint32 div;

	/* turn on UART0 */
	LPC_SC->PCONP |= LPC_SC_PCONP_PCUART0;
    /* set DLAB to enable access to divisor latches- DLL and DLM are accessible,
     * 8 bit character, 1 bit stop, none parity */
    LPC_UART0->LCR = LCR_WORD_LENGTH_SELECT_8BIT | stopBit | parity | LCR_DLAB;
    /* set divisor latches DLL, DLM */
    div = (SYSTEM_FREQUENCY / UART0_BASE_DIV) / baud / 2;
    LPC_UART0->DLM = div / 256;
    LPC_UART0->DLL = div % 256;
    /* clear DLAB to disable DLL, DLM- now RBR, THR and IER are accessible */
    LPC_UART0->LCR &= ~LCR_DLAB;
    /* set fifo */
    LPC_UART0->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_1CHAR;
    /* set pins */
    LPC_IOCON->P0_2 = 0;									/* clear pins */
    LPC_IOCON->P0_3 = 0;
    LPC_IOCON->P0_2 = UART0_PIN_TXD;						/* UART0 pins on */
    LPC_IOCON->P0_3 = UART0_PIN_RXD;
}

/*==============================================================================
 Global function definitions
==============================================================================*/
/*------------------------------------------------------------------------------
 function name:		uarT1Init
 description: 		init UART1
 parameters:   		baud rate, stop bit, parity
 returned value:	none
------------------------------------------------------------------------------*/
void uart1Init (const enum uartBaud baud, const enum uartStopBit stopBit, const enum uartParity parity)
{
	uint32 div;

	/* turn on UART1 */
	LPC_SC->PCONP |= LPC_SC_PCONP_PCUART1;
    /* set DLAB to enable access to divisor latches- DLL and DLM are accessible,
     * 8 bit character, 1 bit stop, none parity */
    LPC_UART1->LCR = LCR_WORD_LENGTH_SELECT_8BIT | stopBit | parity | LCR_DLAB;
    /* set divisor latches DLL, DLM */
    div = (SYSTEM_FREQUENCY / UART1_BASE_DIV) / baud / 2;
    LPC_UART1->DLM = div / 256;
    LPC_UART1->DLL = div % 256;
    /* clear DLAB to disable DLL, DLM- now RBR, THR and IER are accessible */
    LPC_UART1->LCR &= ~LCR_DLAB;
    /* set fifo */
    LPC_UART1->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_1CHAR;
    /* set pins*/
    LPC_IOCON->P3_16 = 0;				/* reset registers */
    LPC_IOCON->P3_17 = 0;
    LPC_IOCON->P3_16 = SET_BIT(0);		/* set registers to FUNC = 011 */
    LPC_IOCON->P3_16 |= SET_BIT(1);
    LPC_IOCON->P3_17 = SET_BIT(0);
    LPC_IOCON->P3_17 |= SET_BIT(1);
}

/*------------------------------------------------------------------------------
 function name:		uart2Init
 description: 		init UART2
 parameters:   		baud rate, stop bit, parity
 returned value:	none
------------------------------------------------------------------------------*/
void uart2Init (const enum uartBaud baud, const enum uartStopBit stopBit, const enum uartParity parity)
{
	uint32 div;

	/* turn on UART0 */
	LPC_SC->PCONP |= LPC_SC_PCONP_PCUART2;
	/* set DLAB to enable access to divisor latches- DLL and DLM are accessible,
	 * 8 bit character, 1 bit stop, none parity */
	LPC_UART2->LCR = LCR_WORD_LENGTH_SELECT_8BIT | stopBit | parity | LCR_DLAB;
    /* set divisor latches DLL, DLM */
    div = (SYSTEM_FREQUENCY / UART2_BASE_DIV) / baud / 2;
    LPC_UART2->DLM = div / 256;
    LPC_UART2->DLL = div % 256;
    /* clear DLAB to disable DLL, DLM- now RBR, THR and IER are accessible */
    LPC_UART2->LCR &= ~LCR_DLAB;
    /* set fifo */
    LPC_UART2->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_1CHAR;
    /* set UART pins */
    LPC_IOCON->P0_10 = 0;									/* clear pins */
    LPC_IOCON->P0_11 = 0;
    LPC_IOCON->P0_10 = UART2_PIN_TXD;						/* turn pins on */
    LPC_IOCON->P0_11 = UART2_PIN_RXD;
}

/*------------------------------------------------------------------------------
 function name:   uart0Tx
 description:     send data via polling
 parameters:      TX buffer, size
 returned value:  none
------------------------------------------------------------------------------*/
void uart0Tx (const uint8 *tx, const uint32 size)
{
	uint32 i;

	i = size;
	for (i = 0; i < size; ++i)
	{
		while (!(LPC_UART0->LSR & LSR_THR_EMPTY));
		LPC_UART0->THR = *tx++;
	}
}

/*------------------------------------------------------------------------------
 function name:   uart0Rx
 description:     get data via polling
 parameters:      RX buffer, size
 returned value:  none
------------------------------------------------------------------------------*/
void uart0Rx (uint8 *rx, const uint32 size)
{
	uint32 i;
	uint8 retval;
	i = size;
	for (i = 0; i < size; ++i)
	{
		while (!(LPC_UART0->LSR & LSR_RECEIVER_DATA_READY)){
			//retval = uart1Getchar();			/* used to receive input from the WiFi module, can comment out */
	    	//if(retval != -1)
	    		//printf("%c", retval);
		}
		*rx++ = LPC_UART0->RBR;
	}
}

/*------------------------------------------------------------------------------
 function name:   uart1Tx
 description:     send data via polling
 parameters:      TX buffer, size
 returned value:  none
------------------------------------------------------------------------------*/
void uart1Tx (const uint8 *tx, const uint32 size)
{
	uint32 i;

	i = size;
	for (i = 0; i < size; ++i)
	{
		while (!(LPC_UART1->LSR & LSR_THR_EMPTY));
			LPC_UART1->THR = *tx++;
	}
}

/*------------------------------------------------------------------------------
 function name:   uart1Rx
 description:     get data via polling
 parameters:      RX buffer, size
 returned value:  none
------------------------------------------------------------------------------*/
void uart1Rx (uint8 *rx, const uint32 size)
{
	uint32 i;

	i = size;
	for (i = 0; i < size; ++i)
	{
		while (!(LPC_UART1->LSR & LSR_RECEIVER_DATA_READY));
		*rx++ = LPC_UART1->RBR;
	}
}

void uart1PutChar (char character)
{
	while (!(LPC_UART1->LSR & LSR_THR_EMPTY));
	LPC_UART1->THR = character;
}

int16 uart1Getchar(void)
{
	if (LPC_UART1->LSR & LSR_RECEIVER_DATA_READY)                 // check if character is available
	{
		//uart0Putch('#');

		return LPC_UART1->RBR;                     // return character
	}

	return -1;
}

int16 uart2Getchar(void)
{
	if (LPC_UART2->LSR & LSR_RECEIVER_DATA_READY)                 // check if character is available
	{
		return LPC_UART2->RBR;                     // return character
	}

	return -1;
}
/*------------------------------------------------------------------------------
 function name:		uart2Tx
 description: 		send characters by uart 2
 parameters:   		pointer to TX buffer, size
 returned value:	none
------------------------------------------------------------------------------*/
void uart2Tx (const uint8 *tx, const uint32 size)
{
	uint32 i;

	i = size;
	for (i = 0; i < size; ++i)
	{
		while (!(LPC_UART2->LSR & LSR_THR_EMPTY));
		LPC_UART2->THR = *tx++;
	}
}

/*------------------------------------------------------------------------------
 function name:		uart2Rx
 description: 		get characters from uart 0
 parameters:   		pointer to RX buffer, size
 returned value:	-1 if timer interrupted
 	 	 	 	 	 0 if okay
------------------------------------------------------------------------------*/
int8 uart2Rx (uint8 *rx, const uint32 size)
{
	uint32 i;

	i = size;
	for (i = 0; i < size; ++i)
	{
		startTimerInt(0,20000);
		while (!(LPC_UART2->LSR & LSR_RECEIVER_DATA_READY) && (!UartRx_interrupt));
		if (UartRx_interrupt)
		{
			UartRx_interrupt = 0;
			return 1;
		}
		*rx++ = LPC_UART2->RBR;
    	LPC_TIM1->MCR &= 0xFFFFFFFE;

	}
	return 0;
}

/*------------------------------------------------------------------------------
 function name:   uart0InitIrq
 description:     init UART2 interrupts
 parameters:      pointer to uart interrupt configuration struct
 returned value:  none
------------------------------------------------------------------------------*/
void uart0InitIrq		(const struct uartIrqConf *irq)
{
    /* reset and configure fifo */
    switch (irq->rxTriggerLvl)
    {
    case uartTRIGER_LEVEL_1CHAR:
    	LPC_UART0->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_1CHAR;
    	break;

    case uartTRIGER_LEVEL_4CHAR:
    	LPC_UART0->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_4CHAR;
    	break;

    case uartTRIGER_LEVEL_8CHAR:
    	LPC_UART0->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_8CHAR;
    	break;

    case uartTRIGER_LEVEL_14CHAR:
    	LPC_UART0->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_14CHAR;
    	break;

    default:
    	LPC_UART0->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_1CHAR;
    	break;
    }

    /* set NVIC */
	NVIC_SetPriority(UART0_IRQn, irq->priority);	/* set priority in NVIC */
	NVIC_EnableIRQ(UART0_IRQn);						/* enable interrupt in NVIC */

    /* TX interrupt */
	uartPrivate[0].txBuff = irq->txBuff;			/* set TX IRQ buffer */
	uartPrivate[0].txBuffSize = irq->txBuffSize;	/* data length */
	if (irq->txEndService == 0)    					/* check if user service is 0 */
	{
	  uartPrivate[0].txIrqService = dummyService;
	}
	else
	{
	  uartPrivate[0].txIrqService = irq->txEndService;
	}

	/* RX interrupt */
	uartPrivate[0].rxTriggerLvl = irq->rxTriggerLvl;
	if (irq->rxBuff == 0)							/* check if buffer is 0, otherwise vector can be overwritten */
	{
		uartPrivate[0].rxBuff = u0buff;
	}
	else
	{
		uartPrivate[0].rxBuff = irq->rxBuff;
	}

	if (irq->rxEndService == 0)    					/* check if user service is 0 */
	{
	  uartPrivate[0].rxIrqService = dummyService;
	}
	else
	{
	   uartPrivate[0].rxIrqService = irq->rxEndService;
	}

	if (irq->timeout == 0)
	{
		uartPrivate[0].timeout = dummyService;
	}
	else
	{
		uartPrivate[0].timeout = irq->timeout;
	}
}


/*------------------------------------------------------------------------------
 function name:   uart1InitIrq
 description:     init UART1 interrupts
 parameters:      pointer to uart interrupt configuration struct
 returned value:  none
------------------------------------------------------------------------------*/
void uart1InitIrq		(const struct uartIrqConf *irq)
{
    /* reset and configure fifo */
    switch (irq->rxTriggerLvl)
    {
    case uartTRIGER_LEVEL_1CHAR:
    	LPC_UART1->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_1CHAR;
    	break;

    case uartTRIGER_LEVEL_4CHAR:
    	LPC_UART1->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_4CHAR;
    	break;

    case uartTRIGER_LEVEL_8CHAR:
    	LPC_UART1->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_8CHAR;
    	break;

    case uartTRIGER_LEVEL_14CHAR:
    	LPC_UART1->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_14CHAR;
    	break;

    default:
    	LPC_UART1->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_1CHAR;
    	break;
    }

    /* set NVIC */
	NVIC_SetPriority(UART1_IRQn, irq->priority);	/* set priority in NVIC */
	NVIC_EnableIRQ(UART1_IRQn);						/* enable interrupt in NVIC */

    /* TX interrupt */
	uartPrivate[1].txBuff = irq->txBuff;			/* set TX IRQ buffer */
	uartPrivate[1].txBuffSize = irq->txBuffSize;	/* data length */
	if (irq->txEndService == 0)    					/* check if user service is 0 */
	{
	  uartPrivate[1].txIrqService = dummyService;
	}
	else
	{
	  uartPrivate[1].txIrqService = irq->txEndService;
	}

	/* RX interrupt */
	uartPrivate[1].rxTriggerLvl = irq->rxTriggerLvl;
	if (irq->rxBuff == 0)							/* check if buffer is 0, otherwise vector can be overwritten */
	{
		uartPrivate[1].rxBuff = u1buff;
	}
	else
	{
		uartPrivate[1].rxBuff = irq->rxBuff;
	}

	if (irq->rxEndService == 0)    					/* check if user service is 0 */
	{
	  uartPrivate[1].rxIrqService = dummyService;
	}
	else
	{
	   uartPrivate[1].rxIrqService = irq->rxEndService;
	}

	if (irq->timeout == 0)
	{
		uartPrivate[1].timeout = dummyService;
	}
	else
	{
		uartPrivate[1].timeout = irq->timeout;
	}
}
/*------------------------------------------------------------------------------
 function name:   uart2InitIrq
 description:     init UART2 interrupts
 parameters:      pointer to uart interrupt configuration struct
 returned value:  none
------------------------------------------------------------------------------*/
void uart2InitIrq	(const struct uartIrqConf *irq)
{
    /* reset and configure fifo */
    switch (irq->rxTriggerLvl)
    {
    case uartTRIGER_LEVEL_1CHAR:
    	LPC_UART2->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_1CHAR;
    	break;

    case uartTRIGER_LEVEL_4CHAR:
    	LPC_UART2->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_4CHAR;
    	break;

    case uartTRIGER_LEVEL_8CHAR:
    	LPC_UART2->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_8CHAR;
    	break;

    case uartTRIGER_LEVEL_14CHAR:
    	LPC_UART2->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_14CHAR;
    	break;

    default:
    	LPC_UART2->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_RX_FIFO_TRIGGER_LVL_1CHAR;
    	break;
    }

    /* set NVIC */
	NVIC_SetPriority(UART2_IRQn, irq->priority);	/* set priority in NVIC */
	NVIC_EnableIRQ(UART2_IRQn);						/* enable interrupt in NVIC */

    /* TX interrupt */
	uartPrivate[2].txBuff = irq->txBuff;			/* set TX IRQ buffer */
	if (irq->txEndService == 0)    					/* check if user service is 0 */
	{
	  uartPrivate[2].txIrqService = dummyService;
	}
	else
	{
	  uartPrivate[2].txIrqService = irq->txEndService;
	}

	/* RX interrupt */
	uartPrivate[2].rxTriggerLvl = irq->rxTriggerLvl;
	if (irq->rxBuff == 0)							/* check if buffer is 0, otherwise vector can be overwritten */
	{
		uartPrivate[2].rxBuff = u0buff;
	}
	else
	{
		uartPrivate[2].rxBuff = irq->rxBuff;
	}

	if (irq->rxEndService == 0)    					/* check if user service is 0 */
	{
	  uartPrivate[2].rxIrqService = dummyService;
	}
	else
	{
	   uartPrivate[2].rxIrqService = irq->rxEndService;
	}

	if (irq->timeout == 0)
	{
		uartPrivate[2].timeout = dummyService;
	}
	else
	{
		uartPrivate[2].timeout = irq->timeout;
	}
}

/*------------------------------------------------------------------------------
 function name:		uart0EnableIrqTx
 description:		enable UART TX interrupts
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void uart0EnableIrqTx (void)
{
	LPC_UART0->IER |= IER_INTERRUPT_EN_THRE;		/* enable TX interrupt */
	LPC_UART0->THR = 'x';							/* this will send first char & trigger TX interrupt ISR */
}

/*------------------------------------------------------------------------------
 function name:		uart1EnableIrqTx
 description:		enable UART TX interrupts
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void uart1EnableIrqTx (void)
{
	LPC_UART1->IER |= IER_INTERRUPT_EN_THRE;		/* enable TX interrupt */
	LPC_UART1->THR = 'x';							/* this will send first char & trigger TX interrupt ISR */
}

/*------------------------------------------------------------------------------
 function name:		uart2EnableIrqTx
 description:		enable UART TX interrupts
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void uart2EnableIrqTx	(void)
{
	LPC_UART2->IER |= IER_INTERRUPT_EN_THRE;		/* enable TX interrupt */
	LPC_UART2->THR = 'x';							/* this will send first char & trigger TX interrupt ISR */
}

/*------------------------------------------------------------------------------
 function name:		uart0DisableIrqTx
 description:		disable UART TX interrupts
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void uart0DisableIrqTx (void)
{
	LPC_UART0->IER &= ~IER_INTERRUPT_EN_THRE;
}

/*------------------------------------------------------------------------------
 function name:		uart1DisableIrqTx
 description:		disable UART TX interrupts
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void uart1DisableIrqTx (void)
{
	LPC_UART1->IER &= ~IER_INTERRUPT_EN_THRE;
}

/*------------------------------------------------------------------------------
 function name:		uart2DisableIrqTx
 description:		disable UART TX interrupts
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void uart2DisableIrqTx (void)
{
	LPC_UART2->IER &= ~IER_INTERRUPT_EN_THRE;
}

/*------------------------------------------------------------------------------
 function name:		uart0EnableIrqRx
 description:		enable UART RX interrupts
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void uart0EnableIrqRx (void)
{
	LPC_UART0->IER |= IER_INTERRUPT_EN_RBR;			/* enable TX interrupt */
}

/*------------------------------------------------------------------------------
 function name:		uart1EnableIrqRx
 description:		enable UART RX interrupts
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void uart1EnableIrqRx (void)
{
	LPC_UART1->IER |= IER_INTERRUPT_EN_RBR;			/* enable TX interrupt */
}

/*------------------------------------------------------------------------------
 function name:		uart2EnableIrqRx
 description:		enable UART RX interrupts
 parameters:		none
 returned value:	none
------------------------------------------------------------------------------*/
void uart2EnableIrqRx	(void)
{
	LPC_UART2->IER |= IER_INTERRUPT_EN_RBR;			/* enable TX interrupt */
}

/*------------------------------------------------------------------------------
 function name:		uart0InitDma
 description: 		init UART DMA support
 parameters:		DMA configuration struct
 returned value:	none
------------------------------------------------------------------------------*/
void uart0InitDma (const struct uartDmaConf *dma)
{
	/* UART TX GPDMA */
	uartDma[0].txConf.mode = gpdmaM2P;						/* memory to uart */
	uartDma[0].txConf.srcWidth = gpdmaS_WIDTH8;
	uartDma[0].txConf.dstWidth = gpdmaD_WIDTH8;				/* uart word width is 1 byte */
	uartDma[0].txConf.sbSize = gpdmaSB_SIZE4;				/* internal GPDMA fifo is 4 word deep */
	uartDma[0].txConf.dbSize = gpdmaDB_SIZE8;				/* uart has 16 byte TX/RX fifo */
	uartDma[0].txConf.srcPeriph = 0;						/* if memory than 0 */
	uartDma[0].txConf.dstPeriph = gpdmaDST_PERIPH_UART0TX;	/* uart 0 TX is destination */
	uartDma[0].txConf.flowCtrl = gpdmaFLOW_CTRL_M2P_DMA;	/* dma is flow controller */
	uartDma[0].txConf.srcAddr = (uint32)u0buff;				/* source in memory */
	uartDma[0].txConf.dstAddr = (uint32)&LPC_UART0->THR;	/* destination uart's fifo */
	if (dma->txEndService == 0)
	{
		uartDma[0].txConf.transferEndService = dummyService;
	}
	else
	{
		uartDma[0].txConf.transferEndService = dma->txEndService;
	}
	if (dma->error == 0)
	{
		uartDma[0].txConf.errorService = dummyService;
	}
	else
	{
		uartDma[0].txConf.errorService = dma->error;
	}

	/* UART RX GPDMA */
	uartDma[0].rxConf.mode = gpdmaP2M;						/* memory to uart */
	uartDma[0].rxConf.srcWidth = gpdmaS_WIDTH8;				/* uart word width is 1 byte */
	uartDma[0].rxConf.dstWidth = gpdmaD_WIDTH8;
	uartDma[0].rxConf.sbSize = gpdmaSB_SIZE8;				/* uart has 16 byte TX/RX fifo */
	uartDma[0].rxConf.dbSize = gpdmaDB_SIZE4;				/* internal GPDMA fifo is 4 word deep */
	uartDma[0].rxConf.srcPeriph = gpdmaSRC_PERIPH_UART0RX;	/* uart 0 RX is source */
	uartDma[0].rxConf.dstPeriph = 0;						/* if memory than 0 */
	uartDma[0].rxConf.flowCtrl = gpdmaFLOW_CTRL_P2M_DMA;	/* dma is flow controller */
	uartDma[0].rxConf.srcAddr = (uint32)&LPC_UART0->RBR;	/* source- uart's fifo */
	uartDma[0].rxConf.dstAddr = (uint32)u0buff;				/* dst in memory */
	uartDma[0].rxConf.transferEndService = u0DmaEndRx;
	if (dma->rxEndService == 0)
	{
		uartDma[0].rxUsrService = dummyService;
	}
	else
	{
		uartDma[0].rxUsrService = dma->rxEndService;
	}

	if (dma->error == 0)
	{
		uartDma[0].rxConf.errorService = dummyService;
	}
	else
	{
		uartDma[0].rxConf.errorService = dma->error;
	}

	switch(dma->txChnl)
	{
	case gpdmaCHNL0:
		uartDma[0].txChnlInit = gpdmaChnl0Init;
		uartDma[0].txChnlEnable = gpdmaChnl0Enable;
		break;
	case gpdmaCHNL1:
		uartDma[0].txChnlInit = gpdmaChnl1Init;
		uartDma[0].txChnlEnable = gpdmaChnl1Enable;
		break;
	case gpdmaCHNL2:
		uartDma[0].txChnlInit = gpdmaChnl2Init;
		uartDma[0].txChnlEnable = gpdmaChnl2Enable;
		break;
	case gpdmaCHNL3:
		uartDma[0].txChnlInit = gpdmaChnl3Init;
		uartDma[0].txChnlEnable = gpdmaChnl3Enable;
		break;
	case gpdmaCHNL4:
		uartDma[0].txChnlInit = gpdmaChnl4Init;
		uartDma[0].txChnlEnable = gpdmaChnl4Enable;
		break;
	case gpdmaCHNL5:
		uartDma[0].txChnlInit = gpdmaChnl5Init;
		uartDma[0].txChnlEnable = gpdmaChnl5Enable;
		break;
	case gpdmaCHNL6:
		uartDma[0].txChnlInit = gpdmaChnl6Init;
		uartDma[0].txChnlEnable = gpdmaChnl6Enable;
		break;
	case gpdmaCHNL7:
		uartDma[0].txChnlInit = gpdmaChnl7Init;
		uartDma[0].txChnlEnable = gpdmaChnl7Enable;
		break;
	}

	switch(dma->rxChnl)
	{
	case gpdmaCHNL0:
		uartDma[0].rxChnlInit = gpdmaChnl0Init;
		uartDma[0].rxChnlEnable = gpdmaChnl0Enable;
		break;
	case gpdmaCHNL1:
		uartDma[0].rxChnlInit = gpdmaChnl1Init;
		uartDma[0].rxChnlEnable = gpdmaChnl1Enable;
		break;
	case gpdmaCHNL2:
		uartDma[0].rxChnlInit = gpdmaChnl2Init;
		uartDma[0].rxChnlEnable = gpdmaChnl2Enable;
		break;
	case gpdmaCHNL3:
		uartDma[0].rxChnlInit = gpdmaChnl3Init;
		uartDma[0].rxChnlEnable = gpdmaChnl3Enable;
		break;
	case gpdmaCHNL4:
		uartDma[0].rxChnlInit = gpdmaChnl4Init;
		uartDma[0].rxChnlEnable = gpdmaChnl4Enable;
		break;
	case gpdmaCHNL5:
		uartDma[0].rxChnlInit = gpdmaChnl5Init;
		uartDma[0].rxChnlEnable = gpdmaChnl5Enable;
		break;
	case gpdmaCHNL6:
		uartDma[0].rxChnlInit = gpdmaChnl6Init;
		uartDma[0].rxChnlEnable = gpdmaChnl6Enable;
		break;
	case gpdmaCHNL7:
		uartDma[0].rxChnlInit = gpdmaChnl7Init;
		uartDma[0].rxChnlEnable = gpdmaChnl7Enable;
		break;
	}

    /* turn on reset and configure fifo, enable DMA support */
	/* GPDMA BURST is half of uart fifo (8 chars), so triger level is set to 8 chars as well */
	LPC_UART1->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_DMA_ENABLE | FCR_RX_FIFO_TRIGGER_LVL_8CHAR;
	uartDma[0].txChnlSettings = (*uartDma[0].txChnlInit)(&uartDma[0].txConf);
	uartDma[0].rxChnlSettings = (*uartDma[0].rxChnlInit)(&uartDma[0].rxConf);
}

/*------------------------------------------------------------------------------
 function name:		uart1InitDma
 description: 		init UART DMA support
 parameters:		DMA configuration struct
 returned value:	none
------------------------------------------------------------------------------*/
void uart1InitDma (const struct uartDmaConf *dma)
{
	/* UART TX GPDMA */
	uartDma[1].txConf.mode = gpdmaM2P;						/* memory to uart */
	uartDma[1].txConf.srcWidth = gpdmaS_WIDTH8;
	uartDma[1].txConf.dstWidth = gpdmaD_WIDTH8;				/* uart word width is 1 byte */
	uartDma[1].txConf.sbSize = gpdmaSB_SIZE4;				/* internal GPDMA fifo is 4 word deep */
	uartDma[1].txConf.dbSize = gpdmaDB_SIZE8;				/* uart has 16 byte TX/RX fifo */
	uartDma[1].txConf.srcPeriph = 0;						/* if memory than 0 */
	uartDma[1].txConf.dstPeriph = gpdmaDST_PERIPH_UART0TX;	/* uart 0 TX is destination */
	uartDma[1].txConf.flowCtrl = gpdmaFLOW_CTRL_M2P_DMA;	/* dma is flow controller */
	uartDma[1].txConf.srcAddr = (uint32)u1buff;				/* source in memory */
	uartDma[1].txConf.dstAddr = (uint32)&LPC_UART1->THR;	/* destination uart's fifo */
	if (dma->txEndService == 0)
	{
		uartDma[1].txConf.transferEndService = dummyService;
	}
	else
	{
		uartDma[1].txConf.transferEndService = dma->txEndService;
	}
	if (dma->error == 0)
	{
		uartDma[1].txConf.errorService = dummyService;
	}
	else
	{
		uartDma[1].txConf.errorService = dma->error;
	}

	/* UART RX GPDMA */
	uartDma[1].rxConf.mode = gpdmaP2M;						/* memory to uart */
	uartDma[1].rxConf.srcWidth = gpdmaS_WIDTH8;				/* uart word width is 1 byte */
	uartDma[1].rxConf.dstWidth = gpdmaD_WIDTH8;
	uartDma[1].rxConf.sbSize = gpdmaSB_SIZE8;				/* uart has 16 byte TX/RX fifo */
	uartDma[1].rxConf.dbSize = gpdmaDB_SIZE4;				/* internal GPDMA fifo is 4 word deep */
	uartDma[1].rxConf.srcPeriph = gpdmaSRC_PERIPH_UART0RX;	/* uart 0 RX is source */
	uartDma[1].rxConf.dstPeriph = 0;						/* if memory than 0 */
	uartDma[1].rxConf.flowCtrl = gpdmaFLOW_CTRL_P2M_DMA;	/* dma is flow controller */
	uartDma[1].rxConf.srcAddr = (uint32)&LPC_UART1->RBR;	/* source- uart's fifo */
	uartDma[1].rxConf.dstAddr = (uint32)u1buff;				/* dst in memory */
	uartDma[1].rxConf.transferEndService = u1DmaEndRx;
	if (dma->rxEndService == 0)
	{
		uartDma[1].rxUsrService = dummyService;
	}
	else
	{
		uartDma[1].rxUsrService = dma->rxEndService;
	}

	if (dma->error == 0)
	{
		uartDma[1].rxConf.errorService = dummyService;
	}
	else
	{
		uartDma[1].rxConf.errorService = dma->error;
	}

	switch(dma->txChnl)
	{
	case gpdmaCHNL0:
		uartDma[1].txChnlInit = gpdmaChnl0Init;
		uartDma[1].txChnlEnable = gpdmaChnl0Enable;
		break;
	case gpdmaCHNL1:
		uartDma[1].txChnlInit = gpdmaChnl1Init;
		uartDma[1].txChnlEnable = gpdmaChnl1Enable;
		break;
	case gpdmaCHNL2:
		uartDma[1].txChnlInit = gpdmaChnl2Init;
		uartDma[1].txChnlEnable = gpdmaChnl2Enable;
		break;
	case gpdmaCHNL3:
		uartDma[1].txChnlInit = gpdmaChnl3Init;
		uartDma[1].txChnlEnable = gpdmaChnl3Enable;
		break;
	case gpdmaCHNL4:
		uartDma[1].txChnlInit = gpdmaChnl4Init;
		uartDma[1].txChnlEnable = gpdmaChnl4Enable;
		break;
	case gpdmaCHNL5:
		uartDma[1].txChnlInit = gpdmaChnl5Init;
		uartDma[1].txChnlEnable = gpdmaChnl5Enable;
		break;
	case gpdmaCHNL6:
		uartDma[1].txChnlInit = gpdmaChnl6Init;
		uartDma[1].txChnlEnable = gpdmaChnl6Enable;
		break;
	case gpdmaCHNL7:
		uartDma[1].txChnlInit = gpdmaChnl7Init;
		uartDma[1].txChnlEnable = gpdmaChnl7Enable;
		break;
	}

	switch(dma->rxChnl)
	{
	case gpdmaCHNL0:
		uartDma[1].rxChnlInit = gpdmaChnl0Init;
		uartDma[1].rxChnlEnable = gpdmaChnl0Enable;
		break;
	case gpdmaCHNL1:
		uartDma[1].rxChnlInit = gpdmaChnl1Init;
		uartDma[1].rxChnlEnable = gpdmaChnl1Enable;
		break;
	case gpdmaCHNL2:
		uartDma[1].rxChnlInit = gpdmaChnl2Init;
		uartDma[1].rxChnlEnable = gpdmaChnl2Enable;
		break;
	case gpdmaCHNL3:
		uartDma[1].rxChnlInit = gpdmaChnl3Init;
		uartDma[1].rxChnlEnable = gpdmaChnl3Enable;
		break;
	case gpdmaCHNL4:
		uartDma[1].rxChnlInit = gpdmaChnl4Init;
		uartDma[1].rxChnlEnable = gpdmaChnl4Enable;
		break;
	case gpdmaCHNL5:
		uartDma[1].rxChnlInit = gpdmaChnl5Init;
		uartDma[1].rxChnlEnable = gpdmaChnl5Enable;
		break;
	case gpdmaCHNL6:
		uartDma[1].rxChnlInit = gpdmaChnl6Init;
		uartDma[1].rxChnlEnable = gpdmaChnl6Enable;
		break;
	case gpdmaCHNL7:
		uartDma[1].rxChnlInit = gpdmaChnl7Init;
		uartDma[1].rxChnlEnable = gpdmaChnl7Enable;
		break;
	}

    /* turn on reset and configure fifo, enable DMA support */
	/* GPDMA BURST is half of uart fifo (8 chars), so triger level is set to 8 chars as well */
	LPC_UART1->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_DMA_ENABLE | FCR_RX_FIFO_TRIGGER_LVL_8CHAR;
	uartDma[1].txChnlSettings = (*uartDma[1].txChnlInit)(&uartDma[1].txConf);
	uartDma[1].rxChnlSettings = (*uartDma[1].rxChnlInit)(&uartDma[1].rxConf);
}
/*------------------------------------------------------------------------------
 function name:		uart2InitDma
 description: 		init UART DMA support
 parameters:		user services at the end of TX/RX, error service
 returned value:	none
------------------------------------------------------------------------------*/
void uart2InitDma (const struct uartDmaConf *dma)
{
	{
		/* set UART TX GPDMA config struct */
		uartDma[2].txConf.mode = gpdmaM2P;						/* memory to uart */
		uartDma[2].txConf.srcWidth = gpdmaS_WIDTH8;
		uartDma[2].txConf.dstWidth = gpdmaD_WIDTH8;			/* uart word width is 1 byte */
		uartDma[2].txConf.sbSize = gpdmaSB_SIZE4;				/* internal GPDMA fifo is 4 word deep */
		uartDma[2].txConf.dbSize = gpdmaDB_SIZE8;				/* uart has 16 byte TX/RX fifo */
		uartDma[2].txConf.srcPeriph = 0;						/* if memory than 0 */
		uartDma[2].txConf.dstPeriph = gpdmaDST_PERIPH_UART2TX;	/* uart 2 TX is destination */
		uartDma[2].txConf.flowCtrl = gpdmaFLOW_CTRL_M2P_DMA;	/* dma is flow controller */
		//uartDma[0].txConf.srcAddr = 							/* source is updated later */
		uartDma[2].txConf.dstAddr = (uint32)&LPC_UART2->THR;
		if (dma->txEndService == 0)
		{
			uartDma[2].txConf.transferEndService = dummyService;
		}
		else
		{
			uartDma[2].txConf.transferEndService = dma->txEndService;
		}
		if (dma->error == 0)
		{
			uartDma[2].txConf.errorService = dummyService;
		}
		else
		{
			uartDma[2].txConf.errorService = dma->error;
		}

		/* set UART RX GPDMA config struct */
		uartDma[2].rxConf.mode = gpdmaP2M;						/* memory to uart */
		uartDma[2].rxConf.srcWidth = gpdmaS_WIDTH8;			/* uart word width is 1 byte */
		uartDma[2].rxConf.dstWidth = gpdmaD_WIDTH8;
		uartDma[2].rxConf.sbSize = gpdmaSB_SIZE8;				/* uart has 16 byte TX/RX fifo */
		uartDma[2].rxConf.dbSize = gpdmaDB_SIZE4;				/* internal GPDMA fifo is 4 word deep */
		uartDma[2].rxConf.srcPeriph = gpdmaSRC_PERIPH_UART2RX;	/* uart 0 RX is source */
		uartDma[2].rxConf.dstPeriph = 0;						/* if memory than 0 */
		uartDma[2].rxConf.flowCtrl = gpdmaFLOW_CTRL_P2M_DMA;	/* dma is flow controller */
		uartDma[2].rxConf.srcAddr = (uint32)&LPC_UART2->RBR;
		//uartDma[0].rxConf.dstAddr = 							/* dst is updated later */
		if (dma->rxEndService == 0)
		{
			uartDma[2].rxConf.transferEndService = dummyService;
		}
		else
		{
			uartDma[2].rxConf.transferEndService = dma->rxEndService;
		}
		if (dma->error == 0)
		{
			uartDma[2].rxConf.errorService = dummyService;
		}
		else
		{
			uartDma[2].rxConf.errorService = dma->error;
		}

	    /* turn on reset and configure fifo, enable DMA support */
		/* GPDMA BURST is half of uart fifo (8 chars), so triger level is set to 8 chars as well */
		LPC_UART2->FCR = FCR_FIFO_ENABLE | FCR_FIFO_RST_RX | FCR_FIFO_RST_TX | FCR_DMA_ENABLE | FCR_RX_FIFO_TRIGGER_LVL_8CHAR;
	}
}

/*------------------------------------------------------------------------------
 function name:   uart0DmaTx
 description:     send data via GDPMA
 parameters:      TX buffer, size
 returned value:  none
------------------------------------------------------------------------------*/
void uart0DmaTx (const uint8 *tx, const uint32 size)
{
	memcpy(u0buff, tx, size);									/* copy data to GPDMA buffer */
	uartDma[0].txChnlEnable(uartDma[0].txChnlSettings, size);	/* enable channel */
}

/*------------------------------------------------------------------------------
 function name:   uart0DmaRx
 description:     send data via GDPMA
 parameters:      TX buffer, size
 returned value:  none
------------------------------------------------------------------------------*/
void uart0DmaRx (uint8 *rx, const uint32 size)
{
	uartDma[0].rxUsrBuff = rx;									/* all RX data need to be saved */
	uartDma[0].rxUsrSize = size;
	uartDma[0].rxChnlEnable(uartDma[0].rxChnlSettings, size);
}

/*------------------------------------------------------------------------------
 function name:   uart1DmaTx
 description:     send data via GDPMA
 parameters:      TX buffer, size
 returned value:  none
------------------------------------------------------------------------------*/
void uart1DmaTx (const uint8 *tx, const uint32 size)
{
	memcpy(u1buff, tx, size);									/* copy data to GPDMA buffer */
	uartDma[1].txChnlEnable(uartDma[1].txChnlSettings, size);	/* enable channel */
}

/*------------------------------------------------------------------------------
 function name:   uart0DmaRx
 description:     send data via GDPMA
 parameters:      TX buffer, size
 returned value:  none
------------------------------------------------------------------------------*/
void uart1DmaRx (uint8 *rx, const uint32 size)
{
	uartDma[1].rxUsrBuff = rx;									/* all RX data need to be saved */
	uartDma[1].rxUsrSize = size;
	uartDma[1].rxChnlEnable(uartDma[1].rxChnlSettings, size);
}

/*------------------------------------------------------------------------------
 function name:   uart2DmaTx
 description:     send data via GDPMA
 parameters:      TX buffer, size
 returned value:  none
------------------------------------------------------------------------------*/
void uart2DmaTx (const uint8 *tx, const uint32 size)
{
	uartDma[2].txConf.srcAddr = (uint32)tx;

	//uart2DmaChnl = gpdmaChnl2Init(&uartDma[2].txConf);
	uartDma[2].txChnlSettings = (*uartDma[2].txChnlInit)(&uartDma[2].txConf);
	//gpdmaChnl2Enable(uart2DmaChnl, size);
	uartDma[2].txChnlEnable(uartDma[2].txChnlSettings, size);
}

/*------------------------------------------------------------------------------
 function name:   uart2DmaRx
 description:     send data via GDPMA
 parameters:      RX buffer, size
 returned value:  none
------------------------------------------------------------------------------*/
void uart2DmaRx(uint8 *rx, const uint32 size)
{
	/* update GPDMA configuration struct */
	uartDma[2].rxConf.dstAddr = (uint32)rx;

	uartDma[2].rxChnlSettings = (*uartDma[2].rxChnlInit)(&uartDma[2].rxConf);
	uartDma[2].rxChnlEnable(uartDma[2].rxChnlSettings, size);
}

/*------------------------------------------------------------------------------
 function name:		uart0PutChar
 description:		function used by uprintf(), sends a char to the uart
 parameters:		character to send
 returned value:	none
------------------------------------------------------------------------------*/
void uart0PutChar (char character)
{
	while (!(LPC_UART0->LSR & LSR_THR_EMPTY));
	LPC_UART0->THR = character;
}

/*------------------------------------------------------------------------------
 function name:		uart2PutChar
 description:		function used by uprintf(), sends a char to the uart
 parameters:		character to send
 returned value:	none
------------------------------------------------------------------------------*/
void uart2PutChar (char character)
{
	while (!(LPC_UART2->LSR & LSR_THR_EMPTY));
	LPC_UART2->THR = character;
}
/*==============================================================================
 Static function definitions
==============================================================================*/
/*------------------------------------------------------------------------------
 function name:   u0DmaEndRx
 description:     function called when GDPMA transfer is completed
 parameters:      none
 returned value:  none
------------------------------------------------------------------------------*/
static void u0DmaEndRx (void)
{
	memcpy(uartDma[0].rxUsrBuff, u0buff, uartDma[0].rxUsrSize);
	(*uartDma[0].rxUsrService)();								/* call user service */
}
/*------------------------------------------------------------------------------
 function name:   u0DmaEndRx
 description:     function called when GDPMA transfer is completed
 parameters:      none
 returned value:  none
------------------------------------------------------------------------------*/
static void u1DmaEndRx (void)
{
	memcpy(uartDma[1].rxUsrBuff, u1buff, uartDma[1].rxUsrSize);
	(*uartDma[1].rxUsrService)();								/* call user service */
}
/*------------------------------------------------------------------------------
 function name:   dummyService
 description:     does nothing
 parameters:      none
 returned value:  none
------------------------------------------------------------------------------*/
static void dummyService (void)
{
	return;
}

/*==============================================================================
 ISR definitions
==============================================================================*/
/*------------------------------------------------------------------------------
 function name:   UART0_IRQHandler
 description:     UART0 ISR
 parameters:      none
 returned value:  none
------------------------------------------------------------------------------*/
void UART0_IRQHandler (void)
{
	volatile uint32 status;
	uint32 i;

	status = LPC_UART0->IIR;

	/* TX fifo empty */
	if ((status & IIR_IRQ_ID_MASK) == IIR_THRE)
	{
		for (i = 0; i < uartPrivate[0].txBuffSize; ++i)
		{
			LPC_UART0->THR = uartPrivate[0].txBuff[i];		/* fill fifo */
		}
		uartPrivate[0].txIrqService();
	}

	/* timeout */
	if ((status & IIR_IRQ_ID_MASK) == IIR_CTI)
	{
		u0buff[0] = LPC_UART0->RBR;
		uartPrivate[0].rxBuff[0] = u0buff[0];
		(*uartPrivate[0].timeout)();
	}

	/* received data available */
	if ((status & IIR_IRQ_ID_MASK) == IIR_RDA)
	{
		switch (uartPrivate[0].rxTriggerLvl)
		{
		case uartTRIGER_LEVEL_1CHAR:
			u0buff[0] = LPC_UART0->RBR;
			uartPrivate[0].rxBuff[0] = u0buff[0];
			break;

		case uartTRIGER_LEVEL_4CHAR:
			u0buff[0] = LPC_UART0->RBR;
			u0buff[1] = LPC_UART0->RBR;
			u0buff[2] = LPC_UART0->RBR;
			u0buff[3] = LPC_UART0->RBR;
			memcpy(uartPrivate[0].rxBuff, u0buff, 4);
			break;

		case uartTRIGER_LEVEL_8CHAR:
			u0buff[0] = LPC_UART0->RBR;
			u0buff[1] = LPC_UART0->RBR;
			u0buff[2] = LPC_UART0->RBR;
			u0buff[3] = LPC_UART0->RBR;
			u0buff[4] = LPC_UART0->RBR;
			u0buff[5] = LPC_UART0->RBR;
			u0buff[6] = LPC_UART0->RBR;
			u0buff[7] = LPC_UART0->RBR;
			memcpy(uartPrivate[0].rxBuff, u0buff, 8);
			break;

		case uartTRIGER_LEVEL_14CHAR:
			u0buff[0] = LPC_UART0->RBR;
			u0buff[1] = LPC_UART0->RBR;
			u0buff[2] = LPC_UART0->RBR;
			u0buff[3] = LPC_UART0->RBR;
			u0buff[4] = LPC_UART0->RBR;
			u0buff[5] = LPC_UART0->RBR;
			u0buff[6] = LPC_UART0->RBR;
			u0buff[7] = LPC_UART0->RBR;
			u0buff[8] = LPC_UART0->RBR;
			u0buff[9] = LPC_UART0->RBR;
			u0buff[10] = LPC_UART0->RBR;
			u0buff[11] = LPC_UART0->RBR;
			u0buff[12] = LPC_UART0->RBR;
			u0buff[13] = LPC_UART0->RBR;
			memcpy(uartPrivate[0].rxBuff, u0buff, 14);
			break;

		default:
			u0buff[0] = LPC_UART0->RBR;
			u0buff[1] = LPC_UART0->RBR;
			u0buff[2] = LPC_UART0->RBR;
			u0buff[3] = LPC_UART0->RBR;
			u0buff[4] = LPC_UART0->RBR;
			u0buff[5] = LPC_UART0->RBR;
			u0buff[6] = LPC_UART0->RBR;
			u0buff[7] = LPC_UART0->RBR;
			memcpy(uartPrivate[0].rxBuff, u0buff, 8);
			break;
		}
		(*uartPrivate[0].rxIrqService)();
	}

}

/*------------------------------------------------------------------------------
 function name:   UART1_IRQHandler
 description:     UART1 ISR
 parameters:      none
 returned value:  none
------------------------------------------------------------------------------*/
void UART1_IRQHandler (void)
{
	volatile uint32 status;
	uint32 i;

	status = LPC_UART1->IIR;

	/* TX fifo empty */
	if ((status & IIR_IRQ_ID_MASK) == IIR_THRE)
	{
		for (i = 0; i < uartPrivate[1].txBuffSize; ++i)
		{
			LPC_UART1->THR = uartPrivate[1].txBuff[i];		/* fill fifo */
		}
		uartPrivate[1].txIrqService();
	}

	/* timeout */
	if ((status & IIR_IRQ_ID_MASK) == IIR_CTI)
	{
		u1buff[1] = LPC_UART1->RBR;
		uartPrivate[1].rxBuff[1] = u1buff[1];
		(*uartPrivate[1].timeout)();
	}

	/* received data available */
	if ((status & IIR_IRQ_ID_MASK) == IIR_RDA)
	{
		switch (uartPrivate[1].rxTriggerLvl)
		{
		case uartTRIGER_LEVEL_1CHAR:
			u1buff[1] = LPC_UART1->RBR;
			uartPrivate[1].rxBuff[1] = u1buff[1];
			break;

		case uartTRIGER_LEVEL_4CHAR:
			u1buff[0] = LPC_UART1->RBR;
			u1buff[1] = LPC_UART1->RBR;
			u1buff[2] = LPC_UART1->RBR;
			u1buff[3] = LPC_UART1->RBR;
			memcpy(uartPrivate[1].rxBuff, u1buff, 4);
			break;

		case uartTRIGER_LEVEL_8CHAR:
			u1buff[0] = LPC_UART1->RBR;
			u1buff[1] = LPC_UART1->RBR;
			u1buff[2] = LPC_UART1->RBR;
			u1buff[3] = LPC_UART1->RBR;
			u1buff[4] = LPC_UART1->RBR;
			u1buff[5] = LPC_UART1->RBR;
			u1buff[6] = LPC_UART1->RBR;
			u1buff[7] = LPC_UART1->RBR;
			memcpy(uartPrivate[1].rxBuff, u1buff, 8);
			break;

		case uartTRIGER_LEVEL_14CHAR:
			u1buff[0] = LPC_UART1->RBR;
			u1buff[1] = LPC_UART1->RBR;
			u1buff[2] = LPC_UART1->RBR;
			u1buff[3] = LPC_UART1->RBR;
			u1buff[4] = LPC_UART1->RBR;
			u1buff[5] = LPC_UART1->RBR;
			u1buff[6] = LPC_UART1->RBR;
			u1buff[7] = LPC_UART1->RBR;
			u1buff[8] = LPC_UART1->RBR;
			u1buff[9] = LPC_UART1->RBR;
			u1buff[10] = LPC_UART1->RBR;
			u1buff[11] = LPC_UART1->RBR;
			u1buff[12] = LPC_UART1->RBR;
			u1buff[13] = LPC_UART1->RBR;
			memcpy(uartPrivate[1].rxBuff, u1buff, 14);
			break;

		default:
			u1buff[0] = LPC_UART1->RBR;
			u1buff[1] = LPC_UART1->RBR;
			u1buff[2] = LPC_UART1->RBR;
			u1buff[3] = LPC_UART1->RBR;
			u1buff[4] = LPC_UART1->RBR;
			u1buff[5] = LPC_UART1->RBR;
			u1buff[6] = LPC_UART1->RBR;
			u1buff[7] = LPC_UART1->RBR;
			memcpy(uartPrivate[1].rxBuff, u1buff, 8);
			break;
		}
		(*uartPrivate[0].rxIrqService)();
	}

}


/*------------------------------------------------------------------------------
 function name:   UART2_IRQHandler
 description:     UART2 ISR
 parameters:      none
 returned value:  none
------------------------------------------------------------------------------*/
void UART2_IRQHandler (void)
{
	volatile uint32 status;

	status = LPC_UART2->IIR;

	/* TX fifo empty */
	if ((status & IIR_IRQ_ID_MASK) == IIR_THRE)
	{
		LPC_UART2->THR = uartPrivate[2].txBuff[0];		/* fill fifo */
		LPC_UART2->THR = uartPrivate[2].txBuff[1];
		LPC_UART2->THR = uartPrivate[2].txBuff[2];
		LPC_UART2->THR = uartPrivate[2].txBuff[3];
		LPC_UART2->THR = uartPrivate[2].txBuff[4];
		LPC_UART2->THR = uartPrivate[2].txBuff[5];
		LPC_UART2->THR = uartPrivate[2].txBuff[6];
		LPC_UART2->THR = uartPrivate[2].txBuff[7];

		LPC_UART2->THR = uartPrivate[2].txBuff[8];
		LPC_UART2->THR = uartPrivate[2].txBuff[9];
		LPC_UART2->THR = uartPrivate[2].txBuff[10];
		LPC_UART2->THR = uartPrivate[2].txBuff[11];
		LPC_UART2->THR = uartPrivate[2].txBuff[12];
		LPC_UART2->THR = uartPrivate[2].txBuff[13];
		LPC_UART2->THR = uartPrivate[2].txBuff[14];
		LPC_UART2->THR = uartPrivate[2].txBuff[15];
		uartPrivate[2].txIrqService();
	}

	/* timeout */
	if ((status & IIR_IRQ_ID_MASK) == IIR_CTI)
	{
		u2buff[0] = LPC_UART2->RBR;
		uartPrivate[2].rxBuff[0] = u2buff[0];
		(*uartPrivate[2].timeout)();
	}

	/* received data available */
	if ((status & IIR_IRQ_ID_MASK) == IIR_RDA)
	{
		switch (uartPrivate[2].rxTriggerLvl)
		{
		case uartTRIGER_LEVEL_1CHAR:
			u2buff[0] = LPC_UART2->RBR;
			uartPrivate[2].rxBuff[0] = u2buff[0];
			break;

		case uartTRIGER_LEVEL_4CHAR:
			u2buff[0] = LPC_UART2->RBR;
			u2buff[1] = LPC_UART2->RBR;
			u2buff[2] = LPC_UART2->RBR;
			u2buff[3] = LPC_UART2->RBR;
			memcpy(uartPrivate[2].rxBuff, u2buff, 4);
			break;

		case uartTRIGER_LEVEL_8CHAR:
			u2buff[0] = LPC_UART2->RBR;
			u2buff[1] = LPC_UART2->RBR;
			u2buff[2] = LPC_UART2->RBR;
			u2buff[3] = LPC_UART2->RBR;
			u2buff[4] = LPC_UART2->RBR;
			u2buff[5] = LPC_UART2->RBR;
			u2buff[6] = LPC_UART2->RBR;
			u2buff[7] = LPC_UART2->RBR;
			memcpy(uartPrivate[2].rxBuff, u2buff, 8);
			break;

		case uartTRIGER_LEVEL_14CHAR:
			u2buff[0] = LPC_UART2->RBR;
			u2buff[1] = LPC_UART2->RBR;
			u2buff[2] = LPC_UART2->RBR;
			u2buff[3] = LPC_UART2->RBR;
			u2buff[4] = LPC_UART2->RBR;
			u2buff[5] = LPC_UART2->RBR;
			u2buff[6] = LPC_UART2->RBR;
			u2buff[7] = LPC_UART2->RBR;
			u2buff[8] = LPC_UART2->RBR;
			u2buff[9] = LPC_UART2->RBR;
			u2buff[10] = LPC_UART2->RBR;
			u2buff[11] = LPC_UART2->RBR;
			u2buff[12] = LPC_UART2->RBR;
			u2buff[13] = LPC_UART2->RBR;
			memcpy(uartPrivate[2].rxBuff, u2buff, 14);
			break;

		default:
			u2buff[0] = LPC_UART2->RBR;
			u2buff[1] = LPC_UART2->RBR;
			u2buff[2] = LPC_UART2->RBR;
			u2buff[3] = LPC_UART2->RBR;
			u2buff[4] = LPC_UART2->RBR;
			u2buff[5] = LPC_UART2->RBR;
			u2buff[6] = LPC_UART2->RBR;
			u2buff[7] = LPC_UART2->RBR;
			memcpy(uartPrivate[0].rxBuff, u2buff, 8);
			break;
		}
		(*uartPrivate[2].rxIrqService)();
	}

}
/******************************************************************************
* END OF FILE
******************************************************************************/



