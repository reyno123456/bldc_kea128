/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     acmp.c
*
* @author   B06050
*
* @version  1.0.2.0
*
* @date     Jul-3-2014
*
* @brief    Analog Comparator SW module source file.
*
*******************************************************************************/
#include "acmp.h"

/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void ADC_Init(void)
*
* Description:  This function initializes the ACMP module 0 for the DC Bus
*               overcurrent detection.
*
*******************************************************************************/
void ACMP0_Init(uint8_t ui8Ref)
{
    /* IN+: ACMP0_IN1, IN-: DAC_OUT */
    ACMP0_C0 = ACMP_C0_ACPSEL(1) | ACMP_C0_ACNSEL(3);
    /* Enable DAC with VDDA as reference, set DAC output voltage equal to:
       Vdac = (ui8Ref+1) x (VDDA/64) */
    ACMP0_C1 = ACMP_C1_DACEN_MASK | ACMP_C1_DACREF_MASK | ACMP_C1_DACVAL(ui8Ref);
    /* ACMP0_IN1 analog input enabled */
    ACMP0_C2 = ACMP_C2_ACIPE(2);
    /* Enable ACMP0 */
    ACMP0_CS = ACMP_CS_ACE_MASK;
}

/*******************************************************************************
*
* Function: void ADC_Init(void)
*
* Description:  This function initializes the ACMP module 1 for the DC Bus
*               undervoltage detection.
*
*******************************************************************************/
void ACMP1_Init(uint8_t ui8Ref)
{
    /* IN+: DAC_OUT, IN-: ACMP1_IN2 */
    ACMP1_C0 = ACMP_C0_ACPSEL(3) | ACMP_C0_ACNSEL(2);
    /* Enable DAC with VDDA as reference, set DAC output voltage equal to:
       Vdac = (ui8Ref+1) x (VDDA/64) */
    ACMP1_C1 = ACMP_C1_DACEN_MASK | ACMP_C1_DACREF_MASK | ACMP_C1_DACVAL(ui8Ref);
    /* ACMP0_IN2 analog input enabled */
    ACMP1_C2 = ACMP_C2_ACIPE(4);
    /* Enable ACMP1 */
    ACMP1_CS = ACMP_CS_ACE_MASK;
}
