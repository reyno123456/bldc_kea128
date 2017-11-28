/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     MC33937_routines.c
*
* @author   B06050
*
* @version  1.0.3.0
*
* @date     Jul-3-2014
*
* @brief    MC33937A MOSFET pre-driver configuration function source file with
*           routines for pre-driver setup and diagnostics.
*
*******************************************************************************/
#include "MC33937_routines.h"

/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void MC33937_SendCommand(uint8_t u8Command)
*
* Description:  Sends pre-driver command via SPI0
*
* Param[in]:    u8Command       8-bit pre-driver command
*
* Notes:        SPI_0_Init (spi.c) function must be already called prior to this
*               function call.
*
*******************************************************************************/
void MC33937_SendCommand(uint8_t u8Command)
{
    MC33937_CS_LOW;     /* CS low */
    (void)SPI_0_SendByte(u8Command);
    MC33937_CS_HIGH;    /* CS high */
}

/*******************************************************************************
*
* Function: uint8_t MC33937_ReadData(uint8_t u8Command)
*
* Description:  Reads pre-driver data.
*
* Param[in]:    u8Command       8-bit pre-driver command
*
* Return:       uint8_t         Read data
*
* Notes:        SPI_0_Init (spi.c) function must be already called prior to this
*               function call.
*
*******************************************************************************/
uint8_t MC33937_ReadData(uint8_t u8Command)
{
    uint8_t u8RxData;   /* Variable to store received word */

    MC33937_CS_LOW;     /* CS low */
    u8RxData = SPI_0_SendByte(u8Command);
    MC33937_CS_HIGH;    /* CS high */

    return(u8RxData);
}

/*******************************************************************************
*
* Function: void MC33937_ClearFaults(void)
*
* Description:  Clears pre-driver faults.
*
* Notes:        SPI_0_Init (spi.c) function must be already called prior to this
*               function call.
*
*******************************************************************************/
void MC33937_ClearFaults(void)
{
    /* Disable MC33937A */
    MC33937_EN_CLEAR;

    MC33937_SendCommand(CLINT0_CMD);
    MC33937_SendCommand(CLINT1_CMD);
}

/*******************************************************************************
*
* Function: void MC33937_Enable(void)
*
* Description:  Enables the pre-driver (sets EN1 & EN2 pins high).
*
*******************************************************************************/
__attribute__((always_inline)) void MC33937_Enable(void)
{
    MC33937_EN_SET;
}

/*******************************************************************************
*
* Function: void MC33937_Disable(void)
*
* Description:  Disables the pre-driver (sets EN1 & EN2 pins low).
*
*******************************************************************************/
__attribute__((always_inline)) void MC33937_Disable(void)
{
    MC33937_EN_CLEAR;
}

/*******************************************************************************
*
* Function: void MC33937_Reset(void)
*
* Description:  Sets the pre-driver to reset (sets /RST pin low).
*
*******************************************************************************/
__attribute__((always_inline)) void MC33937_Reset(void)
{
    MC33937_RST_CLEAR;
}

/*******************************************************************************
*
* Function: MC33937_SR_T MC33937_ReadStatus(void)
*
* Description:  Reads pre-driver internal status.
*
* Return:       MC33937_SR_T        Pre-driver internal status.
*
*******************************************************************************/
MC33937_SR_T MC33937_ReadStatus(void)
{
    MC33937_SR_T devStatus;

    /* Send NULL0 command - ignore return value (previous command unknown) */
    MC33937_SendCommand(NULL0_CMD);

    /* Send NULL1 command - return value is status register 0 value */
    devStatus.sr0.R = MC33937_ReadData(NULL1_CMD);
    /* Send NULL2 command - return value is status register 1 value */
    devStatus.sr1.R = MC33937_ReadData(NULL2_CMD);
    /* Send NULL3 command - return value is status register 2 value */
    devStatus.sr2.R = MC33937_ReadData(NULL3_CMD);
    /* Send NULL0 command - return value is status register 3 value */
    devStatus.sr3 = MC33937_ReadData(NULL0_CMD);

    return(devStatus);
}

/*******************************************************************************
*
* Function: uint16_t MC33937_DeadtimeSetup(void)
*
* Description:  Configures pre-driver dead time (see MC33937_routines.h
*               configuration section).
*
* Return:       uint16_t        Pre-driver internal status.
*
*******************************************************************************/
uint16_t MC33937_DeadtimeSetup(void)
{
    uint16_t setDeadtime;

    /* Sent command to set up dead time */
    MC33937_SendCommand(DEADTIME_CMD);

    PIT_CH1_Delay(PIT_20US);

    /* Dead time value in nanosecond: (required_dead_time * 16) */
    /* Pull down CS to set up dead time */
    MC33937_CS_LOW;
    PIT_CH1_Delay(DEADTIME_CALC);
    /* Pull up CS at the end of the dead time setup */
    MC33937_CS_HIGH;

    /* Read dead time register value */
    /* Send NULL3 command, ignore return value */
    MC33937_SendCommand(NULL3_CMD);
    /* Send NULL2 command, return value is status register 3 value */
    setDeadtime = MC33937_ReadData(NULL2_CMD);

    return(setDeadtime * 50);
}

/*******************************************************************************
*
* Function: uint8_t MC33937_Config(void)
*
* Description:  MC33937A MOSFET pre-driver configuration function.
*
* Return:       uint8_t         Configuration result
*                               0: dead time configuration unsuccessful
*                                  (requested dead time difference > 150ns)
*                               1: dead time configuration successful
*                                  (requested dead time difference <= 150ns)
*
*******************************************************************************/
uint8_t MC33937_Config(void)
{
    int16_t deltaDT;
    uint8_t error_cnt;
    uint8_t dt_set_fault;

    /* Double RESET strategy for MC33937 based on EB701 */
    /* Remove /RST; EN1 and EN2 are still low */
    MC33937_RST_SET;

    /* Wait defined time for power supply voltage stabilization */
    PIT_CH1_Delay(MC33937_POWER_UP_TIME);

    /* Set /RST; EN1 and EN2 are still low */
    MC33937_RST_CLEAR;

    /* Wait defined time for power supply voltage to discharge */
    PIT_CH1_Delay(MC33937_POWER_UP_TIME);

    /* Remove /RST; EN1 and EN2 are still low */
    MC33937_RST_SET;

    /* Wait defined time for power supply voltage stabilization */
    PIT_CH1_Delay(MC33937_POWER_UP_TIME);

    /* Clear faults */
    MC33937_ClearFaults();

    /* Send MASK0 and MASK1 command to mask unwanted interrupts */
    MC33937_SendCommand(MASK0_CMD);
    MC33937_SendCommand(MASK1_CMD);

    error_cnt = 5;
    /* Setup desired dead time of MC33937A in nanoseconds */
    do {
        deltaDT = DEADTIME - MC33937_DeadtimeSetup();
        dt_set_fault = ((deltaDT > 150) || (deltaDT < -150)) ? 1 : 0;
        error_cnt--;
    /* Dead time error is greater than 150 ns, setup dead time again */
    }while(dt_set_fault && error_cnt);

    /* Clear faults */
    MC33937_ClearFaults();

    /* Send MODE command with desired bits with LOCK bit to prevent further
       mode changes */
    MC33937_SendCommand(MODE_CMD);

    return(!dt_set_fault);
}
