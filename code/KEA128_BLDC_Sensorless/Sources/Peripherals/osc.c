/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     osc.c
*
* @author   B06050
*
* @version  1.0.2.0
*
* @date     Jul-3-2014
*
* @brief    Oscillator SW module source file.
*
*******************************************************************************/
#include "osc.h"

/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void OSC_Init(void)
*
* Description:  Initializes and enables Oscillator module:
*               - Low-power mode
*               - High frequency range of 4-24 MHz
*
*******************************************************************************/
void OSC_Init(void)
{
    /* Enable OSC */
    OSC_CR = 0x94;
    /* Wait for OSC to be initialized */
    while((OSC_CR & OSC_CR_OSCINIT_MASK) == 0)
    {
    }
}
