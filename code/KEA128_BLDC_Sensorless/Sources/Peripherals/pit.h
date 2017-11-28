/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     pit.h
*
* @author   B06050
*
* @version  1.0.2.0
*
* @date     Jul-3-2014
*
* @brief    Periodic Interrupt Timer SW module header file.
*
*******************************************************************************/
#ifndef PIT_H_
#define PIT_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "derivative.h"

/*******************************************************************************
* Constants and macros
*******************************************************************************/
/* PIT time-out periods valid for 24MHz bus clock */
/* PIT_period = (period * BUSclk) - 1 */
#define PIT_20US    479     /* 20us delay */
#define PIT_100US   2399    /* 100us delay */
#define PIT_1MS     23999   /* 1ms delay */
#define PIT_5MS     119999  /* 5ms delay */

/*******************************************************************************
* Global function prototypes
*******************************************************************************/
void PIT_Init(void);
void PIT_CH0_Init(uint32_t ui32Ldval);
void PIT_CH1_Delay(uint32_t ui32Delay);

#endif /* PIT_H_ */
