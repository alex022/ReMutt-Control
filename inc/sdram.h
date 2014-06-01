/**********************************************************************
* $Id$		lpc177x_8x_emc.h			2011-06-02
*//**
* @file		lpc177x_8x_emc.h
* @brief	Contains all macro definitions and function prototypes
*			support for EMC firmware library on LPC177x_8x
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

#ifndef __SDRAM_H__
#define __SDRAM_H__

#define tCLK_ns ((double)EMCClock / 1000000000.0 )                   // CCLK period in ns
#define NS_2_CLKS(ns) ( (uint32_t)( (double)(ns) * tCLK_ns ) + 1 )   // convert ns to CCLKs
#define ROW_BANK_COLUMN    // default is BANK_ROW_COLUMN (ROW_BANK_COLUMN is ~0.2% faster...
#define SDRAM_BASE_ADDR 0xA0000000
#define SDRAM_SIZE 0x02000000

extern uint32_t INIT_SDRAM(void);
extern void MemTest(void);
extern void PrintDataAtAddress(volatile unsigned int Address);
extern void itoh1(volatile unsigned int x);

#endif
