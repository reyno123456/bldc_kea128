/****************************************************************************//*!
*
* (c) Copyright 2014 Freescale Semiconductor
*
********************************************************************************
*
* @file             Can.h
*
* @date             May-5-2014
*
* @version          0.0.1.0
*
* @brief            <b>CAN driver header file</b>
*
* @note             Kinetis KEA128 version
*
*******************************************************************************/

#ifndef CAN_H
#define CAN_H

#include "derivative.h"
#include "stddef.h"

/*******************************************************************************
*
*           General data types
*
*******************************************************************************/

typedef uint8_t boolean;


/*******************************************************************************
*
*           CAN module register access
*
*******************************************************************************/

/// CAN message buffer structure
typedef struct{
  uint32_t id;
  uint8_t data[8];
  uint8_t dlc;
  uint8_t tbpr;
  uint8_t tsrh;
  uint8_t tsrl;
}Can_MBRegType;

/// CAN module register structure
typedef struct{
  uint8_t ctl0;
  uint8_t ctl1;
  uint8_t btr0;
  uint8_t btr1;
  uint8_t rflg;
  uint8_t rier;
  uint8_t tflg;
  uint8_t tier;
  uint8_t tarq;
  uint8_t taak;
  uint8_t tbsel;
  uint8_t idac;
  uint8_t _reserved[2];
  uint8_t rxerr;
  uint8_t txerr;
  uint8_t idar0[4];
  uint8_t idmr0[4];
  uint8_t idar1[4];
  uint8_t idmr1[4];
  Can_MBRegType rxfg;
  Can_MBRegType txfg;
}Can_RegsType;

/// CAN boolean values
#define CAN_FALSE   0
#define CAN_TRUE    1

/// CAN module base addresses (duplicity intended)
#define CAN_KEA128_CAN_0_BASE 0x40024000

/// CAN module index
#define CAN_KEA128_CAN0 0

/// CTL0 register masks
#define CAN_CTL0_RXFRM  0x80
#define CAN_CTL0_RXACT  0x40
#define CAN_CTL0_CSWAI  0x20
#define CAN_CTL0_SYNCH  0x10
#define CAN_CTL0_TIME   0x08
#define CAN_CTL0_WUPE   0x04
#define CAN_CTL0_SLPRQ  0x02
#define CAN_CTL0_INITRQ 0x01

/// CTL1 register masks
#define CAN_CTL1_CANE   0x80
#define CAN_CTL1_CLKSRC 0x40
#define CAN_CTL1_LOOPB  0x20
#define CAN_CTL1_LISTEN 0x10
#define CAN_CTL1_WUPM   0x04
#define CAN_CTL1_SLPAK  0x02
#define CAN_CTL1_INITAK 0x01

/// BTR0 register masks
#define CAN_BTR0_SJW    0xC0
#define CAN_BTR0_BRP    0x3F

/// BTR1 register masks
#define CAN_BTR1_SAMP   0x80
#define CAN_BTR1_TSEG2  0x70
#define CAN_BTR1_TSEG1  0x0F

/// RFLG register masks
#define CAN_RFLG_WUPIF  0x80
#define CAN_RFLG_CSCIF  0x40
#define CAN_RFLG_RSTAT  0x30
#define CAN_RFLG_TSTAT  0x0C
#define CAN_RFLG_OVRIF  0x02
#define CAN_RFLG_RXF    0x01

/// RIER register masks
#define CAN_RIER_WUPIE  0x80
#define CAN_RIER_CSCIE  0x40
#define CAN_RIER_RSTATE 0x30
#define CAN_RIER_TSTATE 0x0C
#define CAN_RIER_OVRIE  0x02
#define CAN_RIER_RXFIE  0x01

#define CAN_TFLG_TXE    0x07

#define CAN_TIER_TXEIE  0x07

#define CAN_TARQ_ABTRQ  0x07

#define CAN_TAAK_ABTAK  0x07

#define CAN_TBSEL_TX    0x07

#define CAN_IDAC_IDAM   0x30
#define CAN_IDAC_IDHIT  0x07


/*******************************************************************************
*
*           Configuration structures and types
*
*******************************************************************************/

/// Module identification data type
typedef uint8_t Can_ModuleIDType;

/// Baudrate data type
typedef uint8_t Can_PrescalerType;

/// Resynchronization jump width data type
typedef uint8_t Can_JumpWidthType;

/// Phase segment 1 data type
typedef uint8_t Can_Seg1Type;

/// Phase segment 2 data type
typedef uint8_t Can_Seg2Type;

/// Sampling mode */
typedef enum{
  CAN_SAMPL_1 = 0x00,
  CAN_SAMPL_3 = 0x80
}Can_SamplModeType;

/// CAN module clock source data type
typedef enum{
  CAN_CLK_BUS = 0x40,
  CAN_CLK_OSC = 0x00
}Can_ModuleClkType;

/// Message identificator data type
typedef uint32_t  Can_IDType;

/// Message data length data type
typedef uint8_t   Can_DataLengthType;

/// Acknowledge error flag */
typedef boolean   Can_AckErrType;

/// CRC error flag data type
typedef boolean   Can_CrcErrType;

/// Tx error counter>=96 flag data type
typedef boolean   Can_TxWrnType;

/// Rx error counter>=96 flag data type
typedef boolean   Can_RxWrnType;

/// Bus idle error flag data type
typedef boolean   Can_BusIdleType;

/// CAN drivers status data type
typedef enum{
  CAN_UNINIT = 0x00,
  CAN_INIT = 0x01,
  CAN_RUN = 0x02,
  CAN_SLEEP = 0x03,
  CAN_RX = 0x04,
  CAN_TX = 0x05,
  CAN_ERR = 0x06
}Can_DriverStatusType;

/// CAN drivers mode data type
typedef enum{
  CAN_MODE_UNINIT = 0x00,
  CAN_MODE_INIT = 0x01,
  CAN_MODE_RUN = 0x02,
  CAN_MODE_SLEEP = 0x03
}Can_ModeType;

/// CAN driver return data type
typedef enum{
  CAN_OK = 0x00,
  CAN_NOT_OK = 0x01
}Can_ReturnType;

/// CAN ID mode data type
typedef enum{
  CAN_STD_ID = 0x00,
  CAN_EXT_ID = 0x01
}Can_IDModeType;

/// Acceptance filter mode data type
typedef enum{
  CAN_FILTER_32 = 0x00,
  CAN_FILTER_16 = 0x10,
  CAN_FILTER_8  = 0x20,
  CAN_FILTER_CLOSE = 0x30
}Can_FilterModeType;

/// Acceptance filter mask data type
typedef uint32_t   Can_FilterMaskType;

/// Acceptance filter ID data type
typedef uint32_t   Can_FilterIDType;

/// Filter mask set data type
typedef struct{
  Can_FilterMaskType  Can_FilterMask;
  Can_FilterIDType    Can_FilterID;
  Can_IDModeType      Can_IDMode;
}Can_FilterSetType;

/// CAN module configuration structure data type
typedef struct{
  Can_ModuleIDType        Can_ModuleID;
  Can_PrescalerType       Can_Prescaler;
  Can_JumpWidthType       Can_JumpWidth;
  Can_Seg1Type            Can_Seg1;
  Can_Seg2Type            Can_Seg2;
  Can_SamplModeType       Can_SamplMode;
  Can_ModuleClkType       Can_ModuleClk;
  uint8_t                 Can_MaxFilterCount;
  Can_FilterModeType      Can_FilterMode;
  Can_FilterSetType       *Can_FilterSet;
} Can_ModuleConfigType;

/// CAN message buffer data type
typedef struct{
  uint8_t                 Can_Index;
  Can_IDModeType          Can_IDMode;
  Can_IDType              Can_ID;
  Can_DataLengthType      Can_DataLength;
  uint8_t                 Can_DataStorage[8];
  uint8_t                 Can_Priority;
  uint8_t                 Can_Tsrh;
  uint8_t                 Can_Tsrl;
} Can_DataType;

/// CAN module flags data type
typedef struct{
  uint8_t                 Can_TxWrn   :2; /* Tx counter warning */
  uint8_t                 Can_RxWrn   :2; /* Rx counter warning */
  uint8_t                 Can_OvrRun  :1; /* Overrun flag */
  uint8_t                 Can_RecAct  :1; /* Receiver active flag */
  uint8_t                 Can_Synch   :1; /* Can module synchronization flag */
}Can_ModuleFlagType;

/// CAN module status data type
typedef struct{
  Can_ModuleFlagType      Can_Flags;
  Can_DriverStatusType    Can_DriverStatus;
} Can_StatusType;

/*******************************************************************************
*
*           Function prototypes
*
*******************************************************************************/

extern Can_ReturnType Can_Init(
  Can_ModuleConfigType *Can_ModuleConfig
);

extern Can_ReturnType Can_SentData(
  Can_ModuleIDType  Can_ModuleID,
  Can_DataType      *Can_Data
);

extern void Can_RxNotification(
  Can_ModuleIDType    Can_ModuleID,
  Can_IDType          Can_ID,
  Can_DataLengthType  Can_DataLength,
  uint8_t             *Can_Data
);

extern void Can_WakeupNotification(
  Can_ModuleIDType    Can_ModuleID
);

extern void Can_ReceiveData(
  Can_ModuleIDType  Can_ModuleID
);

extern Can_ReturnType Can_GetStatus(
  Can_ModuleIDType  Can_ModuleID,
  Can_StatusType    *Can_Status
);

extern Can_ReturnType Can_SetMode(
  Can_ModuleIDType  Can_ModuleID,
  Can_ModeType      Can_Mode
);

extern void Can_Wakeup(
  Can_ModuleIDType  Can_ModuleID  
);

#endif /* CAN_H */
