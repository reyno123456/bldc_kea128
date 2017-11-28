/****************************************************************************//*!
*
* (c) Copyright 2014 Freescale Semiconductor
*
********************************************************************************
*
* @file             Can_cfg.c
*
* @date             May-5-2014
*
* @version          0.0.1.0
*
* @brief            <b>CAN driver configuration file</b>
*
* @note             Kinetis KEA128 version
*
* @attention     1. The CAN module prescaler, Synchronization jump, Tseg1 and Tseg2
*                   are not shifted within the driver.
* @attention     2. Acceptance filter ID and filter mask are shifted.
*
*******************************************************************************/

#include "Can.h"
#include "Can_cfg.h"

/// CAN filter set configuration containers
Can_FilterSetType Can_FilterSet[]={ 
  {
    0x000F,/* CAN mask */
    0x0000, /* CAN ID */
    CAN_STD_ID /* Standard ID */
  }
};

/// CAN module configuration containers
Can_ModuleConfigType Can_ModuleConfig[]={
  {
    CAN_KEA128_CAN0, /* CAN0 module */
    0x05, /* Prescaler 6 */
    0x00, /* 1 Tq Synchronization jump width */
    0x04, /* 5Tq Tseg1 */
    0x10, /* 2Tq Tseg2 */
    CAN_SAMPL_1, /* Sampling mode */
    CAN_CLK_BUS, /* Module clock BUS CLK */
    0x01, /* Max filter count */
    CAN_FILTER_16, /* Filter mode configuration */
    Can_FilterSet, /* Pointer to filter configuration container */
  }
};

/// CAN register base address
volatile Can_RegsType *Can_RegsBase[]={
  (Can_RegsType *) CAN_KEA128_CAN_0_BASE
};
