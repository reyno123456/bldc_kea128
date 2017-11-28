/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     nvic.h
*
* @author   B06050
*
* @version  1.0.2.0
*
* @date     Jul-3-2014
*
* @brief    Nested Vector Interrupt Controller SW module header file.
*
*******************************************************************************/
#ifndef NVIC_H_
#define NVIC_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "derivative.h"

/*******************************************************************************
* Types
*******************************************************************************/
typedef enum
{
    NVIC_IRQ_PRIORITY_0 = 0,
    NVIC_IRQ_PRIORITY_1,
    NVIC_IRQ_PRIORITY_2,
    NVIC_IRQ_PRIORITY_3
}NVIC_IrqPriorityType;

/*******************************************************************************
* Global function prototypes
*******************************************************************************/
void NVIC_EnableIrq(uint8_t ui8Irq);
void NVIC_DisableIrq(uint8_t ui8Irq);
void NVIC_SetIrqPriority(uint8_t ui8Irq, NVIC_IrqPriorityType IrqPriority);

#endif /* NVIC_H_ */
