/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     pit.c
*
* @author   B06050
*
* @version  1.0.2.0
*
* @date     Jul-3-2014
*
* @brief    Periodic Interrupt Timer SW module source file.
*
*******************************************************************************/
#include "pit.h"

/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void PIT_Init(void)
*
* Description:  This function initializes the PIT module.
*
*******************************************************************************/
__attribute__((always_inline)) void PIT_Init(void)
{
    PIT_MCR = PIT_MCR_FRZ_MASK; /* Enable PIT module, freeze in Debug mode */
}

/*******************************************************************************
*
* Function: void PIT_CH0_Init(uint32_t ui32Period)
*
* Description:  This function starts the PIT channel 0 counter with selected
*               time-out period.
* 
* Param[in]:    ui32Period  Timeout period in PIT clock ticks
*
*******************************************************************************/
void PIT_CH0_Init(uint32_t ui32Period)
{
    PIT_LDVAL0 = ui32Period;            /* Counter start value */
    /* Enable channel interrupt, enable channel */
    PIT_TCTRL0 = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
}

/*******************************************************************************
*
* Function: void PIT1_Delay(void)
*
* Description:  This function generates a delay (blocking).
*
* Params[in]:   ui32Delay   Delay period in PIT clock ticks
*
*******************************************************************************/
__attribute__((always_inline)) void PIT_CH1_Delay(uint32_t ui32Delay)
{
    PIT_LDVAL1 = ui32Delay;             /* Counter start value */
    PIT_TCTRL1 = PIT_TCTRL_TEN_MASK;    /* Enable PTI timer channel 1 */

    while(PIT_TFLG1 != 1)
    {
    }
    PIT_TCTRL1 = 0x00000000;            /* Disable PTI timer channel 1 */
    PIT_TFLG1 = PIT_TFLG_TIF_MASK;      /* Clear TIF flag */
}
