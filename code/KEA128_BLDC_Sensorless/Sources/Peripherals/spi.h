/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     spi.h
*
* @author   B06050
*
* @version  1.0.2.0
*
* @date     Jul-3-2014
*
* @brief    Oscillator SW module header file.
*
*******************************************************************************/
#ifndef SPI_H_
#define SPI_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "derivative.h"

/*******************************************************************************
* Constants and macros
*******************************************************************************/
/* SPIBaudRate = Bus_clock/BaudRateDivisor */
/* BaudRateDivisor = (SPPR+1)*2^(SPR+1) */
/* For SPIBaudRate = 3.0MHz and BusClock = 24MHz, BaudRateDivisor = 8 */
#define SPI_0_SPPR   0      /* (SPPR+1) = 1 */
#define SPI_0_SPR    2      /* 2^(SPR+1) = 8 */


/*******************************************************************************
* Global function prototypes
*******************************************************************************/
void SPI0_Init(void);
uint8_t SPI_0_SendByte(uint8_t ui8Data);
uint16_t SPI_0_SendWord(uint16_t ui16Data);

#endif /* SPI_H_ */
