/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     gpio.c
*
* @author   B06050
*
* @version  1.0.3.0
*
* @date     Jul-3-2014
*
* @brief    General-Purpose Input/Output SW module source file.
*
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include "gpio.h"

/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void GPIO_Init(void)
*
* Description:  This function initializes the GPIO module.
*
*******************************************************************************/
void GPIO_Init(void)
{
    /* MC33903_CS */
    GPIOA_PSOR = 0x00200000;    /* Set PTC5 logic high */
    GPIOA_PDDR |= 0x00200000;   /* PTC5 configured as output */

    /* MC33937A_CS */
    GPIOB_PSOR = 0x20000000;    /* Set PTH5 logic high */
    GPIOB_PDDR |= 0x20000000;   /* PTH5 configured as output */

    /* MC33937A_EN */
    GPIOA_PCOR = 0x01000000;    /* Set PTD0 logic low */
    GPIOA_PDDR |= 0x01000000;   /* PTD0 configured as output */

    /* MC33937A_RST */
    GPIOB_PCOR = 0x80000000;    /* Set PTH7 logic low */
    GPIOB_PDDR |= 0x80000000;   /* PTH7 configured as output */

    /* LEDs */
    GPIOB_PSOR = 0x03000000;    /* Set PTH1, PTH0 logic high */
    GPIOB_PDDR |= 0x03000000;   /* PTH1, PTH0 configured as output */

    /* Braking MOSFET gate drive */
    GPIOA_PCOR = 0x00002000;    /* Set PTB5 logic low */
    GPIOA_PDDR |= 0x00002000;   /* PTB5 configured as output */
    PORT_HDRVE = 0x00000002;    /* Enable PTB5 high current drive */

    /* User switches */
    GPIOA_PIDR &= ~0x60000000;  /* PTD5, PTD6 configured as input */

    /* Rotation direction SPDT switch */
    GPIOC_PIDR &= ~0x00000060;  /* PTI5, PTI6 configured as input */
}
