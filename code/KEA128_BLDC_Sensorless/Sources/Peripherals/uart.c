/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     uart.c
*
* @author   B06050
*
* @version  1.0.3.0
*
* @date     Jul-3-2014
*
* @brief    Universal Asynchronous Receiver/Transmitter SW module source file.
*
*******************************************************************************/
#include "uart.h"

/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void UART_Init(void)
*
* Description:  This function initializes the SCI module baud rate, frame
*               format, and enables UART transmitter and receiver.
*
*******************************************************************************/
void UART2_Init(void)
{
    /* 115200kBd @24MHz bus clock */
    UART2_BDH = 0x0;
    UART2_BDL = 0x0D;

    UART2_C1 = 0;       /* 8-bit mode. Normal operation */

    /* Transmitter and receiver enable */
    UART2_C2 = (0|UART_C2_RE_MASK|UART_C2_TE_MASK);

    UART2_C3 = 0;       /* Disable all errors interrupts */
}
