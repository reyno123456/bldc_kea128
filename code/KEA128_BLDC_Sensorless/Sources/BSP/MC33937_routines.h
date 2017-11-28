/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     MC33937_routines.h
*
* @author   B06050
*
* @version  1.0.3.0
*
* @date     Jul-3-2014
*
* @brief    MC33937A MOSFET pre-driver configuration header file.
*
*******************************************************************************/
#ifndef MC33937_ROUTINES_H_
#define MC33937_ROUTINES_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "spi.h"
#include "gpio.h"
#include "pit.h"

/*******************************************************************************
* Constants and macros
*******************************************************************************/
/***************************** Configuration section **************************/
#define KEA128_PIT_FREQ     24000000.0  /* KEA128 BUSclk frequency (Hz) */
#define DEADTIME            600         /* Desired dead time (ns) */

/***************************** DO NOT MODIFY section **************************/
/* Calculation of PIT rate */
#define DEADTIME_CALC   ((DEADTIME * MC33937_CALIB_DIV) * (KEA128_PIT_FREQ / 1000000000.0))

#define MASK0_CMD       0x20    /* MASK0 command */
#define MASK1_CMD       0x30    /* MASK1 command */

#define CLINT0_CMD      0x6F    /* CLINT0 command */
#define CLINT1_CMD      0x7F    /* CLINT1 command */

#define MODE_CMD        0x49    /* MODE command */

#define DEADTIME_CMD    0x81    /* DEADTIME command */

#define NULL0_CMD       0x00    /* NULL0 command */
#define NULL1_CMD       0x01    /* NULL1 command */
#define NULL2_CMD       0x02    /* NULL2 command */
#define NULL3_CMD       0x03    /* NULL3 command */

#define MC33937_CALIB_DIV       16      /* MC33937A calibration divider */

/* 15ms time to stabilize power supply voltage (s) */
#define MC33937_POWER_UP_TIME   (0.015 * KEA128_PIT_FREQ)

/*******************************************************************************
* Types
*******************************************************************************/
/* Structure containing status register 0 image of the MC33937A device */
typedef union
{
    uint8_t R;
    struct
    {
        uint8_t tlim:1;         /* TLIM flag detected on any channel (LSB) */
        uint8_t desat:1;        /* DESAT flag detected on any channel */
        uint8_t lowvls:1;       /* Low VLS voltage flag */
        uint8_t overcurrent:1;  /* Overcurrent event flag */
        uint8_t phase_err:1;    /* Phase error flag */
        uint8_t framing_err:1;  /* Framing error flag */
        uint8_t write_err:1;    /* Write Error After the Lock flag */
        uint8_t reset_event:1;  /* reset event flag, is set upon exiting /RST (MSB) */
    } B;
}SR0_T;

/* Structure containing status register 1 image of the MC33937A device */
typedef union
{
    uint8_t R;
    struct
    {
        uint8_t lockbit:1;      /* LockBit indicates the IC regs are locked (LSB) */
        uint8_t fullon:1;       /* present status of FULLON MODE */
        uint8_t reserved2:1;    /* Reserved */
        uint8_t deadtime_cal:1; /* Deadtime calibration occurred */
        uint8_t calib_overfl:1; /* flag for a Deadtime Calibration Overflow */
        uint8_t zds:1;          /* Zero deadtime is commanded */
        uint8_t desat_mode:1;   /* current state of the Desaturation/Phase Error turn-off mode */
        uint8_t reserved7:1;    /* Reserve (MSB) */
    } B;
}SR1_T;

/* Structure containing status register 2 image of the MC33937A device */
typedef union
{
    uint8_t R;
    struct
    {
        uint8_t mask0_0 :1;     /* status of the MASK0.0 bit (LSB) */
        uint8_t mask0_1 :1;     /* status of the MASK0.1 bit */
        uint8_t mask0_2 :1;     /* status of the MASK0.2 bit */
        uint8_t mask0_3 :1;     /* status of the MASK0.3 bit */
        uint8_t mask1_0 :1;     /* status of the MASK1.0 bit */
        uint8_t mask1_1 :1;     /* status of the MASK1.1 bit */
        uint8_t mask1_2 :1;     /* status of the MASK1.2 bit */
        uint8_t mask1_3 :1;     /* status of the MASK1.3 bit (MSB) */
    } B;
}SR2_T;

/* Structure containing status registers image of the MC33937A device */
typedef struct
{
    SR0_T   sr0;    /* status register 0 */
    SR1_T   sr1;    /* status register 1 */
    SR2_T   sr2;    /* status register 2 */
    uint8_t sr3;    /* status register 3 */
}MC33937_SR_T;

/*******************************************************************************
* Global function prototypes
*******************************************************************************/
void MC33937_ClearFaults(void);
void MC33937_Enable(void);
void MC33937_Disable(void);
void MC33937_Reset(void);
MC33937_SR_T MC33937_ReadStatus(void);
uint16_t MC33937_DeadtimeSetup(void);
uint8_t MC33937_Config(void);

#endif /* MC33937_ROUTINES_H_ */
