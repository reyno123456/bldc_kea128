/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     rtc.c
*
* @author   B06050
*
* @version  1.0.1.0
*
* @date     Jun-18-2014
*
* @brief    Real Time Counter SW module source file.
*
*******************************************************************************/
#include "rtc.h"

/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void RTC_Init(uint16_t ui16Modulo)
*
* Description:  This function initializes the RTC module with selected counter
*               modulo value. RTC will run out of ICSIRCLK clock.
*
* Param[in]:    ui16Modulo  RTC counter modulo value
*
*******************************************************************************/
void RTC_Init(uint16_t ui16Modulo)
{
    /* RTC counter modulo */
    RTC_MOD = ui16Modulo;
    /* Internal reference clock (ICSIRCLK) with prescaler 1 selected as an input
       clock */
    RTC_SC = RTC_SC_RTCLKS(2) | RTC_SC_RTCPS(1) | RTC_SC_RTIF_MASK;
}
