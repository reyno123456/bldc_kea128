/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     adc.c
*
* @author   B06050
*
* @version  1.0.2.0
*
* @date     Jul-3-2014
*
* @brief    Analog-to-Digital Converter SW module source file.
*
*******************************************************************************/
#include "adc.h"

/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void ADC_Init(void)
*
* Description:  This function initializes the ADC module.
*
*******************************************************************************/
void ADC_Init(void)
{
    /* Clock div. by 2, 12-bit resolution, alternate clock (OSC_OUT) */
    ADC_SC3 = ADC_SC3_ADIV(1) | ADC_SC3_MODE(2) | ADC_SC3_ADICLK(2);
    /* Disable I/O control on analog inputs S12..SE11, SE7..SE4, SE1..SE0 */
    ADC_APCTL1 = ADC_APCTL1_ADPC(0x18F3);  // select the arrcoding adc channel
}

/*******************************************************************************
*
* Function: void ADC_StartSingleConversion(uint8_t ui8Channel)
*
* Description:  This function disables triggered conversions and starts single
*               channel conversion (12-bit). Conversion complete interrupt is
*               disabled.
*
* Param[in]:    ui8Channel  ADC channel number
*
*******************************************************************************/
void ADC_StartSingleConversion(uint8_t ui8Channel)
{
    /* FIFO disabled */
    ADC_SC4 = 0x00000000;
    /* Software trigger selected */
    ADC_SC2 = 0x00000000;
    /* Start conversion of the selected channel */
    ADC_SC1 = ADC_SC1_ADCH(ui8Channel);
}

/*******************************************************************************
*
* Function: void ADC_EnableTrigSeq(uint8_t *pChannelList, uint8_t ui8FifoDepth)
*
* Description:  This function enables conversion sequence triggered HW trigger
*               input. Conversion complete complete interrupt enabled.
*
* Param[in]:    *pChannelList   Pointer to the list of channels
*               ui8FifoDepth    ADC channel FIFO depth (number of channels)
*
*******************************************************************************/
void ADC_EnableTrigSeq(uint8_t *pChannelList, uint8_t ui8FifoDepth)
{
    uint8_t ui8ChannelCnt;

    /* Mask HW trigger if data FIFO not empty */
    ADC_SC5 = ADC_SC5_HTRGMASKSEL_MASK;

    /* HW trigger pulse triggers multiple conversions in FIFO mode */
    ADC_SC4 = ADC_SC4_HTRGME_MASK | ADC_SC4_AFDEP(ui8FifoDepth - 1);

    /* HW trigger selected */
    ADC_SC2 = ADC_SC2_ADTRG_MASK;

    /* Initialize channel FIFO */
    for(ui8ChannelCnt=0; ui8ChannelCnt<ui8FifoDepth-1; ui8ChannelCnt++)
    {
        ADC_SC1 = *(pChannelList + ui8ChannelCnt);
    }

    /* Define the last channel in FIFO, enable conversion complete interrupt */
    ADC_SC1 = ADC_SC1_AIEN_MASK | *(pChannelList + ui8ChannelCnt);
}
