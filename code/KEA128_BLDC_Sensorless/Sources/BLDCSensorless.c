/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     BLDCSensorless.c
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
#include "derivative.h" /* Include peripheral declarations */
#include "MC33937_routines.h"
#include "sim.h"
#include "ics.h"
#include "osc.h"
#include "rtc.h"
#include "gpio.h"
#include "spi.h"
#include "pit.h"
#include "uart.h"
#include "adc.h"
#include "acmp.h"
#include "can.h"
#include "can_cfg.h"
#include "ftm.h"
#include "nvic.h"
#include "BLDCSensorless.h"
/*******************************************************************************
* Global variables
*******************************************************************************/

/*******************************************************************************
* Local variables
*******************************************************************************/
static uint16_t NextCmtPeriod, duty_cycle;
static uint8_t  NextCmtSector, ActualCmtSector;
static uint16_t alignmentTimer, startCMTcounter, freewheelTimer;
static MC33937_SR_T mc33937_Status;
static uint8_t      appState = APP_INIT;
static uint8_t      rotationDir = ROTATION_DIR_CW;
static volatile     uint8_t appSwitchState = 0, faultSwitchClear;
static tDriveStatus driveStatus;
static tFaultStatus faultStatus, faultStatusLatched;
static tADCresults  ADCResults;
static int16_t  bemfVoltage, bemfVoltageOld;
static uint16_t timeBackEmf, timeOldBackEmf, timeZCToff;
static uint16_t timeZC, lastTimeZC;
static uint16_t ftm_mod_old;
static uint16_t actualPeriodZC;
static uint16_t period6ZC, periodZC[6];
static uint16_t advanceAngle;
static uint16_t debugPeriodMin, debugPeriodMax, periodZcAvrg, stallCheckCounter;
static int16_t speedErr;
static volatile int16_t requiredSpeed = N_MIN;
static int16_t requiredSpeedRamp;
static int16_t actualSpeed;
static t_xl_pi_control speedPIPrms, currentPIPrms;
static t_xl_ramp speedRampPrms;
static int16_t switchCounter[2], switchOffCounter;

static void AppInit(void);
static void AppCalib(void);
static void AppAlignment(void);
static void AppStart(void);
static void AppRun(void);
static void AppStop(void);
static void AppFault(void);

static void AppStopToAlignment(void);
static void AppAlignmentToStart(void);
static void AppStartToRun(void);

static void CheckFaults(void);
static void StallCheck(void);


/* ADC conversion sequence channel list */
static uint8_t AdcChannelList[2][6][3] =
{
    /* Clockwise rotation direction */
    {
        {ADC_BEMFC, ADC_DCBI, ADC_DCBV},
        {ADC_BEMFB, ADC_DCBI, ADC_DCBV},
        {ADC_BEMFA, ADC_DCBI, ADC_DCBV},
        {ADC_BEMFC, ADC_DCBI, ADC_DCBV},
        {ADC_BEMFB, ADC_DCBI, ADC_DCBV},
        {ADC_BEMFA, ADC_DCBI, ADC_DCBV}
    },
    /* Counterclockwise rotation direction */
    {
        {ADC_BEMFC, ADC_DCBI, ADC_DCBV},
        {ADC_BEMFA, ADC_DCBI, ADC_DCBV},
        {ADC_BEMFB, ADC_DCBI, ADC_DCBV},
        {ADC_BEMFC, ADC_DCBI, ADC_DCBV},
        {ADC_BEMFA, ADC_DCBI, ADC_DCBV},
        {ADC_BEMFB, ADC_DCBI, ADC_DCBV}
    }
};

/* MCAT-referenced variables */
/* MCAT Parameters tab */
static volatile int16_t mcat_alignVoltage = ALIGN_VOLTAGE;
static volatile int16_t mcat_alignDuration = ALIGN_DURATION;

/* MCAT Sensorless tab */
static volatile int16_t mcat_NMin = N_MIN;
static volatile uint16_t mcat_FreewheelT = FREEWHEEL_T_LONG;
static volatile uint8_t mcat_startCmtCnt = STARTUP_CMT_CNT;
static volatile int16_t mcat_startCmtPer = STARTUP_CMT_PER;
static volatile int16_t mcat_startCmtAcceler = START_CMT_ACCELER;
static volatile int16_t mcat_cmtTOff = CMT_T_OFF;

static volatile uint16_t mcat_Dummy16;

static uint16_t xl_lib_mul(int16_t x, int16_t y)
{
	uint32_t tmp;
	tmp = ((uint32_t)(x) * (uint32_t)(y));
	tmp >>= 15;
	return (uint16_t)tmp;
}

static signed long xl_lib_F32F16(int16_t x)
{
	signed long tmp;
	tmp = (signed long) x;
	tmp <<= 16;
	return tmp;
}

static signed short xl_lib_F16F32(int32_t x)
{
	signed long tmp;
	tmp = x;
	tmp >>= 16;
	return tmp;
}

static signed short xl_lib_SubSat(signed short x, signed short y)
{
	signed long tmp;
	tmp = x;
	return (signed short)(tmp - y);
}

static signed short xl_lib_div(signed short x, signed short y)
{
	signed long tmp;
	tmp = x;
	tmp <<= 15;
	tmp = tmp / y;
	return (signed short)tmp;
}

static signed short xl_lib_pi(signed short x, signed short y)
{
	if (x/y > 12)
		return 12;
	else if (x/y > 10)
		return 10;
	else if (x/y > 8)
			return 8;
	else if (x/y > 6)
			return 6;
	else if (x/y > 4)
			return 4;
	else
		return 2;
}

static unsigned short int my_duty = XL_LOWEST_DUTY;
/*******************************************************************************
* Constants
*******************************************************************************/
/* Array with pointers to the state machine functions */
static const tPointerFcn AppStateMachine[] = \
{
    AppInit,
    AppCalib,
    AppAlignment,
    AppStart,
    AppRun,
    AppStop,
    AppFault
};

/* FTM2 channel output mask control */
static const uint8_t ui8FTM2OutmaskVal[2][8] =
{
    /* Clockwise rotation direction */
    {
        0x34,   /* sector 0 */
        0x1C,   /* sector 1 */
        0x13,   /* sector 2 */
        0x31,   /* sector 3 */
        0x0D,   /* sector 4 */
        0x07,   /* sector 5 */
        0x05,   /* alignment vector */
        0x3F    /* PWM off */
    },
    /* Counterclockwise rotation direction */
    {
        0x31,   /* sector 3 */
        0x13,   /* sector 2 */
        0x1C,   /* sector 1 */
        0x34,   /* sector 0 */
        0x07,   /* sector 5 */
        0x0D,   /* sector 4 */
        0x05,   /* alignment vector */
        0x3F    /* PWM off */
    }
};

/* FTM channel software output control */
static const uint16_t ui16FTM2SwOctrlVal[2][8] =
{
    /* Clockwise rotation direction */
    {
        0x0808, /* sector 0 */
        0x2020, /* sector 1 */
        0x2020, /* sector 2 */
        0x0202, /* sector 3 */
        0x0202, /* sector 4 */
        0x0808, /* sector 5 */
        0x000A, /* alignment vector */
        0x0000  /* PWM off */
    },
    /* Counterclockwise rotation direction */
    {
        0x0202, /* sector 3 */
        0x2020, /* sector 2 */
        0x2020, /* sector 1 */
        0x0808, /* sector 0 */
        0x0808, /* sector 5 */
        0x0202, /* sector 4 */
        0x0A0A, /* alignment vector */
        0x0000  /* PWM off */
    }
};

/* FreeMASTER constants */
/*
const int16_t fm_voltage = FM_U_DCB_SCALE;
const int32_t fm_current = FM_I_SCALE;
const int32_t fm_speed = FM_N_SCALE;
*/


/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void NMI_ISR(void)
*
* Description:  Non-maskable interrupt service routine
*
*******************************************************************************/
void NMI_ISR(void)
{
    /* Disable non-maskable interrupt */
    SIM_SOPT0 &= ~SIM_SOPT0_NMIE_MASK;
}

/*******************************************************************************
*
* Function: void Can_RxNotification(
*           (
*               Can_ModuleIDType    Can_ModuleID,
*               Can_IDType          Can_ID,
*               Can_DataLengthType  Can_DataLength,
*               uint8_t             *Can_Data
*           )
*
* Description:  CAN receive notification function
*
*******************************************************************************/
void Can_RxNotification
(
    Can_ModuleIDType    Can_ModuleID,
    Can_IDType          Can_ID,
    Can_DataLengthType  Can_DataLength,
    uint8_t             *Can_Data
)
{

}

/*******************************************************************************
*
* Function: void ICS_Trim(void)
*
* Description:  The function trims the internal RC oscillator clock frequency
*               to 37.5kHz.
*
*               The time difference between two RTC overflows (2 RTC ticks) is
*               measured using FTM1 channel 1 input capture to determine actual
*               ICSIRCLK frequency. The FTM1 counter counts the FLL output
*               clock while the RTC counts the ICSIRCLK clock. Based
*               on the difference measured, the function adjusts
*               the ICS_C3[ICSTRIM] value until the desired ICSIRCLK frequency
*               is reached.
*
*               desired_time_diff = 2 * (40MHz / 37.5kHz) = 2133.33;
*
*******************************************************************************/
void ICSIRCLK_Trim(void)
{
    uint16_t ui16Capt, ui16Period = 0xFFFF;

    /* Check if the ICSIRCLK trim values are present in FLASH */
    if( *((uint8_t*) 0x03FFU) != 0xFFU)
    {
        ICS_C3 = *((uint8_t*) 0x03FFU);
        ICS_C4 = (ICS_C4 & 0xFEU) | ((*((uint8_t*) 0x03FEU)) & 0x01U);
    }
    else
    {
        /* Initialize core/platform/system clock to 40MHz, Bus clock to 20MHz */
        ICS_Init_FEE();
        /* Initialize FTM1 for ICSIRCLK trimming (RTC overflow input capture) */
        FTM1_Init();
        /* Initialize RTC counter with the period of 2 IRC ticks */
        RTC_Init(RTC_IRC_TICKS_2);

        /* Perform ICSIRCLK trimming */
        while((ui16Period < 2131) || (ui16Period > 2135))
        {
            /* Clear RTIF flag */
            RTC_SC = RTC_SC;

            /* Wait for input capture */
            while((RTC_SC & RTC_SC_RTIF_MASK) == 0)
            {
            }

            ui16Capt = (uint16_t)FTM1_C1V;
            /* Clear RTIF flag */
            RTC_SC = RTC_SC;

            /* Wait for next input capture */
            while((RTC_SC & RTC_SC_RTIF_MASK) == 0)
            {
            }

            ui16Period = (uint16_t)FTM1_C1V - ui16Capt;

            if(ui16Period > 2135)
            {
                ICS_C3--;
            }
            else if(ui16Period < 2131)
            {
                ICS_C3++;
            }
        }

        /* Disable FTM1 & RTC module clock  */
        SIM_SCGC = (SIM_SCGC & ~SIM_SCGC_FTM1_MASK) & ~SIM_SCGC_RTC_MASK;
    }
}

/*******************************************************************************
*
* Function: void main(void)
*
* Description: main function
*
*******************************************************************************/
int main(void)
{
	uint8_t ret;

    /* Initialize System Integration Module */
    SIM_Init();
    /* Initialize Oscillator module */
    OSC_Init();
    /* Initialize General-Purpose Input/Output module */
    GPIO_Init();

    /* Trim the ICSIRCLK clock frequency to 37.5kHz */
    ICSIRCLK_Trim();

    /* Initialize core/platform/system clock to 48MHz, Bus clock to 24MHz */
    ICS_Init_FEI();

    /* Initialize Serial Peripheral Interface 0 */
    SPI0_Init();
    /* Initialize Periodic Interrupt Timer */
    PIT_Init();
    /* Initialize UART2 for FreeMASTER communication */
    UART2_Init();
    /* Initialize ADC */
    ADC_Init();
    /* Initialize ACMP0 */
    ACMP0_Init(ACMP0_DACVAL);
    /* Initialize ACMP1 */
    ACMP1_Init(ACMP1_DACVAL);
    /* Initialize MSCAN driver */
    //ret_val = Can_Init(&Can_ModuleConfig[0]);
    /* Set MSCAN to RUN mode */
    //ret_val = Can_SetMode(CAN_KEA128_CAN0,CAN_MODE_RUN);

    /* Initialize MC33903D System Basis Chip */
    //MC33903_Config();

    /* Initialize FTM2 (PWM) */
    FTM2_Init();
    /* Initialize FTM0 (commutation timer) */
    FTM0_Init();
    /* Initialize PIT channel 0 (speed and current control loop timer) */
    PIT_CH0_Init(PIT_1MS);

//	while(1)
//	{
//		PIT_CH1_Delay(KEA128_PIT_FREQ);
//		LED_RED_ON;
//		PIT_CH1_Delay(KEA128_PIT_FREQ);
//		LED_RED_OFF;
//	}

    /* Initialize MC33937A pre-driver */
    do {
        ret = MC33937_Config();
    }while(ret == 0);

    /* Enable MC33937A MOSFET pre-driver */
    MC33937_Enable();

    /* Initialize FreeMASTER */
    // ret = FMSTR_Init();

    /* Enable interrupts and set interrupt priorities */
    //NVIC_EnableIrq(INT_MSCAN_RX);
    //NVIC_SetIrqPriority(INT_MSCAN_RX,NVIC_IRQ_PRIORITY_3);

    NVIC_EnableIrq(INT_PIT_CH0);
    NVIC_SetIrqPriority(INT_PIT_CH0,NVIC_IRQ_PRIORITY_2);

    NVIC_EnableIrq(INT_ADC0);
    NVIC_SetIrqPriority(INT_ADC0,NVIC_IRQ_PRIORITY_0);

    NVIC_EnableIrq(INT_FTM0);
    NVIC_SetIrqPriority(INT_FTM0,NVIC_IRQ_PRIORITY_0);

    //NVIC_EnableIrq(INT_UART2);
    //NVIC_SetIrqPriority(INT_UART2,NVIC_IRQ_PRIORITY_1);

    /* Speed PI controller initialization */
    speedPIPrms.f16PropGain = SPEED_LOOP_KP_GAIN;
    speedPIPrms.f16IntegGain = SPEED_LOOP_KI_GAIN;
    speedPIPrms.s16PropGainShift = SPEED_LOOP_KP_SHIFT;
    speedPIPrms.s16IntegGainShift = SPEED_LOOP_KI_SHIFT;
    speedPIPrms.f16UpperLimit = CTRL_LOOP_LIM_HIGH;
    speedPIPrms.f16LowerLimit = CTRL_LOOP_LIM_LOW;

    /* Current PI controller initialization */
    currentPIPrms.f16PropGain = TORQUE_LOOP_KP_GAIN;
    currentPIPrms.f16IntegGain = TORQUE_LOOP_KI_GAIN;
    currentPIPrms.s16PropGainShift = TORQUE_LOOP_KP_SHIFT;
    currentPIPrms.s16IntegGainShift = TORQUE_LOOP_KI_SHIFT;
    currentPIPrms.f16UpperLimit = CTRL_LOOP_LIM_HIGH;
    currentPIPrms.f16LowerLimit = CTRL_LOOP_LIM_LOW;

    /* SPeed ramp initialization */
    speedRampPrms.f32RampUp = SPEED_LOOP_RAMP_UP;
    speedRampPrms.f32RampDown = SPEED_LOOP_RAMP_DOWN;

    /* Set default current measurement bias offset (actual offset will measured
       in the Calibration state function) */
    ADCResults.DCBIOffset = 0.5 * 32768;

    
    for(;;)
    {
        /* Clear SBC watchdog */
        // MC33903_ClearWDT();

        /* FreeMASTER polling function */
        // FMSTR_Poll();

        /* Call BLDC application state machine function */
        AppStateMachine[appState]();

        /* Stall check */
        if(driveStatus.B.StallCheckReq == 1)
        {
            driveStatus.B.StallCheckReq = 0;
            StallCheck();
        }

        /* Check power stage faults */
        CheckFaults();
    }

    return 0;
}


/*******************************************************************************
* Local functions
*******************************************************************************/
/******************************************************************************
*
* Function: void AppInit(void)
*
* Description: BLDC application INIT state function
*
*******************************************************************************/
static void AppInit(void)
{
    driveStatus.B.Alignment = 0;
    driveStatus.B.EnableCMT = 0;
    driveStatus.B.CloseLoop = 0;
    driveStatus.B.Sensorless = 0;
    driveStatus.B.NewZC = 0;

#ifdef REYNO_TEST
    /* Disable all PWMs */
    FTM2_OUTMASK = ui8FTM2OutmaskVal[0][7];
    FTM2_SWOCTRL = ui16FTM2SwOctrlVal[0][7];
    /* Apply OUTMASK & SWOCTRL */
    FTM2_SYNC |= FTM_SYNC_SWSYNC_MASK;

    duty_cycle = 0;
    FTM2_C1V = duty_cycle;
    FTM2_C3V = duty_cycle;
    FTM2_C5V = duty_cycle;
    FTM2_PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;
    SIM_SOPT0 = (SIM_SOPT0 & ~SIM_SOPT0_DELAY_MASK) | SIM_SOPT0_DELAY(DELAY);

    FTM0_SC &= ~FTM_SC_CLKS_MASK;   /* Stop FTM0 counter */
    FTM0_MOD = 0x0000;              /* Reset modulo count */
    FTM0_CNT = 0x0000;              /* Reset FTM0 counter */

    advanceAngle = ADVANCE_ANGLE;

    NextCmtPeriod = FTM0_PERIOD_1MS;
#endif /* 0 */

    /* Wait for DC bus current to settle down (after motor stops) */
    PIT_CH1_Delay(PIT_1MS);
    
    ADC_EnableTrigSeq(&AdcChannelList[0][0][0],3);

    LED_RED_OFF;
    LED_GREEN_OFF;

    appState = APP_STOP;
}

/*******************************************************************************
*
* Function: void AppStop(void)
*
* Description: BLDC application STOP state function
*
*******************************************************************************/
static void AppStop(void)
{
    if((appSwitchState == 1) && (driveStatus.B.Freewheeling == 0))
    {
        if(SW_RUN_L == 1)
        {
            rotationDir = ROTATION_DIR_CCW;
        }
        else
        {
            rotationDir = ROTATION_DIR_CW;
        }

        appState = APP_CALIB;
    }
}

/*******************************************************************************
*
* Function: void AppCalib(void)
*
* Description:  BLDC application CALIB state function
*
*******************************************************************************/
static void AppCalib(void)
{
    uint16_t i;
    uint32_t sum;

    /* DC bus current offset calibration */
    sum = 0;
    for(i=0; i<512; i++)
    {
        /* DC bus current calibration */
        ADC_StartSingleConversion(ADC_DCBI);

        /* Wait until the DC bus current conversion is complete */
        while((ADC_SC1 & ADC_SC1_COCO_MASK) == 0)
        {
        }

        /* Save DC Bus current offset */
        sum += (ADC_R);
    }
    sum = sum >> 6;
    /* Save DC Bus current offset */
    ADCResults.DCBIOffset = (int16_t)sum;

    AppStopToAlignment();
}
/*******************************************************************************
*
* Function: void AppStopToAlignment(void)
*
* Description:  BLDC application STOP to ALIGN state transition function
*
*******************************************************************************/
static void AppStopToAlignment(void)
{
	driveStatus.B.Alignment = 1;
    driveStatus.B.EnableCMT = 0;
    driveStatus.B.CloseLoop = 0;
    driveStatus.B.Sensorless = 0;
    driveStatus.B.NewZC = 0;

    NextCmtPeriod = FTM0_PERIOD_1MS;

    alignmentTimer = mcat_alignDuration;
    duty_cycle = xl_lib_mul(PWM_MODULO,mcat_alignVoltage);

    /* Update PWM duty cycle */
    FTM2_C1V = duty_cycle;
    FTM2_C3V = duty_cycle;
    FTM2_C5V = duty_cycle;

    FTM2_PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;

    /* Apply PWM settings for motor alignment */
    FTM2_OUTMASK = ui8FTM2OutmaskVal[rotationDir][6];   // 0b00000101
    													// A HS mask
    													// B HS mask
    FTM2_SWOCTRL = ui16FTM2SwOctrlVal[rotationDir][6];  // 0b00001010
    													// A LS output enable
    													// B LS output enable
    /* Apply OUTMASK & SWOCTRL */
    FTM2_SYNC |= FTM_SYNC_SWSYNC_MASK;

    FTM0_MOD = FTM0_PERIOD_1MS;
    FTM0_SC |= FTM_SC_CLKS(1);      /* Start FTM0 counter */

    appState = APP_ALIGNMENT;

    LED_GREEN_ON;
}

/*******************************************************************************
*
* Function: void AppAlignment(void)
*
* Description:  BLDC application ALIGN state function
*
*******************************************************************************/
static void AppAlignment(void)
{
    if(alignmentTimer == 0)
    {
        LED_GREEN_OFF;

        AppAlignmentToStart();
    }
}

/*******************************************************************************
*
* Function: void AppAlignmentToStart(void)
*
* Description:  BLDC application ALIGN to START state transition function
*
*******************************************************************************/
static void AppAlignmentToStart(void)
{
    driveStatus.B.Alignment = 0;
    driveStatus.B.EnableCMT = 1;
    driveStatus.B.AfterCMT  = 0;

    NextCmtSector = 0;              /* Starting sector */

    NextCmtPeriod = mcat_startCmtPer;
    startCMTcounter = mcat_startCmtCnt - 1;

    /* Prepare PWM settings for initial commutation sector */
    FTM2_OUTMASK = ui8FTM2OutmaskVal[rotationDir][NextCmtSector];
    FTM2_SWOCTRL = ui16FTM2SwOctrlVal[rotationDir][NextCmtSector];

    FTM0_SC &= ~FTM_SC_CLKS_MASK;   /* Stop FTM0 counter */
    FTM0_CNT = 0x0000;              /* Force commutation sector 0 PWM settings */
    FTM0_MOD = STARTUP_CMT_PER;
    FTM0_SC |= FTM_SC_CLKS(1);      /* Start FTM0 counter */

    NextCmtSector++;

    /* Prepare PWM settings for the next commutation sector */
    FTM2_OUTMASK = ui8FTM2OutmaskVal[rotationDir][NextCmtSector];
    FTM2_SWOCTRL = ui16FTM2SwOctrlVal[rotationDir][NextCmtSector];

    NextCmtPeriod = xl_lib_mul(NextCmtPeriod,mcat_startCmtAcceler);

    appState = APP_START;
}

/*******************************************************************************
*
* Function: void AppStart(void)
*
* Description:  BLDC application START state function
*
*******************************************************************************/
static void AppStart(void)
{
    if(driveStatus.B.AfterCMT == 1)
    {
        timeZC = NextCmtPeriod >> 1;

        startCMTcounter--;
        if(startCMTcounter > 0)
        {
            driveStatus.B.AfterCMT = 0;

            NextCmtPeriod = xl_lib_mul(NextCmtPeriod,mcat_startCmtAcceler);
        }
    }

    if(startCMTcounter == 0)
    {
        AppStartToRun();
    }
}

/*******************************************************************************
*
* Function: void AppStartToRun(void)
*
* Description:  BLDC application START to RUN state transition function
*
*******************************************************************************/
static void AppStartToRun(void)
{
    uint8_t i;

    /* Speed PI controller initialization */
    speedPIPrms.f16InK_1 = 0;
    speedPIPrms.f32IntegPartK_1 = (((int32_t)duty_cycle << 15)/(int32_t)PWM_MODULO) << 16;

    /* Current PI controller initialization */
    currentPIPrms.f16InK_1 = 0;
    currentPIPrms.f32IntegPartK_1 = speedPIPrms.f32IntegPartK_1;

    /* Speed ramp initialization */
    //speedRampPrms.f32State = MLIB_ConvertPU_F32F16(mcat_NMin);
    speedRampPrms.f32State = xl_lib_F32F16(mcat_NMin);

    appState = APP_RUN;
    stallCheckCounter = 0;
    faultStatus.B.StallError = 0;

    for(i=0;i<6;i++)
    {
        periodZC[i] = NextCmtPeriod;
    }
    actualPeriodZC = NextCmtPeriod;

    driveStatus.B.Sensorless = 1;
    driveStatus.B.CloseLoop = 1;
}

/*******************************************************************************
*
* Function: void AppRun(void)
*
* Description:  BLDC application RUN state function
*
*******************************************************************************/
static void AppRun(void)
{
    if(appSwitchState == 0)
    {
        duty_cycle = 0;

        FTM2_C1V = duty_cycle;
        FTM2_C3V = duty_cycle;
        FTM2_C5V = duty_cycle;
        FTM2_PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;

        freewheelTimer = mcat_FreewheelT;
        driveStatus.B.Freewheeling = 1;

        appState = APP_INIT;
    }
}

/*******************************************************************************
*
* Function: void AppFault(void)
*
* Description: BLDC application FAULT state function
*
*******************************************************************************/
static void AppFault(void)
{
    if(faultSwitchClear == 1)
    {
        /* Clear MC33937A status register */
        MC33937_ClearFaults();

        /* Re-enable MC33937A */
        MC33937_Enable();

        driveStatus.B.Fault = 0;
        faultStatus.R = 0;
        faultStatusLatched.R = 0;

        faultSwitchClear = 0;
		my_duty = XL_LOWEST_DUTY;
        appState = APP_INIT;

        LED_RED_OFF;
    }
}

/*******************************************************************************
*
* Function: void CheckFaults(void)
*
* Description:  BLDC application fault detection function.
*
*******************************************************************************/
static void CheckFaults(void)
{
    /* DC bus overcurrent and undervoltage */
    if(FTM2_FMS & (FTM_FMS_FAULTF3_MASK | FTM_FMS_FAULTF0_MASK))
    {
        driveStatus.B.Alignment = 0;
        driveStatus.B.EnableCMT = 0;
        driveStatus.B.CloseLoop = 0;
        driveStatus.B.Sensorless = 0;
        driveStatus.B.NewZC = 0;

        faultStatus.B.UnderDCBusVoltage = (FTM2_FMS & FTM_FMS_FAULTF3_MASK) >> FTM_FMS_FAULTF3_SHIFT;
        faultStatus.B.OverDCBusCurrent = (FTM2_FMS & FTM_FMS_FAULTF0_MASK) >> FTM_FMS_FAULTF0_SHIFT;

        duty_cycle = 0;
        /* Update PWM duty cycle */
        FTM2_C1V = duty_cycle;
        FTM2_C3V = duty_cycle;
        FTM2_C5V = duty_cycle;
        FTM2_PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;

        /* Clear all fault flags */
        FTM2_FMS = 0x00000000;
    }
    else
    {
        faultStatus.B.UnderDCBusVoltage = 0;
        faultStatus.B.OverDCBusCurrent = 0;
    }

    /* MC33937A pre-driver fault */
    if(MC33937_INT == 1)
    {
        faultStatus.B.PreDriverError = 1;
        mc33937_Status = MC33937_ReadStatus();
    }

    /* DC bus voltage overvoltage */
    if(ADCResults.DCBVVoltage > U_DCB_OVERVOLTAGE)
    {
        faultStatus.B.OverDCBusVoltage = 1;

        driveStatus.B.Alignment = 0;
        driveStatus.B.EnableCMT = 0;
        driveStatus.B.CloseLoop = 0;
        driveStatus.B.Sensorless = 0;
        driveStatus.B.NewZC = 0;

        duty_cycle = 0;
        /* Update PWM duty cycle */
        FTM2_C1V = duty_cycle;
        FTM2_C3V = duty_cycle;
        FTM2_C5V = duty_cycle;
        FTM2_PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;
    }
    else
    {
        faultStatus.B.OverDCBusVoltage = 0;
    }

    /* Stall error */
    if(faultStatus.B.StallError)
    {
        driveStatus.B.Alignment = 0;
        driveStatus.B.EnableCMT = 0;
        driveStatus.B.CloseLoop = 0;
        driveStatus.B.Sensorless = 0;
        driveStatus.B.NewZC = 0;

        duty_cycle = 0;
        /* Update PWM duty cycle */
        FTM2_C1V = duty_cycle;
        FTM2_C3V = duty_cycle;
        FTM2_C5V = duty_cycle;
        FTM2_PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;
    }

    faultStatusLatched.R |= faultStatus.R;

    if(faultStatusLatched.R != 0)
    {
        driveStatus.B.Fault = 1;
        appSwitchState = 0;
        appState = APP_FAULT;
        LED_RED_ON;
        LED_GREEN_OFF;
    }
    else
    {
        faultSwitchClear = 0;
    }
}

/*******************************************************************************
*
* Function: void CheckSwitchState(void)
*
* Description:  User switch state detection function
*
*******************************************************************************/
static void CheckSwitchState(void)
{
    if(switchOffCounter == 0)
    {
        /* Speed up or start the motor */
        if(SWB1 == 0)
        {
            switchCounter[0]++;

            if(switchCounter[0] > SW_PRESS_DEBOUNCE)
            {
                if(appSwitchState == 0)
                {
                    appSwitchState = 1;
                    switchOffCounter = SW_PRESS_OFF;
                }
                else
                {
                    requiredSpeed += SPEED_INC;
                }

                switchCounter[0] = 0;
            }
        }

        /* Speed down */
        if(SWB0 == 0)
        {
            switchCounter[1]++;

            if(switchCounter[1] > SW_PRESS_DEBOUNCE)
            {
                requiredSpeed -= SPEED_DEC;

                switchCounter[1] = 0;
            }
        }

        /* Clear faults or stop the motor */
        if((SWB1 == 0) && (SWB0 == 0))
        {
            if(appState == APP_FAULT)
            {
                faultSwitchClear = 1;
            }
			my_duty = XL_LOWEST_DUTY;
            appSwitchState = 0;
            switchOffCounter = SW_PRESS_OFF;
        }
    }
    else
    {
        switchOffCounter--;
    }
}

/*******************************************************************************
*
* Function: void ADC_ISR(void)
*
* Description:  ADC conversion complete interrupt service routine
*
*******************************************************************************/
void ADC_ISR(void)
{
	// 20K sample
    int16_t delta;

    timeOldBackEmf = timeBackEmf;
    timeBackEmf = FTM0_CNT;

    ADCResults.BEMFVoltage = (int16_t)(ADC_R << 3);
    ADCResults.DCBIVoltage = (int16_t)(ADC_R << 3) - ADCResults.DCBIOffset;
    ADCResults.DCBVVoltage = (int16_t)(ADC_R << 3);

    bemfVoltage = ADCResults.BEMFVoltage - (ADCResults.DCBVVoltage >> 1);

    //FMSTR_Recorder();

    if(driveStatus.B.AfterCMT == 1)
    {
        if(timeBackEmf > timeZCToff)
        {
            driveStatus.B.AfterCMT = 0;
        }
    }

    if((driveStatus.B.AfterCMT == 0) && (driveStatus.B.NewZC == 0) && (driveStatus.B.Sensorless == 1))
    {
    	//LED_GREEN_TOGGLE;
        /* If the BEMF voltage is falling, invert BEMF voltage value */
        if((ActualCmtSector & 0x01) == 0)
        {
            bemfVoltage = -bemfVoltage;
        }

        /* Rising BEMF zero-crossing detection */
        if(bemfVoltage >= 0)
        {
			//LED_RED_TOGGLE;
            /* Rising interpolation */
            delta = bemfVoltage - bemfVoltageOld;
            if((driveStatus.B.AdcSaved == 1) && (delta > bemfVoltage))
            {
                timeBackEmf -= xl_lib_mul(xl_lib_div(bemfVoltage, delta), (timeBackEmf - timeOldBackEmf));
            }
            else
            {
                timeBackEmf -= ((timeBackEmf - timeOldBackEmf) >> 1);
            }

            lastTimeZC = timeZC;
            timeZC = timeBackEmf;

            periodZC[ActualCmtSector] = (ftm_mod_old - lastTimeZC) + timeZC;
            actualPeriodZC = (actualPeriodZC + periodZC[ActualCmtSector]) >> 1;

            NextCmtPeriod = xl_lib_mul(actualPeriodZC, advanceAngle);
            FTM0_UPDATE_MOD(timeZC + NextCmtPeriod);

            driveStatus.B.NewZC = 1;
        }

        bemfVoltageOld = bemfVoltage;   /* Save actual BEMF voltage (for ADC
                                           samples interpolation) */
        driveStatus.B.AdcSaved = 1;
    }
}

/*******************************************************************************
*
* Function: void FTM0_ISR(void)
*
* Description:  Commutation interrupt service routine
*
*******************************************************************************/
void FTM0_ISR(void)
{
	LED_RED_TOGGLE;
    ftm_mod_old = FTM0_MOD;

    if(driveStatus.B.Sensorless == 1)
    {
        if(driveStatus.B.NewZC == 0)
        {
            /* In the middle between two commutations */
            timeZC = actualPeriodZC >> 1;
        }

        FTM0_UPDATE_MOD(actualPeriodZC << 1);

        timeZCToff = xl_lib_mul(actualPeriodZC,mcat_cmtTOff);
        driveStatus.B.StallCheckReq = 1;
    }
    else
    {
        FTM0_UPDATE_MOD(NextCmtPeriod);
    }

    ActualCmtSector = NextCmtSector;
    if(driveStatus.B.EnableCMT)
    {
        ADC_EnableTrigSeq(&AdcChannelList[rotationDir][ActualCmtSector][0],3);

        NextCmtSector++;
        if(NextCmtSector > 5)
        {
            NextCmtSector = 0;
        }

        /* Prepare PWM settings for the next commutation sector */
        FTM2_OUTMASK = ui8FTM2OutmaskVal[rotationDir][NextCmtSector];
        FTM2_SWOCTRL = ui16FTM2SwOctrlVal[rotationDir][NextCmtSector];
    }

    if(driveStatus.B.Alignment)
    {
        if(alignmentTimer > 0)
        {
            alignmentTimer--;
        }
    }

    driveStatus.B.NewZC = 0;
    driveStatus.B.AdcSaved = 0;
    driveStatus.B.AfterCMT = 1;

    FTM0_SC &= ~FTM_SC_TOF_MASK;    /* Clear TOF flag */
}

/*******************************************************************************
*
* Function: void PIT_CH0_ISR(void)
*
* Description:  PIT channel 0 time-out interrupt service routine (speed and
*               current control loop)
*
*******************************************************************************/
void PIT_CH0_ISR(void)
{
    uint8_t i;

    PIT_TFLG0 = PIT_TFLG_TIF_MASK;  /* Clear TOF flag */

    if(driveStatus.B.CloseLoop == 1)
    {
        /* Speed control */
        period6ZC = periodZC[0];
        for(i=1;i<6;i++)
        {
            period6ZC += periodZC[i];
        }

        actualSpeed = xl_lib_div(SPEED_SCALE_CONST,period6ZC);
			
		requiredSpeedRamp = requiredSpeed;
		speedErr = requiredSpeedRamp - actualSpeed;

        /* Update PWM duty cycle */
        if (speedErr > 0)
        {
        	my_duty += 5;
        	if (my_duty > XL_HIGHEST_DUTY)
        		my_duty = XL_HIGHEST_DUTY; 
        }
        else
        {
        	my_duty -= 5;
        	if (my_duty < XL_LOWEST_DUTY)
        		my_duty = XL_LOWEST_DUTY;
        }
        duty_cycle = my_duty;
        
        FTM2_C1V = duty_cycle;
        FTM2_C3V = duty_cycle;
        FTM2_C5V = duty_cycle;
        FTM2_PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;
    }
    else
    {
        actualSpeed = 0;
    }

    if(driveStatus.B.Freewheeling)
    {
        if(freewheelTimer > 0)
        {
            freewheelTimer--;
        }
        else
        {
            driveStatus.B.Freewheeling = 0;
        }
    }

    CheckSwitchState();

    if(requiredSpeed < mcat_NMin)
    {
        requiredSpeed = mcat_NMin;
    }

    if(requiredSpeed > SPEED_MAX)
    {
        requiredSpeed = SPEED_MAX;
    }
}

/*******************************************************************************
*
* Function: void StallCheck(void)
*
* Description:  Stall check function
*
*******************************************************************************/
static void StallCheck(void)
{
    uint8_t i;
    uint16_t max = 0, min = 65535;

    for(i=0; i<6; i++)
    {
        if(periodZC[i] > max)
        {
            max = periodZC[i];
        }

        if(periodZC[i] < min)
        {
            min = periodZC[i];
        }
    }

    /* Save min and max commutation periods for tuning purposes */
    debugPeriodMin = min;
    debugPeriodMax = max;

    periodZcAvrg = xl_lib_mul(period6ZC,0.1666 * 32768);

    if ((max > (periodZcAvrg << 1)) || (min < (periodZcAvrg >> 1)))
    {
        if (stallCheckCounter < STALLCHECK_MAX_ERRORS)
        {
            stallCheckCounter++;
        }
    }
    else
    {
        if (min < STALLCHECK_MIN_CMT_PERIOD)
        {
            if (stallCheckCounter < STALLCHECK_MAX_ERRORS)
            {
                stallCheckCounter++;
            }
        }
        else
        {
            if (stallCheckCounter > 0)
            {
                stallCheckCounter--;
            }
        }
    }

    if (stallCheckCounter >= STALLCHECK_MAX_ERRORS)
    {
        faultStatus.B.StallError = 1;
    }
}
