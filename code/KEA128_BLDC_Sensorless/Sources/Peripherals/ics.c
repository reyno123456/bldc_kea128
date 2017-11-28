/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     ics.c
*
* @author   B06050
*
* @version  1.0.2.0
*
* @date     Jul-3-2014
*
* @brief    Internal Clock Source SW module source file.
*
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include "ics.h"

/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void ICS_Init_FEE(void)
*
* Description:  This function sets the system clock to be derived from 40MHz FLL
*               output (external 16MHz clock external reference divided by 512
*               and multiplied by 1280) for ICSIRCLK clock frequency trimming.
*               The FLL operates in engaged external (FEE) mode.
*
*******************************************************************************/
void ICS_Init_FEE(void)
{
    /* Select FLL output clock as the clock source for the bus frequency,
       external reference divided by 512, ICSIRCLK active */
    ICS_C1 = ICS_C1_CLKS(0) | ICS_C1_RDIV(4) | ICS_C1_IRCLKEN_MASK;

    /* Wait for the FLL to lock */
    while((ICS_S & ICS_S_LOCK_MASK) == 0)
    {
    }

    /* FLL output clock divided by 1 to obtain ICSOUTCLK */
    ICS_C2 = 0x00;
}

/*******************************************************************************
*
* Function: void ICS_Init_FEI(void)
*
* Description:  This function sets the system clock to be derived from 48MHz
*               FLL output (trimmed 37.5kHz ICSIRCLK reference clock multiplied
*               by 1280). The FLL operates in engaged internal (FEI) mode.
*
*******************************************************************************/
void ICS_Init_FEI(void)
{
    /* Select FLL output clock as the clock source for the bus frequency,
           internal reference divided by 1, ICSIRCLK active */
    ICS_C1 = ICS_C1_CLKS(0) | ICS_C1_RDIV(0) | ICS_C1_IREFS_MASK | \
             ICS_C1_IRCLKEN_MASK;

    /* Wait for the FLL to lock */
    while((ICS_S & ICS_S_LOCK_MASK) == 0)
    {
    }

    /* FLL output clock divided by 1 to obtain ICSOUTCLK */
    ICS_C2 = 0x00;
}
