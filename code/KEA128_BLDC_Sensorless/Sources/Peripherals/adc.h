/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     adc.h
*
* @author   B06050
*
* @version  1.0.2.0
*
* @date     Jul-3-2014
*
* @brief    Analog-to-Digital Converter SW module header file.
*
*******************************************************************************/
#ifndef ADC_H_
#define ADC_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "derivative.h"

/*******************************************************************************
* Constants and macros
*******************************************************************************/
/* Analog channel assignments */
#define ADC_DCBV    11      /* DC bus voltage */
#define ADC_DCBI    1       /* DC bus current */
#define ADC_BEMFA   6       /* Phase A voltage */
#define ADC_BEMFB   7       /* Phase B voltage */
#define ADC_BEMFC   12      /* Phase C voltage */

/*******************************************************************************
* Global function prototypes
*******************************************************************************/
void ADC_Init(void);
void ADC_StartSingleConversion(uint8_t ui8Channel);
void ADC_EnableTrigSeq(uint8_t *pChannelList, uint8_t ui8FifoDepth);

#endif /* ADC_H_ */
