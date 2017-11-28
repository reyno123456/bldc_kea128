/****************************************************************************//*!
*
* (c) Copyright 2014 Freescale Semiconductor
*
********************************************************************************
*
* @file             Can_cfg.h
*
* @date             May-5-2014
*
* @version          0.0.1.0
*
* @brief            <b>CAN driver configuration header file</b>
*
* @note             Kinetis KEA128 version
*
*******************************************************************************/
#ifndef CAN_CFG_H
#define CAN_CFG_H

#include "Can.h"

#define CAN_MODULE_MAX_ID 1

extern volatile Can_RegsType *Can_RegsBase[];

extern Can_ModuleConfigType Can_ModuleConfig[];

extern Can_FilterSetType Can_FilterSet[];

#endif /* CAN_CFG_H */
