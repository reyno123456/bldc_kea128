/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     rtc.h
*
* @author   B06050
*
* @version  1.0.1.0
*
* @date     Jun-18-2014
*
* @brief    Real Time Counter SW module header file.
*
*******************************************************************************/
#ifndef RTC_H_
#define RTC_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "derivative.h"

/*******************************************************************************
* Constants and macros
*******************************************************************************/
#define RTC_IRC_TICKS_2     1

/*******************************************************************************
* Global function prototypes
*******************************************************************************/
void RTC_Init(uint16_t ui16Modulo);

#endif /* RTC_H_ */
