/****************************************************************************//*!
*
* (c) Copyright 2014 Freescale Semiconductor
*
********************************************************************************
*
* @file             Can.c
*
* @date             May-5-2014
*
* @version          0.0.1.0
*
* @brief            <b>CAN driver source file</b>
*
*
*******************************************************************************/
/****************************************************************************//*!
*
*  @mainpage <b>CAN driver for Kinetis SKEAZ128</b>
*
*  @section Intro Introduction
*
*   This package contains the SW driver for Freescale MSCAN module.<br>
*   <br><br>
*   <b>Known limitations</b>
*   - The CAN module on Kinetis KEA128 is FIFO architecture with three transmit buffers
*     and five receive buffers (one foreground and four background buffers).
*   - The acceptance filter can be configured in the following way:
*     -# 2x full 32-bit ID
*     -# 4x 16-bit ID
*     -# 8x 8-bit ID
*   - This driver can run only in interrupt mode, thus for correct behaviour it is
*     necessary to place appropriate ISR handlers to the vector table.
*   - Acceptance filter ID and filter mask in configuration are shifted during Rx/Tx.
*   - The CAN module prescaler, Synchronization jump, Tseg1 and Tseg2
*     are not shifted within the driver, it have to be entered in correct format.
*   - The driver remains in INIT state after the Can_Init function call.<br><br>
*
*   <b>Recommended installation sequence</b><br><br>
*   To use the this CAN driver in user application please follow these steps:
*   -# Configure the filters and CAN module in can_cfg.h file.
*   -# Include the can.h and can_cfg.h files to user application.
*   -# In user application configure the message buffer objects.
*   -# Configure the pins used by CAN module.
*   -# Enable the CAN physical layer if necessary.
*   -# Initialize the CAN driver by Can_Init function call.
*   -# Set the CAN driver to RUN mode by Can_SetMode function call.
*   -# Use the Can_SendData function to transmit the data over the CAN.
*   -# Upon the CAN message reception the Can_RxNotification notification function is called.
*
*   @attention
*    <b>NO WARRANTY</b>. TO THE MAXIMUM EXTENT PERMITTED BY LAW, FREESCALE
*    EXPRESSLY DISCLAIMS ANY WARRANTY FOR THE SOFTWARE. THE SOFTWARE IS
*    PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR
*    IMPLIED, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR NONINFRINGEMENT.
*    YOU ASSUME THE ENTIRE RISK ARISING OUT OF THE USE OR
*    PERFORMANCE OF THE SOFTWARE, OR ANY SYSTEMS YOU DESIGN USING THE
*    SOFTWARE (IF ANY). NOTHING IN THIS AGREEMENT MAY BE CONSTRUED AS A
*    WARRANTY OR REPRESENTATION BY FREESCALE THAT THE SOFTWARE OR ANY
*    DERIVATIVE WORK DEVELOPED WITH OR INCORPORATING THE SOFTWARE WILL
*    BE FREE FROM INFRINGEMENT OF THE INTELLECTUAL PROPERTY RIGHTS OF THIRD PARTIES.
*
*******************************************************************************/

#include "Can.h"
#include "Can_cfg.h"


/*******************************************************************************
*
*           Global variables
*
*******************************************************************************/

Can_DriverStatusType Can_DriverStatus[CAN_MODULE_MAX_ID];
#define CAN_MAX_LOOP_COUNT 1000


/***************************************************************************//*!
*
*    @can_function    Can_Init
*    @can_sync_async  Asynchronous
*    @can_reentrancy  Non Reentrant
*    @param[in]       *Can_ModuleConfig - pointer to CAN Driver configuration.
*    @param[out]      None
*    @return          Can_ReturnType - return value of the CAN driver API (CAN_OK / CAN_NOT_OK).
*    @brief           Function initializes the CAN module based on Can_ModuleConfig configuration.
*    @attention       N/A
*
*******************************************************************************/

Can_ReturnType Can_Init(
  Can_ModuleConfigType *Config
){
  Can_ReturnType ret_val = CAN_OK;
  uint8_t module_id,i,j;
  uint16_t timer = CAN_MAX_LOOP_COUNT;
  volatile Can_RegsType *Can_Regs;
  Can_FilterSetType *Can_Filter;
  Can_FilterMaskType Can_Mask;
  Can_FilterIDType Can_ID;

  if (Config == NULL){ /* Check for valid pointer */
    ret_val = CAN_NOT_OK;
  }else{
    module_id = Config->Can_ModuleID;
    /* Set uninit mode to module status */
    Can_DriverStatus[module_id] = CAN_UNINIT;
    /* Get the register base */
    Can_Regs = Can_RegsBase[module_id];
    /* Enable CAN module */
    Can_Regs->ctl1 |= CAN_CTL1_CANE; 
    /* Enter initialization mode */
    Can_Regs->ctl0 |= CAN_CTL0_INITRQ;
    /* Set CAN module source clock */
    Can_Regs->ctl1 |= (Config->Can_ModuleClk)&CAN_CTL1_CLKSRC;
    /* Wait for init mode acknowledge */
    while ((((timer>0)&(Can_Regs->ctl1 & CAN_CTL1_INITAK))!=CAN_CTL1_INITAK)){
      timer--; 
    }
    if (timer == 0){
      /* Init mode is not acknowledge */
      ret_val = CAN_NOT_OK;
    }else{
      
      /* Set the prescaler and synch. jump width from config */
      Can_Regs->btr0 = ((Config->Can_JumpWidth)&CAN_BTR0_SJW)|
                       ((Config->Can_Prescaler)&CAN_BTR0_BRP);
      /* Set the sampling mode and Tseg from config */
      Can_Regs->btr1 = ((Config->Can_Seg1)&CAN_BTR1_TSEG1)|
                       ((Config->Can_Seg2)&CAN_BTR1_TSEG2)|
                       ((Config->Can_SamplMode)&CAN_BTR1_SAMP);
      /* Set acceptance filter mode */
      Can_Regs->idac = Config->Can_FilterMode&CAN_IDAC_IDAM;
      /* Get the filter set base */
      Can_Filter = Config->Can_FilterSet;
      switch (Config->Can_FilterMode){
        case CAN_FILTER_32: /* 32-bit acceptance filter mode */
        for(i=0, j=0;(j < (Config->Can_MaxFilterCount))&(j<2);i+=4, j++){
          if(Can_Filter->Can_IDMode == CAN_STD_ID){
            Can_ID = (uint32_t)((Can_Filter[j].Can_FilterID)<<21);
            Can_Mask = (uint32_t)((Can_Filter[j].Can_FilterMask) << 21);
          }else{
            Can_ID = (uint32_t)((Can_Filter[j].Can_FilterID)<<1);
            Can_Mask = (uint32_t)((Can_Filter[j].Can_FilterMask) << 1);
          }
          if (i<4){
            Can_Regs->idmr0[3] = (uint8_t)Can_Mask;
            Can_Regs->idar0[3] = (uint8_t)Can_ID;
            Can_Regs->idmr0[2] = (uint8_t)(Can_Mask>>8);
            Can_Regs->idar0[2] = (uint8_t)(Can_ID>>8);
            Can_Regs->idmr0[1] = (uint8_t)(Can_Mask>>16);
            Can_Regs->idar0[1] = (uint8_t)(Can_ID>>16);
            Can_Regs->idmr0[0] = (uint8_t)(Can_Mask>>24);
            Can_Regs->idar0[0] = (uint8_t)(Can_ID>>24);
          }else{
            Can_Regs->idmr1[3] = (uint8_t)Can_Mask;
            Can_Regs->idar1[3] = (uint8_t)Can_ID;
            Can_Regs->idmr1[2] = (uint8_t)(Can_Mask>>8);
            Can_Regs->idar1[2] = (uint8_t)(Can_ID>>8);
            Can_Regs->idmr1[1] = (uint8_t)(Can_Mask>>16);
            Can_Regs->idar1[1] = (uint8_t)(Can_ID>>16);
            Can_Regs->idmr1[0] = (uint8_t)((Can_Filter[j].Can_FilterMask)>>24);
            Can_Regs->idar1[0] = (uint8_t)((Can_ID)>>24);
          }
        }
        break;
        case CAN_FILTER_16: /* 16-bit acceptance filter mode */
        for(i=0, j=0;(j < (Config->Can_MaxFilterCount))&(j<4);i+=2, j++){
          if(Can_Filter->Can_IDMode == CAN_STD_ID){
            Can_ID = (uint32_t)((Can_Filter[j].Can_FilterID)<<5);
            Can_Mask = (uint32_t)((Can_Filter[j].Can_FilterMask) << 5);
          }else{
            Can_ID = (uint32_t)((Can_Filter[j].Can_FilterID)>>15);
            Can_Mask = (uint32_t)((Can_Filter[j].Can_FilterMask) >> 15);
          }
          if (i<4){
            Can_Regs->idmr0[i+1] = (uint8_t)Can_Mask;
            Can_Regs->idar0[i+1] = (uint8_t)Can_ID;
            Can_Regs->idmr0[i] = (uint8_t)(Can_Mask>>8);
            Can_Regs->idar0[i] = (uint8_t)(Can_ID>>8);
          }else{
            Can_Regs->idmr1[i-3] = (uint8_t)Can_Mask;
            Can_Regs->idar1[i-3] = (uint8_t)Can_ID;
            Can_Regs->idmr1[i-4] = (uint8_t)(Can_Mask>>8);
            Can_Regs->idar1[i-4] = (uint8_t)(Can_ID>>8);
          }
        }
        break;
        case CAN_FILTER_8: /* 8-bit acceptance filter mode */
        for(i=0, j=0;(j < (Config->Can_MaxFilterCount))&(j<8);i++, j++){
          if(Can_Filter->Can_IDMode == CAN_STD_ID){
            Can_ID = (uint32_t)((Can_Filter[j].Can_FilterID)>>3);
            Can_Mask = (uint32_t)((Can_Filter[j].Can_FilterMask) >> 3);
          }else{
            Can_ID = (uint32_t)((Can_Filter[j].Can_FilterID)>>23);
            Can_Mask = (uint32_t)((Can_Filter[j].Can_FilterMask) >> 23);
          }
          if (i<4){
            Can_Regs->idmr0[i] = (uint8_t)Can_Mask;
            Can_Regs->idar0[i] = (uint8_t)Can_ID;
          }else{
            Can_Regs->idmr1[i-4] = (uint8_t)Can_Mask;
            Can_Regs->idar1[i-4] = (uint8_t)Can_ID;
          }
        }
        break;
        default:
        break;
      }
      Can_Regs->ctl1 |= CAN_CTL1_WUPM;    /* Wakeup filter on */
      /* Module successfully initialized, change mode */
      Can_DriverStatus[module_id] = CAN_INIT;
    }
  }
  return (ret_val);
}


/***************************************************************************//*!
*
*    @can_function    Can_SentData
*    @can_sync_async  Asynchronous
*    @can_reentrancy  Non Reentrant
*    @param[in]       Can_ModuleID - CAN module to be used
*                     *Can_Data - pointer to data to be transmitted.
*    @param[out]      None
*    @return          Can_ReturnType - return value of the CAN driver API (CAN_OK / CAN_NOT_OK).
*    @brief           Function send the data from Can_Data pointer via Can_ModuleID CAN module.
*    @attention       N/A
*
*******************************************************************************/

Can_ReturnType Can_SentData(
  Can_ModuleIDType  Can_ModuleID,
  Can_DataType      *Can_Data
){
  Can_ReturnType ret_val = CAN_OK;
  volatile Can_RegsType *Can_Regs;
  uint8_t txbuffer,i,data_length;

  /* Check for valid pointer and RUN state */
  if ((Can_Data == NULL)&&(Can_DriverStatus[Can_ModuleID] == CAN_RUN)){
    ret_val = CAN_NOT_OK;
  }else{
    /* Set the driver status to Tx*/
    Can_DriverStatus[Can_ModuleID] = CAN_TX;
    /* Get the actual register base address */
    Can_Regs = Can_RegsBase[Can_ModuleID];
    /* Get the empty buffers */
    txbuffer = Can_Regs->tflg;
    /* Check for empty buffer */
    if (txbuffer == 0x00){
      ret_val = CAN_NOT_OK;
    }else{ 
      /* Select the lowest empty buffer */
      Can_Regs->tbsel = txbuffer;
      /* Store selected empty buffer */
      txbuffer = Can_Regs->tbsel;
      /* Copy the message ID */
      if ((Can_Data->Can_IDMode) == CAN_STD_ID){      
        Can_Regs->txfg.id = ((Can_Data->Can_ID)<<0x15)&0xFFE00000;
      }else{
        Can_Regs->txfg.id = (((Can_Data->Can_ID)&0x1FFC0000)<<0x03)|(((Can_Data->Can_ID)<<0x01)&0x0007FFFE)|(0x00180000);
      }     
      /* Get the data length */
      data_length = Can_Data->Can_DataLength;
      /* Set the data length */
      Can_Regs->txfg.dlc = data_length;
      /* Copy data to message fore*/
      for (i=0;((i< data_length)&(i<8));i++){
        Can_Regs->txfg.data[i] = Can_Data->Can_DataStorage[i];
      }
      /* Set the message buffer priority */
      Can_Regs->txfg.tbpr = Can_Data->Can_Priority;
      /* Start transmission */
      Can_Regs->tflg = txbuffer;
    }
    /* Return back to init state */
    Can_DriverStatus[Can_ModuleID] = CAN_INIT; 
  }
  return (ret_val);    
}


/***************************************************************************//*!
*
*    @can_function    Can_ReceiveData
*    @can_sync_async  Asynchronous
*    @can_reentrancy  Non Reentrant
*    @param[in]       Can_ModuleID - CAN module to be used
*                     *Can_Data - pointer to data to be transmitted.
*    @param[out]      None
*    @return          Can_ReturnType - return value of the CAN driver API (CAN_OK / CAN_NOT_OK).
*    @brief           Function receive data from Can_ModuleID CAN module and call the notification function.
*    @attention       N/A
*
*******************************************************************************/

void Can_ReceiveData(
  Can_ModuleIDType  Can_ModuleID
){
  volatile Can_RegsType *Can_Regs;
  Can_IDType          can_ID;
  Can_DataLengthType  length;
  uint8_t             data[8],i;
    
  /* Get the actual register base address */
  Can_Regs = Can_RegsBase[Can_ModuleID];
  /* Disable Rx interrupt */
  Can_Regs->rier &= ~CAN_RIER_RXFIE; 
  /* Set the module status to Rx state */
  Can_DriverStatus[Can_ModuleID] = CAN_RX;      
  /* Copy message ID */
  if (((Can_Regs->rxfg.id)&0x00080000) == 0x00080000){
    /* Extend ID, extract the ID */
    can_ID = (((Can_Regs->rxfg.id)&0xFFE00000)>>0x03)|(((Can_Regs->rxfg.id)&0x0007FFFE)>>0x01);
  }else{
    /* Standard ID, just shifting */
    can_ID = ((Can_Regs->rxfg.id)&0xFFE00000)>>0x15;  
  }     
  /* Get the message length */
  length = Can_Regs->rxfg.dlc;
  /* Copy data */
  for (i=0;((i<length)&(i<8));i++){
    data[i] = Can_Regs->rxfg.data[i];
  }
  /* Set the module status back to init state */
  Can_DriverStatus[Can_ModuleID] = CAN_INIT;
  /* Call the notification function */
  Can_RxNotification(Can_ModuleID, can_ID, length, data);
  /* Clear Rx ISR flag */
  Can_Regs->rflg &= CAN_RFLG_RXF;
  /* Enable Rx interrupt */
  Can_Regs->rier |= CAN_RIER_RXFIE;     
}


/***************************************************************************//*!
*
*    @can_function    Can_GetStatus
*    @can_sync_async  Synchronous
*    @can_reentrancy  Non Reentrant
*    @param[in]       Can_ModuleID - CAN module to be used                     
*    @param[out]      *Can_Status - pointer to driver status.
*    @return          Can_ReturnType - return value of the CAN driver API (CAN_OK / CAN_NOT_OK).
*    @brief           Function return status of Can_ModuleID module and CAN driver.
*    @attention       N/A
*
*******************************************************************************/

Can_ReturnType Can_GetStatus(
  Can_ModuleIDType  Can_ModuleID,
  Can_StatusType    *Can_Status
){
  Can_ReturnType ret_val = CAN_OK;
  volatile Can_RegsType *Can_Regs;

  /* Check for valid pointer */
  if (Can_Status == NULL){
    ret_val = CAN_NOT_OK;
  }else{
    /* Get the actual register base address */
    Can_Regs = Can_RegsBase[Can_ModuleID];
    /* Get the Receiver Active Status */
    if (((Can_Regs->ctl0) & CAN_CTL0_RXACT)==CAN_CTL0_RXACT){
      Can_Status->Can_Flags.Can_RecAct = CAN_TRUE;
    }else{
      Can_Status->Can_Flags.Can_RecAct = CAN_FALSE;
    }
    /* Get the Synchronization status */
    if (((Can_Regs->ctl0) & CAN_CTL0_SYNCH)==CAN_CTL0_SYNCH){
      Can_Status->Can_Flags.Can_Synch = CAN_TRUE;
    }else{
      Can_Status->Can_Flags.Can_Synch = CAN_FALSE;
    }
    /* Get the Overrun status */
    if (((Can_Regs->rflg) & CAN_RFLG_OVRIF)==CAN_RFLG_OVRIF){
      Can_Status->Can_Flags.Can_OvrRun = CAN_TRUE;
      /* Clear the status flag */
      Can_Regs->rflg = CAN_RFLG_OVRIF;
    }else{
      Can_Status->Can_Flags.Can_OvrRun = CAN_FALSE;
    } 
    /* Get the Rx counter status */
    Can_Status->Can_Flags.Can_OvrRun = ((Can_Regs->rflg) & CAN_RFLG_RSTAT)>>4;
    /* Get the Tx counter status */
    Can_Status->Can_Flags.Can_OvrRun = ((Can_Regs->rflg) & CAN_RFLG_TSTAT)>>2;
    /* Get the module status */   
    Can_Status->Can_DriverStatus = Can_DriverStatus[Can_ModuleID];
  }
  return (ret_val);    
}


/***************************************************************************//*!
*
*    @can_function    Can_SetMode
*    @can_sync_async  Synchronous
*    @can_reentrancy  Non Reentrant
*    @param[in]       Can_ModuleID - CAN module to be used
*                     Can_Mode - driver mode to be introduced.
*    @param[out]      None
*    @return          Can_ReturnType - return value of the CAN driver API (CAN_OK / CAN_NOT_OK).
*    @brief           Function set status of Can_ModuleID module and CAN driver.
*    @attention       N/A
*
*******************************************************************************/

Can_ReturnType Can_SetMode(
  Can_ModuleIDType  Can_ModuleID,
  Can_ModeType      Can_Mode
){
  Can_ReturnType ret_val = CAN_OK;
  volatile Can_RegsType *Can_Regs;
  uint16_t timer = CAN_MAX_LOOP_COUNT;

  /* Check for valid mode */
  if ((Can_Mode != CAN_MODE_SLEEP)&&(Can_Mode != CAN_MODE_INIT)&&(Can_Mode != CAN_MODE_RUN)){
    ret_val = CAN_NOT_OK;
  }else{
    /* Get the actual register base address */
    Can_Regs = Can_RegsBase[Can_ModuleID];
    switch(Can_Mode){

      case CAN_MODE_RUN:
        Can_Regs->ctl1 &= ~CAN_CTL1_LISTEN; /* Clear listen only mode */
        Can_Regs->ctl0 &= ~CAN_CTL0_INITRQ; /* Leave init mode */
        while ((((timer>0)&(Can_Regs->ctl1 & CAN_CTL1_INITAK))!=0x00)){
          timer--;
        }
        Can_Regs->rier |= CAN_RIER_RXFIE; /* Enable Rx interrupts */
        /* Change driver status to RUN */
        Can_DriverStatus[Can_ModuleID] = CAN_RUN;
        if (timer == 0){
          /* Timer overflow */
          ret_val = CAN_NOT_OK;
        }
      break;

      case CAN_MODE_SLEEP:
        /* Wait for end of reception */
        while((((timer>0)&(Can_Regs->ctl1 & CAN_CTL0_RXACT))!=CAN_CTL0_RXACT)){
          timer--; 
        }
        if (timer == 0){
          /* Timer overflow */
          ret_val = CAN_NOT_OK;
        }
        /* Enable wakeup interrupt */
        Can_Regs->rier |= CAN_RIER_WUPIE;
        /* Enable wakeup */
        Can_Regs->ctl0 |= CAN_CTL0_WUPE;
        /* Request sleep mode transition */
        Can_Regs->ctl0 |= CAN_CTL0_SLPRQ;
        /* Wait to process the sleep mode transition */
        while ((((timer>0)&(Can_Regs->ctl1 & CAN_CTL1_SLPAK))!=CAN_CTL1_SLPAK)){
          timer--; 
        }
        /* Change driver status to sleep */
        Can_DriverStatus[Can_ModuleID] = CAN_SLEEP;
        if (timer == 0){
          /* Timer overflow */
          ret_val = CAN_NOT_OK;
        }
      break;
      
      case CAN_MODE_INIT:
        /* Disable Wakeup */
        Can_Regs->ctl0 &= ~CAN_CTL0_WUPE;
        /* Disable wakeup interrupt */
        Can_Regs->rier &= ~CAN_RIER_WUPIE;
        /* Leave sleep mode */
        Can_Regs->ctl0 &= ~CAN_CTL0_SLPRQ;
        /* Wait to process the run mode transition */
        while ((((timer>0)&(Can_Regs->ctl1 & CAN_CTL1_SLPAK))==0x00)){
          timer--; 
        }
        if (timer == 0){
          /* Timer overflow */
          ret_val = CAN_NOT_OK;
        }
        /* Change driver status to init */
        Can_DriverStatus[Can_ModuleID] = CAN_INIT;
      break;
      
      default:
        ret_val = CAN_NOT_OK;
      break;
    }
  }  
  return (ret_val);    
}


/***************************************************************************//*!
*
*    @can_function    Can_WakeupNotification
*    @can_sync_async  Synchronous
*    @can_reentrancy  Non Reentrant
*    @param[in]       None
*    @param[out]      None
*    @return          Can_ReturnType - return value of the CAN driver API (CAN_OK / CAN_NOT_OK).
*    @brief           Wakeup notification function.
*    @attention       N/A
*
*******************************************************************************/

void Can_Wakeup(
  Can_ModuleIDType  Can_ModuleID  
){
  volatile Can_RegsType *Can_Regs;
  
  /* Get the actual register base address */
  Can_Regs = Can_RegsBase[Can_ModuleID];
  /* Disable wakeup interrupt */
  Can_Regs->rier &= ~CAN_RIER_WUPIE;
  /* Clear wakeup flag */
  Can_Regs->rflg |= CAN_RFLG_WUPIF;
  /* Disable wakeup over CAN */
  Can_Regs->ctl0 &= ~CAN_CTL0_WUPE;
  /* Clear sleep request */
  Can_Regs->ctl0 &= ~CAN_CTL0_SLPRQ;
  /* Call Wakeup notification function */
  Can_WakeupNotification(Can_ModuleID);
  /* Set the module status back to init state */
  Can_DriverStatus[Can_ModuleID] = CAN_INIT;    
}
