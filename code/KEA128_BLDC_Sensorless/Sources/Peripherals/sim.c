/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     sim.c
*
* @author   B06050
*
* @version  1.0.4.0
*
* @date     Aug-27-2014
*
* @brief    System Integration Module SW module source file.
*
*******************************************************************************/
#include "sim.h"

/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void SIM_Init(void)
*
* Description: Initializes the SIM module.
*
*******************************************************************************/
void SIM_Init(void)
{
    /* System options 0:
       - FTM2 trigger delay 0 bus clocks
       - FTM2 init trigger with 8-bit programmable delay
       - Bus clock output div. by 4
       - RTC overflow is connected to FTM1 channel 1 input
       - SWD & RESET enabled
       - NMI disabled
    */
    SIM_SOPT0 = SIM_SOPT0_DELAY(0) | SIM_SOPT0_ADHWT(2) | SIM_SOPT0_BUSREF(2) | \
                SIM_SOPT0_RTCC_MASK | SIM_SOPT0_SWDE_MASK | SIM_SOPT0_RSTPE_MASK;

    /* Pin selection 0:
       - SPI0_SCK, SPI0_MOSI, SPI0_MISO, and SPI0_PCS are mapped on PTE0, PTE1,
         PTE2, and PTE3
       - UART0_RX and UART2_TX are mapped on PTA2 and PTA3
    */
    SIM_PINSEL0 = SIM_PINSEL_UART0PS_MASK | SIM_PINSEL_SPI0PS_MASK;

    /* Pin selection 1:
       - CAN_TX on PTC7, CAN_RX on PTC6
       - UART2_TX on PTI1, UART2_RX on PTI0
       - FTM2 CH5 mapped on PTG7
       - FTM2 CH4 mapped on PTG6
       - FTM2 CH3 mapped on PTG5
       - FTM2 CH2 mapped on PTG4
       - FTM2 CH1 mapped on PTF1
       - FTM2 CH0 mapped on PTF0
    */
    SIM_PINSEL1 = SIM_PINSEL1_UART2PS_MASK | SIM_PINSEL1_FTM2PS5_MASK | \
                  SIM_PINSEL1_FTM2PS4_MASK | SIM_PINSEL1_FTM2PS3(2) | \
                  SIM_PINSEL1_FTM2PS2(2) | SIM_PINSEL1_FTM2PS1(2) | \
                  SIM_PINSEL1_FTM2PS0(2);

    /* Enable bus clock to ACMP1, ACMP0, ADC, SPI0, MSCAN, SWD, FLASH, FTM2,
       FTM1, FTM0, PIT, RTC */
    SIM_SCGC = SIM_SCGC_ACMP1_MASK | SIM_SCGC_ACMP0_MASK | SIM_SCGC_ADC_MASK   | \
               SIM_SCGC_UART2_MASK | SIM_SCGC_SPI0_MASK  | SIM_SCGC_MSCAN_MASK | \
               SIM_SCGC_SWD_MASK   | SIM_SCGC_FLASH_MASK | SIM_SCGC_FTM2_MASK  | \
               SIM_SCGC_FTM1_MASK  | SIM_SCGC_FTM0_MASK  | SIM_SCGC_PIT_MASK   | \
               SIM_SCGC_RTC_MASK ;

    /* Clock dividers:
       - Core/system clock same as ICSOUTCLK
       - Bus/FLASH div. by 2 from core/system clock
       - Timers clock same as ICSOUTCLK
    */
    SIM_CLKDIV = SIM_CLKDIV_OUTDIV2_MASK;
}
