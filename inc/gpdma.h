#ifndef GPDMA_H_
#define GPDMA_H_
/*******************************************************************************
 * File name: 		gpdma.h
 * Description: 
 * Project: 		gpdma
 * Target: 			LPC1788
 * Compiler: 		arm-none-eabi-gcc
 * Date: 			Sep 28, 2012
 * Author: 			kuba
 * Based on: 		---
 ******************************************************************************/

/*==============================================================================
 Includes
==============================================================================*/
# include "typedef.h"
# include "irq.h"
/*==============================================================================
 Defines
==============================================================================*/

/*==============================================================================
 Types
==============================================================================*/
enum gpdmaChnl {
	gpdmaCHNL0,
	gpdmaCHNL1,
	gpdmaCHNL2,
	gpdmaCHNL3,
	gpdmaCHNL4,
	gpdmaCHNL5,
	gpdmaCHNL6,
	gpdmaCHNL7
};

enum mode {
   gpdmaM2M,       			/* memory to memory */
   gpdmaM2P,            	/* memory to peripheral */
   gpdmaP2M,               	/* peripheral to memory */
   gpdmaP2P					/* peripheral to peripheral */
};

enum sbSize {
   gpdmaSB_SIZE1,    		/* 1 word */
   gpdmaSB_SIZE4,    		/* 4 words */
   gpdmaSB_SIZE8,    		/* 8 words */
   gpdmaSB_SIZE16,   		/* and so on.. */
   gpdmaSB_SIZE32,
   gpdmaSB_SIZE64,
   gpdmaSB_SIZE128,
   sgpdmaSB_SIZE256
};

enum dbSize {
   gpdmaDB_SIZE1,    		/* 1 word */
   gpdmaDB_SIZE4,    		/* 4 words */
   gpdmaDB_SIZE8,    		/* 8 words */
   gpdmaDB_SIZE16,   		/* and so on.. */
   gpdmaDB_SIZE32,
   gpdmaDB_SIZE64,
   gpdmaDB_SIZE128,
   gpdmaDB_SIZE256
};

enum sWidth {
   gpdmaS_WIDTH8,        	/* 8 bits */
   gpdmaS_WIDTH16,       	/* 16 bits */
   gpdmaS_WIDTH32        	/* 32 bits */
};

enum dWidth {
   gpdmaD_WIDTH8,        	/* 8 bits */
   gpdmaD_WIDTH16,       	/* 16 bits */
   gpdmaD_WIDTH32        	/* 32 bits */
};

enum srcPeriph {
	gpdmaSRC_PERIPH_SD,			//0
	gpdmaSRC_PERIPH_SSP0TX,		//1
	gpdmaSRC_PERIPH_SSP0RX,		//2
	gpdmaSRC_PERIPH_SSP1TX,		//3
	gpdmaSRC_PERIPH_SSP1RX,		//4
	gpdmaSRC_PERIPH_SSP2TX,		//5
	gpdmaSRC_PERIPH_SSP2RX,		//6
	gpdmaSRC_PERIPH_ADC,		//7
	gpdmaSRC_PERIPH_DAC,		//8
	gpdmaSRC_PERIPH_UART0TX,	//9
	gpdmaSRC_PERIPH_UART0RX,	//10
	gpdmaSRC_PERIPH_UART1TX,	//11
	gpdmaSRC_PERIPH_UART1RX,	//12
	gpdmaSRC_PERIPH_UART2TX,	//13
	gpdmaSRC_PERIPH_UART2RX,	//14
	gpdmaSRC_PERIPH_TIMER0MAT0,	//15	//0
	gpdmaSRC_PERIPH_TIMER0MAT1,	//16	//1
	gpdmaSRC_PERIPH_TIMER1MAT0,	//17	//2
	gpdmaSRC_PERIPH_TIMER1MAT1,	//18	//3
	gpdmaSRC_PERIPH_TIMER2MAT0,	//19	//4
	gpdmaSRC_PERIPH_TIMER2MAT1,	//20	//5
	gpdmaSRC_PERIPH_I2SCH0,		//21	//6
	gpdmaSRC_PERIPH_I2SCH1,		//22	//7
	gpdmaSRC_PERIPH_UART3TX,	//23	//8
	gpdmaSRC_PERIPH_UART3RX,	//24	//9
	gpdmaSRC_PERIPH_UART4TX,	//25	//10
	gpdmaSRC_PERIPH_UART4RX,	//26	//11
	gpdmaSRC_PERIPH_TIMER3MAT0,	//27	//12
	gpdmaSRC_PERIPH_TIMER3MAT1	//28	//13
};

enum dstPeriph {
	gpdmaDST_PERIPH_SD,
	gpdmaDST_PERIPH_SSP0TX,
	gpdmaDST_PERIPH_SSP0RX,
	gpdmaDST_PERIPH_SSP1TX,
	gpdmaDST_PERIPH_SSP1RX,
	gpdmaDST_PERIPH_SSP2TX,
	gpdmaDST_PERIPH_SSP2RX,
	gpdmaDST_PERIPH_ADC,
	gpdmaDST_PERIPH_DAC,
	gpdmaDST_PERIPH_UART0TX,
	gpdmaDST_PERIPH_UART0RX,
	gpdmaDST_PERIPH_UART1TX,
	gpdmaDST_PERIPH_UART1RX,
	gpdmaDST_PERIPH_UART2TX,
	gpdmaDST_PERIPH_UART2RX,
	gpdmaDST_PERIPH_TIMER0MAT0,
	gpdmaDST_PERIPH_TIMER0MAT1,
	gpdmaDST_PERIPH_TIMER1MAT0,
	gpdmaDST_PERIPH_TIMER1MAT1,
	gpdmaDST_PERIPH_TIMER2MAT0,
	gpdmaDST_PERIPH_TIMER2MAT1,
	gpdmaDST_PERIPH_I2SCH0,
	gpdmaDST_PERIPH_I2SCH1,
	gpdmaDST_PERIPH_UART3TX,
	gpdmaDST_PERIPH_UART3RX,
	gpdmaDST_PERIPH_UART4TX,
	gpdmaDST_PERIPH_UART4RX,
	gpdmaDST_PERIPH_TIMER3MAT0,
	gpdmaDST_PERIPH_TIMER3MAT1
};

enum flowCtrl {
   gpdmaFLOW_CTRL_M2M_DMA,	/* memory to memory transfer, flow control: DMA */
   gpdmaFLOW_CTRL_M2P_DMA,	/* memory to periph transfer, flow control: DMA */
   gpdmaFLOW_CTRL_P2M_DMA, 	/* periph to memory transfer, flow control: DMA */
   gpdmaFLOW_CTRL_P2P_DMA, 	/* periph to periph transfer, flow control: DMA */
   gpdmaFLOW_CTRL_P2P_DP,  	/* periph to periph transfer, flow control: destination periph */
   gpdmaFLOW_CTRL_M2P_P,   	/* memory to periph transfer, flow control: periph */
   gpdmaFLOW_CTRL_P2M_P,   	/* periph to memory transfer, flow control: periph */
   gpdmaFLOW_CTRL_P2P_SP   	/* periph to periph transfer, flow control: source periph */
};

struct gpdmaConf {
   enum mode         mode;                         /* type of transfer */
   enum sWidth       srcWidth;                     /* source word width */
   enum dWidth       dstWidth;                     /* destination word width */
   enum sbSize       sbSize;                       /* source burst size */
   enum dbSize       dbSize;                       /* destination burst size */
   enum srcPeriph    srcPeriph;                    /* source peripheral */
   enum dstPeriph    dstPeriph;                    /* destination peripheral */
   enum flowCtrl     flowCtrl;                     /* flow control */
   uint32            srcAddr;                      /* source address */
   uint32            dstAddr;                      /* destination address */
   void              (*transferEndService)(void);  /* user service at the end of transfer */
   void              (*errorService)(void);        /* user service in case of error */
};

typedef struct gpdmaChannel *gpdmaChannel;

/*==============================================================================
 Globals
==============================================================================*/

/*==============================================================================
 Global function prototypes
==============================================================================*/
/*------------------------------------------------------------------------------
 function name:   gpdmaInit
 description:     GPDMA initialization
 parameters:      interrupt priority
 returned value:  none
------------------------------------------------------------------------------*/
void gpdmaInit (enum irqPriority priority);

/*------------------------------------------------------------------------------
 function name:   gpdmaChnlNInit
 description:     channel 0 to 7 initialization
 parameters:      pointer to configuration struct
 returned value:  channel settings
------------------------------------------------------------------------------*/
gpdmaChannel gpdmaChnl0Init (const struct gpdmaConf *gpdmaConf);
gpdmaChannel gpdmaChnl1Init (const struct gpdmaConf *gpdmaConf);
gpdmaChannel gpdmaChnl2Init (const struct gpdmaConf *gpdmaConf);
gpdmaChannel gpdmaChnl3Init (const struct gpdmaConf *gpdmaConf);
gpdmaChannel gpdmaChnl4Init (const struct gpdmaConf *gpdmaConf);
gpdmaChannel gpdmaChnl5Init (const struct gpdmaConf *gpdmaConf);
gpdmaChannel gpdmaChnl6Init (const struct gpdmaConf *gpdmaConf);
gpdmaChannel gpdmaChnl7Init (const struct gpdmaConf *gpdmaConf);

/*------------------------------------------------------------------------------
 function name:   gpdmaChnlnEnable
 description:     enable channel 0 to 7
 parameters:      channel settings
 returned value:  none
------------------------------------------------------------------------------*/
void gpdmaChnl0Enable (const gpdmaChannel chnl, const uint16 size);
void gpdmaChnl1Enable (const gpdmaChannel chnl, const uint16 size);
void gpdmaChnl2Enable (const gpdmaChannel chnl, const uint16 size);
void gpdmaChnl3Enable (const gpdmaChannel chnl, const uint16 size);
void gpdmaChnl4Enable (const gpdmaChannel chnl, const uint16 size);
void gpdmaChnl5Enable (const gpdmaChannel chnl, const uint16 size);
void gpdmaChnl6Enable (const gpdmaChannel chnl, const uint16 size);
void gpdmaChnl7Enable (const gpdmaChannel chnl, const uint16 size);

/******************************************************************************
* END OF FILE
******************************************************************************/
#endif /* GPDMA_H_ */
