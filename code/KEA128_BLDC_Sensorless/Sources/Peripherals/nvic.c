/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     nvic.c
*
* @author   B06050
*
* @version  1.0.3.0
*
* @date     Jul-3-2014
*
* @brief    Nested Vector Interrupt Controller SW module header file.
*
*******************************************************************************/
#include "nvic.h"

/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void NVIC_EnableIrq(uint8_t ui8Irq)
*
* Description:  Enables the selected interrupt.
*
* Param[in]:    ui8Irq  Interrupt vector number.
*
*******************************************************************************/
void NVIC_EnableIrq (uint8_t ui8Irq)
{
    ui8Irq -= 16;

    if(!(ui8Irq > 31))
    {
        /* Set the ICPR register accordingly */
        NVIC_ISER = (1 << ui8Irq);
    }
}

/*******************************************************************************
*
* Function: void NVIC_DisableIrq(uint8_t ui8Irq)
*
* Description:  Disables the selected interrupt.
*
* Param[in]:    ui8Irq	Interrupt vector number.
*
*******************************************************************************/
void NVIC_DisableIrq (uint8_t ui8Irq)
{
    ui8Irq -= 16;

    if(!(ui8Irq > 31))
    {
        /* Set the ICER register accordingly */
        NVIC_ICER = (1 << ui8Irq);
    }
}

/*******************************************************************************
*
* Function: void NVIC_SetIrqPriority (uint8_t ui8Irq, NVIC_IrqPriorityType IrqPriority)
*
* Description:  Sets the priority of the selected interrupt.
*
* Params[in]:   ui8Irq			Interrupt vector number.
* 				IrqPriority		Interrupt priority.
*******************************************************************************/
void NVIC_SetIrqPriority (uint8_t ui8Irq, NVIC_IrqPriorityType IrqPriority)
{
    uint8_t ui8Offset, ui8Shift;

    ui8Irq -= 16;

    if(!(ui8Irq > 31))
    {
            ui8Offset = ui8Irq >> 2;
            ui8Shift = ui8Irq % 4;
            /* Set the IPR[ui8Offset] register accordingly */
            NVIC_IP(ui8Offset) = (IrqPriority << (6 + (ui8Shift << 3)));
    }
}
