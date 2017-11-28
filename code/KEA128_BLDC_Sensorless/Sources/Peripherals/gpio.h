/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     gpio.h
*
* @author   B06050
*
* @version  1.0.3.0
*
* @date     Jul-3-2014
*
* @brief    General-Purpose Input/Output SW module header file.
*
*******************************************************************************/
#ifndef GPIO_H_
#define GPIO_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "derivative.h"

/*******************************************************************************
* Constants and macros
*******************************************************************************/
/* Serial Peripheral Interface SPI0  */
#define SCLK0                       /* PTE2/SCK0  */
#define MOSI0                       /* PTE3/MOSI0 */
#define MISO0                       /* PTE4/MISO0 */

/* SBC - MC33903S pin assignments */
#define MC33903_SCLK                /* PTE0/SCK0     */
#define MC33903_MOSI                /* PTE1/MOSI0    */
#define MC33903_MISO                /* PTE2/MISO0    */
#define MC33903_CS                  /* PTC5          */
#define MC33903_INT                 /* PTD1          */
#define MC33903_RST                 /* RESET         */
#define MC33903_LIN_RX              /* PTA2/UART0_RX */
#define MC33903_LIN_TX              /* PTA3/UART0_TX */
#define MC33903_CAN_RX              /* PTC6/CAN0_RX  */
#define MC33903_CAN_TX              /* PTC7/CAN0_TX  */
#define UART2_RX                    /* PTI0/UART2_RX */
#define UART2_TX                    /* PTI1/UART2_TX */

#define MC33903_CS_LOW              GPIOA_PCOR = 0x00200000
#define MC33903_CS_HIGH             GPIOA_PSOR = 0x00200000

#define MC33937_CS_LOW              GPIOB_PCOR = 0x20000000
#define MC33937_CS_HIGH             GPIOB_PSOR = 0x20000000
#define MC33937_EN_SET              GPIOA_PSOR = 0x01000000
#define MC33937_EN_CLEAR            GPIOA_PCOR = 0x01000000
#define MC33937_RST_SET             GPIOB_PSOR = 0x80000000
#define MC33937_RST_CLEAR           GPIOB_PCOR = 0x80000000

#define MC33937_INT                 ((GPIOB_PDIR & 0x40000000) >> 30)

#define BRAKE_PWM_ON                GPIOA_PSOR = 0x00002000
#define BRAKE_PWM_OFF               GPIOA_PCOR = 0x00002000

#define LED_GREEN_ON                GPIOB_PCOR = 0x01000000
#define LED_GREEN_OFF               GPIOB_PSOR = 0x01000000
#define LED_GREEN_TOGGLE            GPIOB_PTOR = 0x01000000
#define LED_RED_ON                  GPIOB_PCOR = 0x02000000
#define LED_RED_OFF                 GPIOB_PSOR = 0x02000000
#define LED_RED_TOGGLE              GPIOB_PTOR = 0x02000000

#define SW_RUN_R                    ((GPIOC_PDIR & 0x00000020) >> 5)
#define SW_RUN_L                    ((GPIOC_PDIR & 0x00000040) >> 6)

#define SWB0                        ((GPIOA_PDIR & 0x20000000) >> 29)
#define SWB1                        ((GPIOA_PDIR & 0x40000000) >> 30)

/*******************************************************************************
* Global function prototypes
*******************************************************************************/
void GPIO_Init(void);

#endif /* GPIO_H_ */
