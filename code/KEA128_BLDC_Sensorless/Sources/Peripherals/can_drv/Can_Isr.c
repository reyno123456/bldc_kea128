/****************************************************************************//*!
*
* (c) Copyright 2014 Freescale Semiconductor
*
********************************************************************************
*
* @file             Can_Isr.c
*
* @date             May-5-2014
*
* @version          0.0.1.0
*
* @brief            <b>CAN interrupt service routines source file</b>
*
* @note             Kinetis KEA128 version
*
*******************************************************************************/

#include "Can.h"
#include "Can_cfg.h"

/***************************************************************************//*!
*
*    @can_function    Can_ISR_Wake_0
*    @can_sync_async  Asynchronous
*    @can_reentrancy  Non Reentrant
*    @param[in]       None
*    @param[out]      None
*    @return          None
*    @brief           ISR wakeup function for CAN_0.
*    @attention       N/A
*
*******************************************************************************/

void Can_ISR_Wake_0(void){

  Can_Wakeup(0x00);
}


/***************************************************************************//*!
*
*    @can_function    Can_ISR_Rx_0
*    @can_sync_async  Asynchronous
*    @can_reentrancy  Non Reentrant
*    @param[in]       None
*    @param[out]      None
*    @return          None
*    @brief           ISR receive function for CAN_0.
*    @attention       N/A
*
*******************************************************************************/

void Can_ISR_Rx_0(void){

  Can_ReceiveData(0x00);
}

