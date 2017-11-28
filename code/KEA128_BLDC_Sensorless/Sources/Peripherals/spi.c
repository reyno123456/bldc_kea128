/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     spi.c
*
* @author   B06050
*
* @version  1.0.3.0
*
* @date     Jul-3-2014
*
* @brief    Serial Peripheral Interface SW module source file.
*
*******************************************************************************/
#include "spi.h"

/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void SPI_0_Init(void)
*
* Description:  This function initializes the SPI0 module.
*
* Note:         This function must be called first, before a call to any other
*               SPI0 related function within this SW module.
*
*******************************************************************************/
void SPI0_Init(void)
{
    SPI0_C1 = 0;        /* SPI module disabled */
    SPI0_C2 = 0x00;     /* Slave select output pin not used by SPI module */

    (void)SPI0_S;       /* Read the status register and */
    (void)SPI0_D;       /* Read the data register to clear the status flags */

    /* Baud rate prescaler and divider */
    SPI0_BR = (SPI_0_SPPR << SPI_BR_SPPR_SHIFT) | SPI_0_SPR;

    SPI0_C1 = 0x54;     /* SPI enable, master mode, sampling at falling edges */
}

/*******************************************************************************
*
* Function: uint8_t SPI_0_SendByte(uint8_t data)
*
* Description:  This function transmits 8-bit data on the SPI0 bus
*               and returns 8-bit data received from the slave device.
*
* Param[in]:    data        8-bit data to be transmitted
*
* Return:       uint8_t     received 8-bit data from the slave device
*
*******************************************************************************/
uint8_t SPI_0_SendByte(uint8_t ui8Data)
{
    /* Wait for empty data register */
    while ((SPI0_S & SPI_S_SPTEF_MASK) == 0)
    {
    }
    SPI0_D = ui8Data;   /* Load byte in data transmission register */

    /* Wait for the end of the transfer */
    while ((SPI0_S & SPI_S_SPRF_MASK) == 0)
    {
    }

    return SPI0_D;
}

/*******************************************************************************
*
* Function: uint16_t SPI_0_SendWord(uint16_t data)
*
* Description:  This function transmits 16-bit data on the SPI0 bus
*               and returns 16-bit data received from the slave device.
*
* Param[in]:    data        16-bit data to be transmitted
*
* Return:       uint16_t    received 16-bit data from the slave device
*
*******************************************************************************/
uint16_t SPI_0_SendWord(uint16_t ui16Data)
{
    uint16_t ui16OutData;

    /* Wait for empty data register */
    while ((SPI0_S & SPI_S_SPTEF_MASK) == 0)
    {
    }
    /* Load first 8-bit word into data transmission register */
    SPI0_D = (uint8_t)(ui16Data >> 8);

    /* Wait for the end of the transfer */
    while ((SPI0_S & SPI_S_SPRF_MASK) == 0)
    {
    }
    ui16OutData = SPI0_D;                 /* Store the second 8-bit word */

    /* Wait for empty data register */
    while ((SPI0_S & SPI_S_SPTEF_MASK) == 0)
    {
    }

    /* Load the second 8-bit word into data transmission register */
    SPI0_D = (uint8_t)ui16Data;

    /* Wait for the end of the transfer */
    while ((SPI0_S & SPI_S_SPRF_MASK) == 0)
    {
    }
    ui16OutData |= (uint16_t)(SPI0_D << 8); /* Store the first 8-bit word */

    return ui16OutData;
}
