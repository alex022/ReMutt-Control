/*****************************************************************************
 *   ex_sdram.c:  External SDRAM memory module file for NXP LPC24xx Family
 *   Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.01.13  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC177x_8x.h"
#include "type.h"
#include "irq.h"
#include "macros.h"
#include "uart.h"
#include "sdram.h"
#include "clkpwr.h"

uint32_t EMC_SDRAM_REFRESH(uint32_t time)
{
    uint32_t emc_freq = CLKPWR_GetCLK(CLKPWR_CLKTYPE_EMC);
    return (((uint64_t)((uint64_t)time * emc_freq)/16000000ull)+1);
}

uint32_t EMC_NS2CLK(uint32_t time){
    uint32_t emc_freq = CLKPWR_GetCLK(CLKPWR_CLKTYPE_EMC);
    return (((uint64_t)time*emc_freq/1000000000ull));
}

/*********************************************************************
 * @brief        Power on EMC Block
 * @param[in]    None
 * @return       None
 **********************************************************************/
EMC_FUNC_CODE EMC_PwrOn(void)
{
    CLKPWR_SetCLKDiv(CLKPWR_CLKTYPE_EMC, 0); // Same clock as CPU

    // Power on
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCEMC, ENABLE);

    // Enable
    LPC_EMC->Control = EMC_Control_E;

    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Initialize external dynamic memory
 * @param[in]    pConfig    Configuration
 * @return         EMC_FUNC_OK/EMC_FUNC_INVALID_PARAM/EMC_FUNC_ERR
 **********************************************************************/
EMC_FUNC_CODE DynMem_Init(EMC_DYN_MEM_Config_Type* pConfig)
{
     EMC_FUNC_CODE ret = EMC_FUNC_OK;

     /* Pin Configuration
     * P2.16 - EMC_CAS
     * P2.17 - EMC_RAS
     * P2.18 - EMC_CLK[0]
     *
     * P2.20 - EMC_DYCS0
     *
     * P2.24 - EMC_CKE0
     *
     * P2.28 - EMC_DQM0
     * P2.29 - EMC_DQM1
     *
     * P3.0-P3.15 - EMC_D[0-15]
     * P4.0-P4.11 - EMC_A[0-11]
     * P4.13-P4.14 - EMC_A[12-13]
     *
     * P4.25 - EMC_WE
     */
     LPC_IOCON->P2_16 = 0;
     LPC_IOCON->P2_16 = SET_BIT(0);
     LPC_IOCON->P2_17 = 0;
     LPC_IOCON->P2_17 = SET_BIT(0);
     LPC_IOCON->P2_18 = 0;
     LPC_IOCON->P2_18 = SET_BIT(0);
     LPC_IOCON->P2_20 = 0;
     LPC_IOCON->P2_20 = SET_BIT(0);

     LPC_IOCON->P2_24 = 0;
     LPC_IOCON->P2_24 = SET_BIT(0);

     LPC_IOCON->P2_28 = 0;
     LPC_IOCON->P2_28 = SET_BIT(0);
     LPC_IOCON->P2_29 = 0;
     LPC_IOCON->P2_29 = SET_BIT(0);

     LPC_IOCON->P3_0 = 0;
     LPC_IOCON->P3_0 = SET_BIT(0);
     LPC_IOCON->P3_1 = 0;
     LPC_IOCON->P3_1 = SET_BIT(0);
     LPC_IOCON->P3_2 = 0;
     LPC_IOCON->P3_2 = SET_BIT(0);
     LPC_IOCON->P3_3 = 0;
     LPC_IOCON->P3_3 = SET_BIT(0);
     LPC_IOCON->P3_4 = 0;
     LPC_IOCON->P3_4 = SET_BIT(0);
     LPC_IOCON->P3_5 = 0;
     LPC_IOCON->P3_5 = SET_BIT(0);
     LPC_IOCON->P3_6 = 0;
     LPC_IOCON->P3_6 = SET_BIT(0);
     LPC_IOCON->P3_7 = 0;
     LPC_IOCON->P3_7 = SET_BIT(0);
     LPC_IOCON->P3_8 = 0;
     LPC_IOCON->P3_8 = SET_BIT(0);
     LPC_IOCON->P3_9 = 0;
     LPC_IOCON->P3_9 = SET_BIT(0);
     LPC_IOCON->P3_10 = 0;
     LPC_IOCON->P3_10 = SET_BIT(0);
     LPC_IOCON->P3_11 = 0;
     LPC_IOCON->P3_11 = SET_BIT(0);
     LPC_IOCON->P3_12 = 0;
     LPC_IOCON->P3_12 = SET_BIT(0);
     LPC_IOCON->P3_13 = 0;
     LPC_IOCON->P3_13 = SET_BIT(0);
     LPC_IOCON->P3_14 = 0;
     LPC_IOCON->P3_14 = SET_BIT(0);
     LPC_IOCON->P3_15 = 0;
     LPC_IOCON->P3_15 = SET_BIT(0);

     LPC_IOCON->P4_0 = 0;
     LPC_IOCON->P4_0 = SET_BIT(0);
     LPC_IOCON->P4_1 = 0;
     LPC_IOCON->P4_1 = SET_BIT(0);
     LPC_IOCON->P4_2 = 0;
     LPC_IOCON->P4_2 = SET_BIT(0);
     LPC_IOCON->P4_3 = 0;
     LPC_IOCON->P4_3 = SET_BIT(0);
     LPC_IOCON->P4_4 = 0;
     LPC_IOCON->P4_4 = SET_BIT(0);
     LPC_IOCON->P4_5 = 0;
     LPC_IOCON->P4_5 = SET_BIT(0);
     LPC_IOCON->P4_6 = 0;
     LPC_IOCON->P4_6 = SET_BIT(0);
     LPC_IOCON->P4_7 = 0;
     LPC_IOCON->P4_7 = SET_BIT(0);
     LPC_IOCON->P4_8 = 0;
     LPC_IOCON->P4_8 = SET_BIT(0);
     LPC_IOCON->P4_9 = 0;
     LPC_IOCON->P4_9 = SET_BIT(0);
     LPC_IOCON->P4_10 = 0;
     LPC_IOCON->P4_10 = SET_BIT(0);
     LPC_IOCON->P4_11 = 0;
     LPC_IOCON->P4_11 = SET_BIT(0);
     LPC_IOCON->P4_13 = 0;
     LPC_IOCON->P4_13 = SET_BIT(0);
     LPC_IOCON->P4_14 = 0;
     LPC_IOCON->P4_14 = SET_BIT(0);

     LPC_IOCON->P4_25 = 0;
     LPC_IOCON->P4_25 = SET_BIT(0);

     // Power On
	 ret |= EMC_PwrOn();

	 /*Init SDRAM controller*/
	 LPC_SC->EMCDLYCTL |= (8<<0);
	 /*Set data read delay*/
	 LPC_SC->EMCDLYCTL |=(8<<8);
	 LPC_SC->EMCDLYCTL |= (0x08 <<16);

	 ret |= EMC_ConfigEndianMode(EMC_Config_Little_Endian_Mode);

     /* Dynamic memory setting */
	 ret |= EMC_DynCtrlSelfRefresh(EMC_DYNAMIC_CTRL_SR_NORMALMODE);
	 ret |= EMC_DynCtrlPowerDownMode(EMC_DYNAMIC_CTRL_DP_NORMAL);
	 ret |= EMC_DynCtrlClockEnable(EMC_DYNAMIC_CTRL_CE_ALLCLK_HI);
	 ret |= EMC_DynCtrlMMC(EMC_DYNAMIC_CTRL_MMC_CLKOUT_ENABLED);
	 ret |= EMC_DynCtrlClockControl(EMC_DYNAMIC_CTRL_CE_CLKOUT_CONT);

	 /* Timing */
	 ret |= EMC_SetDynMemoryParameter(EMC_DYN_MEM_REFRESH_TIMER, pConfig->RefreshTime);
	 ret |= EMC_SetDynMemoryParameter(EMC_DYN_MEM_READ_CONFIG, pConfig->ReadConfig);
	 ret |= EMC_SetDynMemoryParameter(EMC_DYN_MEM_TRP, pConfig->PrechargeCmdPeriod);
	 ret |= EMC_SetDynMemoryParameter(EMC_DYN_MEM_TRAS, pConfig->Active2PreChargeTime);
	 ret |= EMC_SetDynMemoryParameter(EMC_DYN_MEM_TSREX, pConfig->SeftRefreshExitTime);
	 ret |= EMC_SetDynMemoryParameter(EMC_DYN_MEM_TAPR, pConfig->DataOut2ActiveTime);
	 ret |= EMC_SetDynMemoryParameter(EMC_DYN_MEM_TDAL, pConfig->DataIn2ActiveTime);
	 ret |= EMC_SetDynMemoryParameter(EMC_DYN_MEM_TWR, pConfig->WriteRecoveryTime);
	 ret |= EMC_SetDynMemoryParameter(EMC_DYN_MEM_TRC, pConfig->Active2ActivePeriod);
	 ret |= EMC_SetDynMemoryParameter(EMC_DYN_MEM_TRFC, pConfig->AutoRefrehPeriod);
	 ret |= EMC_SetDynMemoryParameter(EMC_DYN_MEM_TXSR, pConfig->ExitSelfRefreshTime);
	 ret |= EMC_SetDynMemoryParameter(EMC_DYN_MEM_TRRD, pConfig->ActiveBankLatency);
	 ret |= EMC_SetDynMemoryParameter(EMC_DYN_MEM_TMRD, pConfig->LoadModeReg2Active);
	 ret |= EMC_DynMemRAS(pConfig->CSn,pConfig->RASLatency);
	 ret |= EMC_DynMemCAS(pConfig->CSn,pConfig->CASLatency);

	 ret |= EMC_DynMemConfigMD(pConfig->CSn,EMC_DYNAMIC_CFG_MEMDEV_SDRAM);
	 ret |= EMC_DynMemConfigAM(pConfig->CSn, pConfig->AddrBusWidth, pConfig->AddrMap, pConfig->DataWidth, pConfig->ChipSize);

	 return ret;
}

/*********************************************************************//**
 * @brief         EMC initialize (power on block, config EMC pins).
 * @param[in]    None
 * @return         None
 **********************************************************************/
EMC_FUNC_CODE EMC_Init(void)
{
    CLKPWR_SetCLKDiv(CLKPWR_CLKTYPE_EMC, 0); // Same clock as CPU

    LPC_SC->PCONP       |= 0x00000800;
    LPC_SC->EMCDLYCTL   = 0x00001010;
    LPC_EMC->Control     = 0x00000001;
    LPC_EMC->Config      = 0x00000000;

    /* Pin Configuration
    * P2.16 - EMC_CAS
    * P2.17 - EMC_RAS
    * P2.18 - EMC_CLK[0]
    *
    * P2.20 - EMC_DYCS0
    *
    * P2.24 - EMC_CKE0
    *
    * P2.28 - EMC_DQM0
    * P2.29 - EMC_DQM1
    *
    * P3.0-P3.15 - EMC_D[0-15]
    * P4.0-P4.11 - EMC_A[0-11]
    * P4.13-P4.14 - EMC_A[12-13]
    *
    * P4.25 - EMC_WE
    */
    LPC_IOCON->P2_16 = 0;
    LPC_IOCON->P2_16 = SET_BIT(0);
    LPC_IOCON->P2_17 = 0;
    LPC_IOCON->P2_17 = SET_BIT(0);
    LPC_IOCON->P2_18 = 0;
    LPC_IOCON->P2_18 = SET_BIT(0);
    LPC_IOCON->P2_20 = 0;
    LPC_IOCON->P2_20 = SET_BIT(0);

    LPC_IOCON->P2_24 = 0;
    LPC_IOCON->P2_24 = SET_BIT(0);

    LPC_IOCON->P2_28 = 0;
    LPC_IOCON->P2_28 = SET_BIT(0);
    LPC_IOCON->P2_29 = 0;
    LPC_IOCON->P2_29 = SET_BIT(0);

    LPC_IOCON->P3_0 = 0;
    LPC_IOCON->P3_0 = SET_BIT(0);
    LPC_IOCON->P3_1 = 0;
    LPC_IOCON->P3_1 = SET_BIT(0);
    LPC_IOCON->P3_2 = 0;
    LPC_IOCON->P3_2 = SET_BIT(0);
    LPC_IOCON->P3_3 = 0;
    LPC_IOCON->P3_3 = SET_BIT(0);
    LPC_IOCON->P3_4 = 0;
    LPC_IOCON->P3_4 = SET_BIT(0);
    LPC_IOCON->P3_5 = 0;
    LPC_IOCON->P3_5 = SET_BIT(0);
    LPC_IOCON->P3_6 = 0;
    LPC_IOCON->P3_6 = SET_BIT(0);
    LPC_IOCON->P3_7 = 0;
    LPC_IOCON->P3_7 = SET_BIT(0);
    LPC_IOCON->P3_8 = 0;
    LPC_IOCON->P3_8 = SET_BIT(0);
    LPC_IOCON->P3_9 = 0;
    LPC_IOCON->P3_9 = SET_BIT(0);
    LPC_IOCON->P3_10 = 0;
    LPC_IOCON->P3_10 = SET_BIT(0);
    LPC_IOCON->P3_11 = 0;
    LPC_IOCON->P3_11 = SET_BIT(0);
    LPC_IOCON->P3_12 = 0;
    LPC_IOCON->P3_12 = SET_BIT(0);
    LPC_IOCON->P3_13 = 0;
    LPC_IOCON->P3_13 = SET_BIT(0);
    LPC_IOCON->P3_14 = 0;
    LPC_IOCON->P3_14 = SET_BIT(0);
    LPC_IOCON->P3_15 = 0;
    LPC_IOCON->P3_15 = SET_BIT(0);

    LPC_IOCON->P4_0 = 0;
    LPC_IOCON->P4_0 = SET_BIT(0);
    LPC_IOCON->P4_1 = 0;
    LPC_IOCON->P4_1 = SET_BIT(0);
    LPC_IOCON->P4_2 = 0;
    LPC_IOCON->P4_2 = SET_BIT(0);
    LPC_IOCON->P4_3 = 0;
    LPC_IOCON->P4_3 = SET_BIT(0);
    LPC_IOCON->P4_4 = 0;
    LPC_IOCON->P4_4 = SET_BIT(0);
    LPC_IOCON->P4_5 = 0;
    LPC_IOCON->P4_5 = SET_BIT(0);
    LPC_IOCON->P4_6 = 0;
    LPC_IOCON->P4_6 = SET_BIT(0);
    LPC_IOCON->P4_7 = 0;
    LPC_IOCON->P4_7 = SET_BIT(0);
    LPC_IOCON->P4_8 = 0;
    LPC_IOCON->P4_8 = SET_BIT(0);
    LPC_IOCON->P4_9 = 0;
    LPC_IOCON->P4_9 = SET_BIT(0);
    LPC_IOCON->P4_10 = 0;
    LPC_IOCON->P4_10 = SET_BIT(0);
    LPC_IOCON->P4_11 = 0;
    LPC_IOCON->P4_11 = SET_BIT(0);
    LPC_IOCON->P4_13 = 0;
    LPC_IOCON->P4_13 = SET_BIT(0);
    LPC_IOCON->P4_14 = 0;
    LPC_IOCON->P4_14 = SET_BIT(0);

    LPC_IOCON->P4_25 = 0;
    LPC_IOCON->P4_25 = SET_BIT(0);

    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Configure Little Endian/Big Endian mode for EMC
 *
 * @param[in]    endia_mode    Endian mode, should be:
 *
 *                 - EMC_Config_Little_Endian_Mode:    Little-endian mode
 *
 *                 - EMC_Config_Big_Endian_Mode    :    Big-endian mode
 *
 * @return         EMC_FUNC_OK
 **********************************************************************/
EMC_FUNC_CODE  EMC_ConfigEndianMode(uint32_t endian_mode)
{
    LPC_EMC->Config &= ~(EMC_Config_Endian_Mask);
    LPC_EMC->Config |= (endian_mode&EMC_Config_Endian_Mask);
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Switch the Self-refresh mode between normal and self-refresh mode
 *
 * @param[in]    self_refresh_mode    self refresh mode, should be:
 *
 *                - EMC_DYNAMIC_CTRL_SR_NORMALMODE: Normal mode
 *
 *                - EMC_DYNAMIC_CTRL_SR_SELFREFRESH: Enter self-refresh mode
 *
 * @return         EMC_FUNC_OK
 **********************************************************************/
EMC_FUNC_CODE EMC_DynCtrlSelfRefresh(uint32_t self_refresh_mode)
{
    LPC_EMC->DynamicControl &= ~EMC_DYNAMIC_CTRL_SELFREFRESH_REQ_BMASK;
    LPC_EMC->DynamicControl =self_refresh_mode & EMC_DYNAMIC_CTRL_SELFREFRESH_REQ_BMASK;
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Switch between Normal operation and deep sleep power mode
 *
 * @param[in]    Power_command    Low-power SDRAM deep-sleep mode, should be:
 *
 *                - EMC_DYNAMIC_CTRL_DP_NORMAL: Normal operation
 *
 *                - EMC_DYNAMIC_CTRL_DP_DEEPSLEEP: Enter deep-sleep mode
 *
 * @return         EMC_FUNC_OK
 **********************************************************************/
EMC_FUNC_CODE EMC_DynCtrlPowerDownMode(uint32_t Power_command)
{
    LPC_EMC->DynamicControl &= ~EMC_DYNAMIC_CTRL_SDRAM_PWRMODE_BMASK;
    LPC_EMC->DynamicControl |= Power_command & EMC_DYNAMIC_CTRL_SDRAM_PWRMODE_BMASK;
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Set the dsvalue for dynamic clock enable bit
 *
 * @param[in]    clock_enable    clock enable mode, should be:
 *
 *                - EMC_DYNAMIC_CTRL_CE_SAVEPWR: Clock enable of idle devices
 *                are deasserted to save power
 *
 *                - EMC_DYNAMIC_CTRL_CE_ALLCLK_HI: All clock enables are driven
 *                HIGH continuously
 *
 * @return         EMC_FUNC_CODE
 **********************************************************************/
EMC_FUNC_CODE EMC_DynCtrlClockEnable(uint32_t clock_enable)
{
    LPC_EMC->DynamicControl &= ~(EMC_DYNAMIC_CTRL_MEMCLK_EN_BMASK);
    LPC_EMC->DynamicControl |= clock_enable & EMC_DYNAMIC_CTRL_MEMCLK_EN_BMASK;
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Enable/disable CLKOUT
 *
 * @param[in]    MMC_val    Memory clock control mode, should be:
 *
 *                     - EMC_DYNAMIC_CTRL_MMC_CLKOUT_ENABLED: CLKOUT enabled
 *
 *                     - EMC_DYNAMIC_CTRL_MMC_CLKOUT_DISABLED: CLKOUT disabled
 *
 * @return         EMC_FUNC_OK
 **********************************************************************/
EMC_FUNC_CODE EMC_DynCtrlMMC(uint32_t MMC_val)
{
    LPC_EMC->DynamicControl &= ~EMC_DYNAMIC_CTRL_MMC_CLKOUTCTRL_BMASK;
    LPC_EMC->DynamicControl |=MMC_val & EMC_DYNAMIC_CTRL_MMC_CLKOUTCTRL_BMASK;
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Set the value for dynamic memory clock control: stops or
 *                runs continuously
 *
 * @param[in]    clock_control    clock control mode, should be:
 *
 *                - EMC_DYNAMIC_CTRL_CS_CLKOUT_STOP: CLKOUT stops when all
 *                SDRAMs are idle and during self-refresh mode
 *
 *                - EMC_DYNAMIC_CTRL_CS_CLKOUT_CONT: CLKOUT runs continuously
 *
 * @return         EMC_FUNC_OK
 **********************************************************************/
EMC_FUNC_CODE EMC_DynCtrlClockControl(int32_t clock_control)
{
    LPC_EMC->DynamicControl  &= ~EMC_DYNAMIC_CTRL_CLKCTRL_BMASK;
    LPC_EMC->DynamicControl |= clock_control & EMC_DYNAMIC_CTRL_CLKCTRL_BMASK;
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Set the value of EMC dynamic memory registers
 *
 * @param[in]    par    EMC register that will set value, should be:
 *                    - EMC_DYN_MEM_REFRESH_TIMER: Dynamic Refresh register
 *                    - EMC_DYN_MEM_READ_CONFIG: Dynamic Read Config register
 *                    - EMC_DYN_MEM_TRP: Dynamic RP register
 *                    - EMC_DYN_MEM_TRAS: Dynamic RAS register
 *                    - EMC_DYN_MEM_TSREX: Dynamic SREX register
 *                    - EMC_DYN_MEM_TAPR: Dynamic APR register
 *                    - EMC_DYN_MEM_TDAL: Dynamic DAL register
 *                    - EMC_DYN_MEM_TWR: Dynamic WR register
 *                    - EMC_DYN_MEM_TRC: Dynamic RC register
 *                    - EMC_DYN_MEM_TRFC: Dynamic RFC register
 *                    - EMC_DYN_MEM_TXSR: Dynamic XSR register
 *                    - EMC_DYN_MEM_TRRD: Dynamic RRD register
 *                    - EMC_DYN_MEM_TMRD: Dynamic MRD register
 *
 * @return         EMC_FUNC_OK/EMC_FUNC_INVALID_PARAM
 **********************************************************************/
EMC_FUNC_CODE EMC_SetDynMemoryParameter(EMC_DYN_MEM_PAR par, uint32_t val)
{
    switch ( par)
    {
        case EMC_DYN_MEM_REFRESH_TIMER:
            LPC_EMC->DynamicRefresh = EMC_DynamicRefresh_REFRESH(val);
            break;

        case EMC_DYN_MEM_READ_CONFIG:
            LPC_EMC->DynamicReadConfig = EMC_DynamicReadConfig_RD(val);
            break;

        case EMC_DYN_MEM_TRP:
            LPC_EMC->DynamicRP = EMC_DynamictRP_tRP(val);
            break;

        case EMC_DYN_MEM_TRAS:
            LPC_EMC->DynamicRAS = EMC_DynamictRP_tRAS(val);
            break;

        case EMC_DYN_MEM_TSREX:
            LPC_EMC->DynamicSREX = EMC_DynamictRP_tSREX(val);
            break;

        case EMC_DYN_MEM_TAPR:
            LPC_EMC->DynamicAPR = EMC_DynamictAPR_tAPR(val);
            break;

        case EMC_DYN_MEM_TDAL:
            LPC_EMC->DynamicDAL =EMC_DynamictDAL_tDAL(val);
            break;

        case EMC_DYN_MEM_TWR:
            LPC_EMC->DynamicWR = EMC_DynamictWR_tWR(val);
            break;

        case EMC_DYN_MEM_TRC:
            LPC_EMC->DynamicRC = EMC_DynamictRC_tRC(val);
            break;

        case EMC_DYN_MEM_TRFC:
            LPC_EMC->DynamicRFC = EMC_DynamictRFC_tRFC(val);
            break;

        case EMC_DYN_MEM_TXSR:
            LPC_EMC->DynamicXSR = EMC_DynamictXSR_tXSR(val);
            break;

        case EMC_DYN_MEM_TRRD:
            LPC_EMC->DynamicRRD = EMC_DynamictRRD_tRRD(val);
            break;

        case EMC_DYN_MEM_TMRD:
            LPC_EMC->DynamicMRD = EMC_DynamictMRD_tMRD(val);
            break;
        default:
            return EMC_FUNC_INVALID_PARAM;
    }
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Set value for RAS latency
 *
 * @param[in]    index index number, should be from 0 to 3
 *
 * @param[in]     ras_val  RAS value should be in range: 0..3
 *
 * @return         EMC_FUNC_OK
 **********************************************************************/
EMC_FUNC_CODE EMC_DynMemRAS(uint32_t index , uint32_t ras_val)
{
    switch ( index)
    {
        case 0:
            LPC_EMC->DynamicRasCas0 &= ~EMC_DYNAMIC_RASCAS_RASCFG_BMASK;
            LPC_EMC->DynamicRasCas0 |=( ras_val << EMC_DYNAMIC_RASCAS_RASCFG_POS)
                                                &EMC_DYNAMIC_RASCAS_RASCFG_BMASK;
            break;

        case 1:
            LPC_EMC->DynamicRasCas1 &= ~EMC_DYNAMIC_RASCAS_RASCFG_BMASK;
            LPC_EMC->DynamicRasCas1 |= ( ras_val << EMC_DYNAMIC_RASCAS_RASCFG_POS)
                                            &EMC_DYNAMIC_RASCAS_RASCFG_BMASK;
            break;

        case 2:
            LPC_EMC->DynamicRasCas2 &= ~EMC_DYNAMIC_RASCAS_RASCFG_BMASK;
            LPC_EMC->DynamicRasCas2 |= ( ras_val << EMC_DYNAMIC_RASCAS_RASCFG_POS)
                                            &EMC_DYNAMIC_RASCAS_RASCFG_BMASK;
            break;

        case 3:
            LPC_EMC->DynamicRasCas3 &= ~EMC_DYNAMIC_RASCAS_RASCFG_BMASK;
            LPC_EMC->DynamicRasCas3 |= ( ras_val << EMC_DYNAMIC_RASCAS_RASCFG_POS)
                                            &EMC_DYNAMIC_RASCAS_RASCFG_BMASK;
            break;
	default:
	   return EMC_FUNC_INVALID_PARAM;
    }
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Set value for CAS latency
 *
 * @param[in]    index index number, should be from 0 to 3
 *
 * @param[in]     ras_val  CAS value should be in range: 0..3
 *
 * @return         EMC_FUNC_OK/EMC_FUNC_INVALID_PARAM
 **********************************************************************/
EMC_FUNC_CODE EMC_DynMemCAS(uint32_t index , uint32_t cas_val)
{
    switch ( index)
    {
        case 0:
            LPC_EMC->DynamicRasCas0 &= ~EMC_DYNAMIC_RASCAS_CASCFG_BMASK;
            LPC_EMC->DynamicRasCas0 |= (cas_val<<EMC_DYNAMIC_RASCAS_CASCFG_POS)
                                            &EMC_DYNAMIC_RASCAS_CASCFG_BMASK;
            break;

        case 1:
            LPC_EMC->DynamicRasCas1 &= ~EMC_DYNAMIC_RASCAS_CASCFG_BMASK;
            LPC_EMC->DynamicRasCas1 |= (cas_val<<EMC_DYNAMIC_RASCAS_CASCFG_POS)
                                            &EMC_DYNAMIC_RASCAS_CASCFG_BMASK;
            break;

        case 2:
            LPC_EMC->DynamicRasCas2 &= ~EMC_DYNAMIC_RASCAS_CASCFG_BMASK;
            LPC_EMC->DynamicRasCas2 |= (cas_val<<EMC_DYNAMIC_RASCAS_CASCFG_POS)
                                            &EMC_DYNAMIC_RASCAS_CASCFG_BMASK;
            break;

        case 3:
            LPC_EMC->DynamicRasCas3 &= ~EMC_DYNAMIC_RASCAS_CASCFG_BMASK;
            LPC_EMC->DynamicRasCas3 |= (cas_val<<EMC_DYNAMIC_RASCAS_CASCFG_POS)
                                            &EMC_DYNAMIC_RASCAS_CASCFG_BMASK;
            break;
	default:
	   return EMC_FUNC_INVALID_PARAM;
    }
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Configure the memory device
 *
 * @param[in]    index index number, should be from 0 to 3
 *
 * @param[in]    mem_dev Memory device, should be:
 *
 *                    - EMC_DYNAMIC_CFG_MEMDEV_SDRAM: SDRAM
 *
 *                    - EMC_DYNAMIC_CFG_MEMDEV_LOWPWR_SDRAM: Low-power SDRAM
 *
 *
 * @return         EMC_FUNC_OK/EMC_FUNC_INVALID_PARAM
 **********************************************************************/
EMC_FUNC_CODE EMC_DynMemConfigMD(uint32_t index , uint32_t mem_dev)
{
    switch (index)
    {
        case 0:
            LPC_EMC->DynamicConfig0  &= ~EMC_DYNAMIC_CFG_MEMDEV_BMASK;
            LPC_EMC->DynamicConfig0 |=  mem_dev & EMC_DYNAMIC_CFG_MEMDEV_BMASK;
            break;

        case 1:
            LPC_EMC->DynamicConfig1  &= ~EMC_DYNAMIC_CFG_MEMDEV_BMASK;
            LPC_EMC->DynamicConfig1 |= mem_dev & EMC_DYNAMIC_CFG_MEMDEV_BMASK;
            break;

        case 2:
            LPC_EMC->DynamicConfig2  &= ~EMC_DYNAMIC_CFG_MEMDEV_BMASK;
            LPC_EMC->DynamicConfig2 |= mem_dev & EMC_DYNAMIC_CFG_MEMDEV_BMASK;
            break;

        case 3:
            LPC_EMC->DynamicConfig3  &= ~EMC_DYNAMIC_CFG_MEMDEV_BMASK;
            LPC_EMC->DynamicConfig3 |=  mem_dev & EMC_DYNAMIC_CFG_MEMDEV_BMASK;
            break;
        default:
            return EMC_FUNC_INVALID_PARAM;
    }
    return EMC_FUNC_OK;
}

/*********************************************************************//**
 * @brief         Map the address for the memory device
 *
 * @param[in]    index index number, should be from 0 to 3
 *
 * @param[in]     add_mapped address where the memory will be mapped
 *
 * @return         EMC_FUNC_OK
 **********************************************************************/
EMC_FUNC_CODE EMC_DynMemConfigAM(uint32_t index , uint8_t addr_bus_width, uint8_t addr_map,
uint8_t data_bus_width, uint16_t chip_size)
{
    const     int  chip_max_size   =  512;  // 512Mb
    uint8_t     data_bus_max_size = 0;
    uint32_t  add_mapped_p1 = 0x00, add_mapped_p2 = 0x00, add_mapped_p3 = 0x00;
    uint32_t tmp = 16, i = 0, j = 0;

     /* Get part 3 of address map */
    switch(addr_bus_width)
    {
       case 16:
          add_mapped_p3 = 0;
          data_bus_max_size = 16;
          break;
       case 32:
           add_mapped_p3 = 1;
          data_bus_max_size = 32;
          break;
       default:
            return EMC_FUNC_INVALID_PARAM;
     }

      /* Get part 2 of address map */
     add_mapped_p2 = EMC_DYNAMIC_CFG_ADD_MAP_P2(addr_map);


     /* Get part 1 of address map */
     if(chip_size == 16)
     {
          if(data_bus_width == 8)
               add_mapped_p1 = 0;
          else  if(data_bus_width == 16)
               add_mapped_p1 = 1;
          else
	     return EMC_FUNC_INVALID_PARAM;
     }
     else
     {
           while(1)
           {
                i++;
                tmp = 16*(0x01 << (i+1));
                if(tmp == chip_size)
                {
                    for(j = 0; (8<<j)<=data_bus_max_size;j++)
                    {
                         if((8<<j) == data_bus_width)
                             break;
                    }
                    if( (8<<j) > data_bus_max_size)
                         return EMC_FUNC_INVALID_PARAM;

                    add_mapped_p1 = (i<<2) + j;
                    break;
                }
                if(tmp >= chip_max_size)
                {
                    return EMC_FUNC_INVALID_PARAM;
                }
           }
       }

    switch ( index)
    {
        case 0:
            LPC_EMC->DynamicConfig0 &= ~EMC_DYNAMIC_CFG_ADD_MAP_P1_MASK;
            LPC_EMC->DynamicConfig0 |=  EMC_DYNAMIC_CFG_ADD_MAP_P1(add_mapped_p1);
            LPC_EMC->DynamicConfig0 &= ~EMC_DYNAMIC_CFG_ADD_MAP_P2_MASK;
            LPC_EMC->DynamicConfig0 |=  EMC_DYNAMIC_CFG_ADD_MAP_P2(add_mapped_p2);
            LPC_EMC->DynamicConfig0 &= ~EMC_DYNAMIC_CFG_ADD_MAP_P3_MASK;
            LPC_EMC->DynamicConfig0 |=  EMC_DYNAMIC_CFG_ADD_MAP_P3(add_mapped_p3);
            break;

        case 1:
            LPC_EMC->DynamicConfig1 &= ~EMC_DYNAMIC_CFG_ADD_MAP_P1_MASK;
            LPC_EMC->DynamicConfig1 |=  EMC_DYNAMIC_CFG_ADD_MAP_P1(add_mapped_p1);
            LPC_EMC->DynamicConfig1 &= ~EMC_DYNAMIC_CFG_ADD_MAP_P2_MASK;
            LPC_EMC->DynamicConfig1 |=  EMC_DYNAMIC_CFG_ADD_MAP_P2(add_mapped_p2);
            LPC_EMC->DynamicConfig1 &= ~EMC_DYNAMIC_CFG_ADD_MAP_P3_MASK;
            LPC_EMC->DynamicConfig1 |=  EMC_DYNAMIC_CFG_ADD_MAP_P3(add_mapped_p3);

            break;

        case 2:
            LPC_EMC->DynamicConfig2 &= ~EMC_DYNAMIC_CFG_ADD_MAP_P1_MASK;
            LPC_EMC->DynamicConfig2 |=  EMC_DYNAMIC_CFG_ADD_MAP_P1(add_mapped_p1);
            LPC_EMC->DynamicConfig2 &= ~EMC_DYNAMIC_CFG_ADD_MAP_P2_MASK;
            LPC_EMC->DynamicConfig2 |= EMC_DYNAMIC_CFG_ADD_MAP_P2( add_mapped_p2);
            LPC_EMC->DynamicConfig2 &= ~EMC_DYNAMIC_CFG_ADD_MAP_P3_MASK;
            LPC_EMC->DynamicConfig2 |=  EMC_DYNAMIC_CFG_ADD_MAP_P3(add_mapped_p3);

            break;

        case 3:
            LPC_EMC->DynamicConfig3 &= ~EMC_DYNAMIC_CFG_ADD_MAP_P1_MASK;
            LPC_EMC->DynamicConfig3 |=  EMC_DYNAMIC_CFG_ADD_MAP_P1(add_mapped_p1);
            LPC_EMC->DynamicConfig3 &= ~EMC_DYNAMIC_CFG_ADD_MAP_P2_MASK;
            LPC_EMC->DynamicConfig3 |=  EMC_DYNAMIC_CFG_ADD_MAP_P2(add_mapped_p2);
            LPC_EMC->DynamicConfig3 &= ~EMC_DYNAMIC_CFG_ADD_MAP_P3_MASK;
            LPC_EMC->DynamicConfig3 |=  EMC_DYNAMIC_CFG_ADD_MAP_P3(add_mapped_p3);

            break;
        default:
            return EMC_FUNC_INVALID_PARAM;
    }
    return EMC_FUNC_OK;
}

/*unsigned int  SDRAM_Test (void)
{
	uart0Puts("SDRAM TEST\n\r");
  unsigned int i;

  uart0Puts("32 bit Access\n\r");
  // 32 bits access
  for (i = 0; i < 0x1000000; i+=sizeof(unsigned int))
  {

    *(unsigned int *)((unsigned int )SDRAM_BASE_ADDR+i) = i;
    //PrintDataAtAddress((unsigned int)(SDRAM_BASE_ADDR+i));
    //PrintDataAtAddress((unsigned int)(SDRAM_BASE_ADDR+i));
  }

  for (i = 0; i < 0x1000000; i+=sizeof(unsigned int ))
  {
    if (*(unsigned int *)((unsigned int )SDRAM_BASE_ADDR+i) != i)
    {
    	uart0Puts("32 bit Access Failed\n\r");
    	PrintDataAtAddress((unsigned int)(SDRAM_BASE_ADDR+i));
      return(FALSE);
    }
  }
  uart0Puts("16 bit Access\n\r");
  // 16 bits access
  for (i = 0; i < 0x10000; i+=sizeof(unsigned short))
  {
	 // PrintDataAtAddress((unsigned int)(SDRAM_BASE_ADDR+i));
    *(unsigned short*)((unsigned int)SDRAM_BASE_ADDR+i) = i;
   //PrintDataAtAddress((unsigned int)(SDRAM_BASE_ADDR+i));
  }

  for (i = 0; i < 0x10000; i+=sizeof(unsigned short))
  {
    if (*(unsigned short*)((unsigned int)SDRAM_BASE_ADDR+i) != i)
    {
    	uart0Puts("16 bit Access Failed\n\r");
    	PrintDataAtAddress((unsigned int)(SDRAM_BASE_ADDR+i));
      return(FALSE);
    }
  }

  // 8 bits access
  uart0Puts("8 bit Access\n\r");
  for ( i = 0; i < 0x100; i+=sizeof(unsigned char))
  {
	 // PrintDataAtAddress((unsigned int)(SDRAM_BASE_ADDR+i));
    *(unsigned char*)((unsigned int)SDRAM_BASE_ADDR+i) = i;
   // PrintDataAtAddress((unsigned int)(SDRAM_BASE_ADDR+i));
  }

  for (i = 0; i < 0x100; i+=sizeof(unsigned char))
  {
    if (*(unsigned char*)((unsigned int)SDRAM_BASE_ADDR+i) != i)
    {
    	PrintDataAtAddress((unsigned int)(SDRAM_BASE_ADDR+i));
    	uart0Puts("8 bit Access\n\r");
      return(FALSE);
    }
  }
  return(TRUE);
}*/
