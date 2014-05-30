#include "LPC177x_8x.h"
#include "system_LPC177x_8x.h"
#include "typedef.h"
#include "sdram.h"
#include "uart.h"


/***********************************************************************************************
** Function name:    SDRAM_init
**
** Descriptions:     Initialize SDRAM (Synchronous Dynamic Random Access Memory)
**
** parameters:       None
** Returned value:   TRUE when complete
**
***********************************************************************************************/
uint32_t INIT_SDRAM( void )
   {
   uint32_t Temp = Temp;
   uint32_t CAS_Latency, RAS_Latency;  // these can be 2 or 3 clks
   uint32_t DelayConstant;

   // configure SDRAM control signals
   // ------------------------------------------------------------------------------------------
   LPC_IOCON->P2_16 |= 1;              // CASN @ P2.16         (SDRAM Column Address Strobe)
   LPC_IOCON->P2_17 |= 1;              // RASN @ P2.17         (SDRAM Row Address Strobe)
   LPC_IOCON->P2_18 |= 1;              // CLK[0] @ P2.18       (SDRAM System Clock)
   LPC_IOCON->P2_20 |= 1;              // CS @ P2.20           (SDRAM Chip Select)
   LPC_IOCON->P2_24 |= 1;              // CKE @ P2.24          (SDRAM Clock Enable)
   LPC_IOCON->P2_28 |= 1;              // DQML @ P2.28         (SDRAM Data Input/Output Mask)
   LPC_IOCON->P2_29 |= 1;              // DQMH @ P2.29         (SDRAM Data Input/Output Mask)

   // configure for 16-bit external data bus
   // ------------------------------------------------------------------------------------------
   LPC_IOCON->P3_0 |= 1;               // DQ0 @ P3.0
   LPC_IOCON->P3_1 |= 1;               // DQ1 @ P3.1
   LPC_IOCON->P3_2 |= 1;               // DQ2 @ P3.2
   LPC_IOCON->P3_3 |= 1;               // DQ3 @ P3.3
   LPC_IOCON->P3_4 |= 1;               // DQ4 @ P3.4
   LPC_IOCON->P3_5 |= 1;               // DQ5 @ P3.5
   LPC_IOCON->P3_6 |= 1;               // DQ6 @ P3.6
   LPC_IOCON->P3_7 |= 1;               // DQ7 @ P3.7
   LPC_IOCON->P3_8 |= 1;               // DQ8 @ P3.8
   LPC_IOCON->P3_9 |= 1;               // DQ9 @ P3.9
   LPC_IOCON->P3_10 |= 1;              // DQ10 @ P3.10
   LPC_IOCON->P3_11 |= 1;              // DQ11 @ P3.11
   LPC_IOCON->P3_12 |= 1;              // DQ12 @ P3.12
   LPC_IOCON->P3_13 |= 1;              // DQ13 @ P3.13
   LPC_IOCON->P3_14 |= 1;              // DQ14 @ P3.14
   LPC_IOCON->P3_15 |= 1;              // DQ15 @ P3.15

   // configure for 16-bit external address bus
   // ------------------------------------------------------------------------------------------
   LPC_IOCON->P4_0 |= 1;               //A0 @ P4.0
   LPC_IOCON->P4_1 |= 1;               //A1 @ P4.1
   LPC_IOCON->P4_2 |= 1;               //A2 @ P4.2
   LPC_IOCON->P4_3 |= 1;               //A3 @ P4.3
   LPC_IOCON->P4_4 |= 1;               //A4 @ P4.4
   LPC_IOCON->P4_5 |= 1;               //A5 @ P4.5
   LPC_IOCON->P4_6 |= 1;               //A6 @ P4.6
   LPC_IOCON->P4_7 |= 1;               //A7 @ P4.7
   LPC_IOCON->P4_8 |= 1;               //A8 @ P4.8
   LPC_IOCON->P4_9 |= 1;               //A9 @ P4.9
   LPC_IOCON->P4_10 |= 1;              //A10 @ P4.10
   LPC_IOCON->P4_11 |= 1;              //A11 @ P4.11
   LPC_IOCON->P4_13 |= 1;              //A12 @ P4.13
   LPC_IOCON->P4_14 |= 1;              //A13 @ P4.14

   // Configure write enable for SDRAM...
   // ------------------------------------------------------------------------------------------
   LPC_IOCON->P4_25 |= 1;              // WEN @ P4.25

   // Enable EMC Reset, (both EMC resets are asserted when any type of reset occurs)
   // [Ref: LPC178x_7x_UM_1.4 page 32 ]
   // ------------------------------------------------------------------------------------------
   LPC_SC->SCS &= ~(1<<1);    // clears bit 1, which is the power on default state anyway...

   // Enable EMC power/clock control bit...
   // ------------------------------------------------------------------------------------------
   LPC_SC->PCONP   |= (1<<11);

   CAS_Latency = 3;//was 2
   RAS_Latency = 3;//was 2
   DelayConstant = 0x00001F1F;

   // [Ref: LPC178x_7x_UM_1.4 page 183 ]
   // ----------------------------------------------------------------------------------------------------------------------
   // Bit      Symbol         Description                                                                              Reset
   //                                                                                                                  Value
   // ----------------------------------------------------------------------------------------------------------------------
   // 4:0      CMDDLY         Programmable delay value for EMC outputs in command delayed mode. See                    0x10
   //                         Section 10.12.6. The delay amount is roughly (CMDDLY+1) * 250 picoseconds.
   // 7:5      -              Reserved. Read value is undefined, only zero should be written.                          NA
   // 12:8     FBCLKDLY       Programmable delay value for the feedback clock that controls input data sampling. See   0x02
   //                         Section 10.5.3. The delay amount is roughly (FBCLKDLY+1) * 250 picoseconds.
   // 15:13    -              Reserved. Read value is undefined, only zero should be written.                          NA
   // 20:16    CLKOUT0DLY     Programmable delay value for the CLKOUT0 output. This would typically be used in clock   0x00
   //                         delayed mode. See Section 10.12.6 The delay amount is roughly (CLKOUT0DLY+1) *
   //                         250 picoseconds.
   // 23:21    -              Reserved. Read value is undefined, only zero should be written.                          NA
   // 28:24    CLKOUT1DLY     Programmable delay value for the CLKOUT1 output. This would typically be used in clock   0x00
   //                         delayed mode. See Section 10.12.6 The delay amount is roughly (CLKOUT1DLY+1) *
   //                         250 picoseconds.
   // 31:29    -              Reserved. Read value is undefined, only zero should be written.                          NA
   LPC_SC->EMCDLYCTL   = DelayConstant;

   // Reset the EMC and put configuration back to power up reset (little-endian mode, 1:1 clock)
   // [Ref: LPC178x_7x_UM_0.01 page 156]
   // ------------------------------------------------------------------------------------------
   LPC_EMC->Control = 0x00000001;   // Disable Address mirror, leave EMC enabled...
   LPC_EMC->Config  = 0x00000000;

   // define connection configuration [32 bit external bus address mapping]
   // ------------------------------------------------------------------------------------------
//   #ifdef ROW_BANK_COLUMN
//      LPC_EMC->DynamicConfig0 = 0x00000880;  // row, bank, column, 32-bit
//   #else
//      LPC_EMC->DynamicConfig0 = 0x00001880;  // bank, row, column, 32-bit
//   #endif
   LPC_EMC->DynamicConfig0 = 0x00000480;

   // define delays for RAS and CAS...
   // bits 1:0-RAS latency, bits 9:8-CAS latency (in CCLK cycle counts)
   // ------------------------------------------------------------------------------------------
   LPC_EMC->DynamicRasCas0 = RAS_Latency + (CAS_Latency<<8);    // reset value is 3 and 3

   // configure the dynamic memory read strategy.  Note: This register is used for all four
   // dynamic memory chip selects. Therefore the worst case value for all of the chip selects
   // must be programmed.
   // [Ref: LPC178x_7x_UM_0.01 page 169 Table 117]
   // ------------------------------------------------------------------------------------------
   LPC_EMC->DynamicReadConfig = 0x00000001;  // Command delayed strategy, using EMCCLKDELAY
                                             // (command delayed, clock out not delayed)

   // *** Taken from the MT48LC2M32B2 data sheet, page 47 ***
   // configure timing, from Table 18:
   // -6 (6ns part) timings (see marking on actual chip: MT48LC2M32B2-6)
   // NOTE: all timing values for LPC1788 are in units of CLK counts
   // ------------------------------------------------------------------------------------------
   LPC_EMC->DynamicRP   = NS_2_CLKS(15);     // tRP: precharge command period             (20ns)
   LPC_EMC->DynamicRAS  = NS_2_CLKS(37);     // tRAS: active to precharge command period  (44ns)
   LPC_EMC->DynamicSREX = NS_2_CLKS(67);     // tXSR: self-refresh exit time              (75ns)
   LPC_EMC->DynamicAPR  = NS_2_CLKS(15);     // tAPR: last-data-out to active command time
                                             // note: no tAPR value, using tRCD value     (20ns)
   LPC_EMC->DynamicDAL  = CAS_Latency+2;     // tDAL: data-in to active command time
                                             //       for CL=3, tDAL = 5 tCK
                                             //       for CL=2, tDAL = 4 tCK
                                             //       for CL=1, tDAL = 3 tCK
   LPC_EMC->DynamicWR   = (NS_2_CLKS(7)+1);// tWR: write recovery time is (12ns) UNLESS we're
                                             //      using AUTO PRECHARGE, then it's   (tCK+6ns)
   LPC_EMC->DynamicRC   = NS_2_CLKS(60);     // tRC: ACTIVE-to-ACTIVE command period      (66ns)
   LPC_EMC->DynamicRFC  = NS_2_CLKS(66);     // tRFC: AUTO REFRESH period                 (66ns)
   LPC_EMC->DynamicXSR  = NS_2_CLKS(67);     // tXSR: Exit self refresh to ACTIVE command (75ns)
   LPC_EMC->DynamicRRD  = NS_2_CLKS(14);     // tRRD: active bank A to active bank B command
                                             //       latency                             (15ns)
   LPC_EMC->DynamicMRD  = 2;                 // tMRD: LOAD MODE REGISTER command to ACTIVE or
                                             // REFRESH command time                      (2tCK)

   // *** Taken from the MT48LC2M32B2 data sheet, page 10 ***
   // ------------------------------------------------------------------------------------------
   // SDRAMs must be powered up and initialized in a predefined manner. Operational
   // procedures other than those specified may result in undefined operation. After power is
   // applied to VDD and VDDQ (simultaneously) and the clock is stable (stable clock is
   // defined as a signal cycling within timing constraints specified for the clock pin), the
   // SDRAM requires a 100us delay prior to issuing any command other than a COMMAND
   // INHIBIT or NOP. Starting at some point during this 100us period, and continuing at least
   // through the end of this period, COMMAND INHIBIT or NOP commands must be
   // applied.
   // When the 100us delay has been satisfied with at least one COMMAND INHIBIT or NOP
   // command having been applied, a PRECHARGE command should be applied. All banks
   // must then be precharged, thereby placing the device in the all banks idle state.
   // ------------------------------------------------------------------------------------------

   // *** Taken from the MT48LC2M32B2 data sheet, page 17 ***
   // The NO OPERATION (NOP) command is used to perform an NOP to an SDRAM that is
   // selected (CS# is LOW). This prevents unwanted commands from being registered during
   // idle or wait states. Operations already in progress are not affected.
   // ------------------------------------------------------------------------------------------
   // Send command: NOP
   // (CLKOUT runs continuously; All clock enables are driven HIGH continuously)
   // ------------------------------------------------------------------------------------------
   LPC_EMC->DynamicControl = 0x00000183;

   // wait > 100us
   // ------------------------------------------------------------------------------------------
   for( Temp=NS_2_CLKS(100000); Temp; Temp-- );

   // *** Taken from the MT48LC2M32B2 data sheet, page 18 ***
   // The PRECHARGE command is used to deactivate the open row in a particular bank or
   // the open row in all banks. The bank(s) will be available for a subsequent row access a
   // specified time (tRP) after the PRECHARGE command is issued. Input A10 determines
   // whether one or all banks are to be precharged, and in the case where only one bank is to
   // be precharged, inputs BA0 and BA1 select the bank. Otherwise BA0 and BA1 are treated
   // as â€œDonâ€™t Care.â€ After a bank has been precharged, it is in the idle state and must be
   // activated prior to any READ or WRITE commands being issued to that bank.
   // ------------------------------------------------------------------------------------------
   // Send command: PRECHARGE-ALL and set the shortest possible refresh period
   // ------------------------------------------------------------------------------------------
   LPC_EMC->DynamicControl = 0x00000103;
   LPC_EMC->DynamicRefresh = 0x00000002;  // 2 x 16 = 32 CCLKs between SDRAM refresh cycles

   // wait >128 clks
   // ------------------------------------------------------------------------------------------
   for( Temp=128; Temp; Temp-- );

   // *** Taken from the MT48LC2M32B2 data sheet, page 18 ***
   // Regardless of device width, the
   // 64Mb SDRAM requires 4,096 AUTO REFRESH cycles every 64ms (commercial and industrial)
   // or 16ms (automotive). Providing a distributed AUTO REFRESH command every
   // 15.625us (commercial and industrial) or 3.906us (automotive) will meet the refresh
   // requirement and ensure that each row is refreshed. Alternatively, 4,096 AUTO REFRESH
   // commands can be issued in a burst at the minimum cycle rate (tRFC), once every 64ms
   // (commercial and industrial) or 16ms (automotive).
   // so... Refresh period (4,096 rows) tREF â€“ 64ms
   // which gives a time of 15.625us per row, or 15625ns
   // ------------------------------------------------------------------------------------------
   // Set correct refresh period
   // ------------------------------------------------------------------------------------------
   LPC_EMC->DynamicRefresh = NS_2_CLKS(15625)>>4;     // Refresh units are x16
   //LPC_EMC->DynamicRefresh = 14;
   //LPC_EMC->DynamicRefresh = NS_2_CLKS(31250)>>4;     // Refresh units are x16

   // wait >128 clks
   // ------------------------------------------------------------------------------------------
   //for( Temp=128; Temp; Temp-- );

   // ------------------------------------------------------------------------------------------
   // Send command: MODE
   // ------------------------------------------------------------------------------------------
   LPC_EMC->DynamicControl = 0x00000083;

   // Set mode register in SDRAM
   // A0â€“A10 define the op-code written to the mode register
   // Bank select is A13-A14, address is A0-A12, data I/O mask is DQM0-DQM3...
   // ------------------------------------------------------------------------------------------
   // Mode register table for Micron's MT48LCxx (MT48LC2M32B2P ON FDI BOARD)
   //    bit 9:   Write Burst Mode: Programmed burst length(0), Single Location Access(1)
   //    bit 8~7: Operating Mode: Standard Operation(0) is the only thing defined
   //    bit 6~4: CAS latency: 001(1), 010(2), 011(3)
   //    bit 3:   Type of Burst: Sequential(0) or Interleaved(1)
   //    bit 2~0: Burst length: 000(1), 001(2), 010(4), 011(8), 111(Full Page)
   //

   //#ifdef ROW_BANK_COLUMN  // shift includes bank
   	  //Temp = *((volatile uint32_t *)(SDRAM_BASE_ADDR|((0x04+(CAS_Latency<<4))<<15)));
   	   Temp = *((volatile uint32_t *)(SDRAM_BASE_ADDR|(0x33 << 12)));
   //#else                   // shift excludes bank
      //Temp = *((volatile uint32_t *)(SDRAM_BASE_ADDR|((0x02+(CAS_Latency<<4))<<10)));
   //#endif

   // wait >128 clks
   // ------------------------------------------------------------------------------------------
   for( Temp=128; Temp; Temp-- );

   // ------------------------------------------------------------------------------------------
   //Send command: NORMAL
   // ------------------------------------------------------------------------------------------
   LPC_EMC->DynamicControl = 0x00000000;

   //Enable buffer
   LPC_EMC->DynamicConfig0 |= 0x00080000;

   return(1);
   }

void MemTest( void )
{
  unsigned int i;

  printf("32 bit Access\n\r");
  // 32 bits access
  for (i = 0x00000000; i < 0x01000000; i+=sizeof(volatile unsigned int))
  {
    *(volatile unsigned int *)((volatile unsigned int )SDRAM_BASE_ADDR+i) = i;
    if (*(volatile unsigned int *)((volatile unsigned int )SDRAM_BASE_ADDR+i) != i)
    {
    	PrintDataAtAddress((volatile unsigned int)(SDRAM_BASE_ADDR+i));
    }
  }

  for (i = 0x00000000; i < 0x01000000; i+=sizeof(volatile unsigned int ))
  {
    if (*(volatile unsigned int *)((volatile unsigned int )SDRAM_BASE_ADDR+i) != i)
    {
    	PrintDataAtAddress((volatile unsigned int)(SDRAM_BASE_ADDR+i));
    }
  }
  printf("16 bit Access\n\r");
  // 16 bits access
  for (i = 0; i < 0x10000; i+=sizeof(unsigned short))
  {
    *(unsigned short*)((unsigned int)SDRAM_BASE_ADDR+i) = i;
  }

  for (i = 0; i < 0x10000; i+=sizeof(unsigned short))
  {
    if (*(unsigned short*)((unsigned int)SDRAM_BASE_ADDR+i) != i)
    {
    	PrintDataAtAddress((unsigned int)(SDRAM_BASE_ADDR+i));
    }
  }

   //8 bits access
  printf("8 bit Access\n\r");
  for ( i = 0; i < 0x100; i+=sizeof(unsigned char))
  {
    *(unsigned char*)((unsigned int)SDRAM_BASE_ADDR+i) = i;
  }

  for (i = 0; i < 0x100; i+=sizeof(unsigned char))
  {
    if (*(unsigned char*)((unsigned int)SDRAM_BASE_ADDR+i) != i)
    {
    	PrintDataAtAddress((unsigned int)(SDRAM_BASE_ADDR+i));
    }
  }
}

void PrintDataAtAddress(volatile unsigned int Address){
	itoh1(Address);
	printf(" - ");
	itoh1(*(volatile unsigned int*)((volatile unsigned int)Address));
	printf("\n\r");
}

void itoh1(unsigned int x){
	char HexData[9];
	int counter = 7;
	int i;
	for(i = 0; i < 9; i ++){
		HexData[i] = '0';
	}
	char HexChars[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	while(x != 0){
		//printf("HexData = %c  ", HexData[counter]);
		HexData[counter] = HexChars[x%16];
		//printf("HexData = %c  ", HexData[counter]);
		counter --;
		x = x/16;
	}

	HexData[8] = 0x00;
	//printf("%c\n\r", HexData);
	uart0Tx(HexData,9);

}
