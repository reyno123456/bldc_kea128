/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     BLDCSensorless.h
*
* @author   B06050
*
* @version  1.0.5.0
*
* @date     Aug-29-2014
*
* @brief    Sensorless BLDC Motor Control Application for Kinetis KEA128.
*
*******************************************************************************/
#ifndef BLDCSENSORLESS_H_
#define BLDCSENSORLESS_H_

/*******************************************************************************
* Includes
*******************************************************************************/
//#include "gdflib.h"
//#include "gflib.h"
//#include "mlib.h"

/*******************************************************************************
* Constants and macros
*******************************************************************************/
#define APP_INIT                0   /* Application states */
#define APP_CALIB               1
#define APP_ALIGNMENT           2
#define APP_START               3
#define APP_RUN                 4
#define APP_STOP                5
#define APP_FAULT               6

#define ROTATION_DIR_CW         0
#define ROTATION_DIR_CCW        1

#define SFRACT_MAX          (0.999969482421875)
#define SFRACT_MIN          (-1.0)

#define FRAC16(x)           ((int32_t) ((x < SFRACT_MAX) ? ((x >= SFRACT_MIN) ? (x*32768.0) : INT16_MIN) : INT16_MAX))

#define FRACT_MAX           (0.9999999995343387126922607421875)
#define FRACT_MIN           (-1.0)
#define FRAC32(x)           ((int32_t) ((x < FRACT_MAX) ? ((x >= FRACT_MIN) ? (x*2147483648.0) : INT32_MIN) : INT32_MAX))


/*******************************************************************************
* Types
*******************************************************************************/
typedef void (*tPointerFcn)(void);  /* pointer to a function */

typedef union {
    uint16_t R;
    struct {
        uint16_t Alignment:1;
        uint16_t Sensorless:1;
        uint16_t StallCheckReq:1;
        uint16_t EnableCMT:1;
        uint16_t AfterCMT:1;
        uint16_t CloseLoop:1;
        uint16_t NewZC:1;
        uint16_t AdcSaved:1;
        uint16_t CurrentLimiting:1;
        uint16_t Fault:1;
        uint16_t Freewheeling:1;
        uint16_t Reserved:5;
    }B;
}tDriveStatus;

typedef union {
    uint8_t R;
    struct {
        uint8_t OverDCBusCurrent:1;
        uint8_t OverDCBusVoltage:1;
        uint8_t UnderDCBusVoltage:1;
        uint8_t PreDriverError:1;
        uint8_t StallError:1;
        uint8_t Reserved:3;
    }B;
}tFaultStatus;

typedef struct {
    int16_t BEMFVoltage;
    int16_t DCBVVoltage;
    int16_t DCBIVoltage;
    int16_t DCBIOffset;
}tADCresults;

typedef struct{
  int16_t		f16PropGain;		/*!< Proportional Gain, fractional format normalized to fit into (-2\f$^{15}\f$, 2\f$^{15}\f$-1). */
  int16_t		f16IntegGain;		/*!< Integral Gain, fractional format normalized to fit into  (-2\f$^{15}\f$, 2\f$^{15}\f$-1). */
  int16_t		s16PropGainShift;	/*!< Proportional Gain Shift, integer format [-15, 15]. */
  int16_t		s16IntegGainShift;	/*!< Integral Gain Shift, integer format [-15, 15]. */
  int16_t		f16LowerLimit;		/*!< Lower Limit of the controller, fractional format normalized to fit into (-2\f$^{15}\f$, 2\f$^{15}\f$-1). */
  int16_t		f16UpperLimit;		/*!< Upper Limit of the controller, fractional format normalized to fit into (-2\f$^{15}\f$, 2\f$^{15}\f$-1). */
  int32_t		f32IntegPartK_1;	/*!< State variable integral part at step k-1. */
  int16_t		f16InK_1;			/*!< State variable input error at step k-1. */
  uint16_t		u16LimitFlag;		/*!< Limitation flag, if set to 1, the controller output has reached either the UpperLimit or LowerLimit. */
}t_xl_pi_control;

typedef struct{
  int32_t f32State; /*!< Ramp state value. */
  int32_t f32RampUp; /*!< Ramp up increment coefficient. */
  int32_t f32RampDown; /*!< Ramp down increment (decrement) coefficient. */
}t_xl_ramp;



/*******************************************************************************
* Global function prototypes
*******************************************************************************/
int main(void);
void NMI_ISR(void);
//void Can_RxNotification(Can_ModuleIDType Can_ModuleID, Can_IDType Can_ID, Can_DataLengthType Can_DataLength, uint8_t *Can_Data);
void ICSIRCLK_Trim(void);

/*******************************************************************************
* Local function prototypes
*******************************************************************************/

//Application scales                    
#define I_MAX                           (41.667)
#define U_DCB_MAX                       (25.0)
#define N_MAX                           (10000.0)
#define I_DCB_OVERCURRENT               FRAC16(0.359997120023)
#define U_DCB_UNDERVOLTAGE              FRAC16(0.360000000000)
#define U_DCB_OVERVOLTAGE               FRAC16(0.800000000000)
#define I_DCB_LIMIT                     FRAC16(0.071999424005)
#define U_DCB_TRIP                      FRAC16(0.680000000000)
#define N_NOM                           FRAC16(0.935000000000)
#define I_PH_NOM                        FRAC16(0.139198886409)
#define U_PH_NOM                        FRAC16(0.480000000000)
//Mechanical Alignment                  
#define ALIGN_VOLTAGE                   FRAC16(0.125000000000)
#define ALIGN_DURATION                  (200)

//BLDC Control Loop                     
//----------------------------------------------------------------------
//Loop sample time                      = 0.001 [sec]
//----------------------------------------------------------------------
//Control loop limits                   
#define CTRL_LOOP_LIM_HIGH              FRAC16(0.900000000000)
#define CTRL_LOOP_LIM_LOW               FRAC16(0.100000000000)

//Speed Controller - Parallel type      
#define SPEED_LOOP_KP_GAIN              FRAC16(0.514714422649)
#define SPEED_LOOP_KP_SHIFT             (-8)
#define SPEED_LOOP_KI_GAIN              FRAC16(0.643393028311)
#define SPEED_LOOP_KI_SHIFT             (-7)

//Speed ramp increments                 
#define SPEED_LOOP_RAMP_UP              FRAC32(0.003000000000)
#define SPEED_LOOP_RAMP_DOWN            FRAC32(0.002000000000)

//Torque Controller - Parallel type     
#define TORQUE_LOOP_KP_GAIN             FRAC16(0.666672000000)
#define TORQUE_LOOP_KP_SHIFT            (-3)
#define TORQUE_LOOP_KI_GAIN             FRAC16(0.800006400000)
#define TORQUE_LOOP_KI_SHIFT            (-5)
#define TORQUE_LOOP_MAF                 (5)

//Sensorless Control Module             
//----------------------------------------------------------------------
//Timer Frequency                       = 750000 [Hz]
//----------------------------------------------------------------------
#define N_MIN                           FRAC16(0.080000000000)
#define N_START_TRH                     FRAC16(0.080000000000)
#define STARTUP_CMT_CNT                 (8)
#define STARTUP_CMT_PER                 (30000)
#define CMT_T_OFF                       FRAC16(0.250000000000)
#define FREEWHEEL_T_LONG                (1000)
#define FREEWHEEL_T_SHORT               (500)
#define SPEED_SCALE_CONST               (1125)
#define CMT_PER_MIN                     (188)
#define START_CMT_ACCELER               FRAC16(0.694747747187)
#define INTEG_TRH                       (0)

//FreeMASTER Scale Variables            
//----------------------------------------------------------------------
//Note: Scaled at input = 1000          
//----------------------------------------------------------------------
#define FM_I_SCALE                      (41667)
#define FM_U_DCB_SCALE                  (25000)
#define FM_N_SCALE                      (10000000)

#define ADVANCE_ANGLE   FRAC16(0.3815)      /* 12500 */

/* PWM period in FTM2 ticks */
#define PWM_MODULO      2400    /* 20kHz/50us @48MHz system clock */

/* Duty cycle limit for DC bus current measurement */
#define DC_THRESHOLD    400

/* PWM dead time in FTM2 ticks (if less than MC33937A dead time, then mask by MC33937A dead time) */
#define PWM_DEADTIME    19      /* ~400ns @48MHz FTM2 clock */

/* > MC33937 dead time + MC33937 high side turn ON time + phase ringing (in BUS/4 clock ticks) */
#define DELAY           18      /* 3us @ 6MHz */

/* DC Bus overcurrent limit (A) */
#define DCBUS_OVERCURR_THR      (((float)I_DCB_OVERCURRENT / (float)0x8000) * I_MAX)
/* DC Bus undervoltage limit (V) */
#define DCBUS_UNDERVOLT_THR     (((float)U_DCB_UNDERVOLTAGE / (float)0x8000) * U_DCB_MAX)

/* Speed increase step [RPM] */
#define SPEED_RPM_INC   400
/* Speed decrease step [RPM] */
#define SPEED_RPM_DEC   400
/* Maximal speed [RPM] */
#define SPEED_RPM_MAX   8000

/* Maximum number of stall check errors */
#define STALLCHECK_MAX_ERRORS		6
/* Minimal stall commutation period */
#define STALLCHECK_MIN_CMT_PERIOD	94	/* 20KRPM => 125us => 93.75 @750kHz */

/* User switch debounce timeout [ms] */
#define SW_PRESS_DEBOUNCE   75
/* User switch input blocking delay [ms] */
#define SW_PRESS_OFF        250

/********************** HW specific constants and macros **********************/
/* DC Bus voltage divider value */
#define DCBV_R_DIV              5.0
/* DC Bus current amplifier (MC33937A) gain */
#define DCBI_GAIN               12.0
/* DC Bus current amplifier (MC33937A) AMP_P input DC bias [V] */
#define DCBI_P_BIAS             2.5
/* DC Bus shunt resistor resistance [Ohm] */
#define DCBI_SHUNT_R            0.01
/* VDDA voltage [V] */
#define VDDA_VOLTAGE            5.0

/* Scaled speed increase step */
#define SPEED_INC   FRAC16(SPEED_RPM_INC/N_MAX)
/* Scaled speed decrease step */
#define SPEED_DEC   FRAC16(SPEED_RPM_DEC/N_MAX)
/* Scaled maximal speed */
#define SPEED_MAX   FRAC16(SPEED_RPM_MAX/N_MAX)

/* Analog comparators thresholds: DACVAL = (64 x Vthr/VDDA) - 1 */
/* "- 1" omitted for rounding */
#define ACMP0_DACVAL            (uint8_t)((64.0 * ((DCBUS_OVERCURR_THR * DCBI_SHUNT_R * DCBI_GAIN) + DCBI_P_BIAS)) / VDDA_VOLTAGE)
#define ACMP1_DACVAL            (uint8_t)((64.0 * (DCBUS_UNDERVOLT_THR / DCBV_R_DIV)) / VDDA_VOLTAGE)

#define REYNO_TEST
#define XL_LOWEST_DUTY 300
#define XL_HIGHEST_DUTY 2300

#endif /* BLDCSENSORLESS_H_ */
