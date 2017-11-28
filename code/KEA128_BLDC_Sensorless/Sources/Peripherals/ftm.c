/*******************************************************************************
*
* (c) Copyright 2014 Freescale Semiconductor
*
****************************************************************************//*!
*
* @file     ftm.c
*
* @author   B06050
*
* @version  1.0.3.0
*
* @date     Jul-3-2014
*
* @brief    FlexTimer SW module source file.
*
*******************************************************************************/
#include "ftm.h"
#include "../BLDCSensorless.h"
//#include "KEA128_appconfig.h"

/*******************************************************************************
* Global functions
*******************************************************************************/
/*******************************************************************************
*
* Function: void FTM0_Init(void)
*
* Description:  Initializes the FTM0 channel 0 for commutation control
*
*******************************************************************************/
void FTM0_Init(void)
{
    /* Edge-aligned PWM - High-true pulses (clear output on match) */
    FTM0_C0SC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    /* Clear output after 1 counter tick */
    FTM0_C0V = 0x00000001;
    /* Timer overflow interrupt enable, divide clock by 64 */
    FTM0_SC = FTM_SC_TOIE_MASK | FTM_SC_PS(6);
}

/*******************************************************************************
*
* Function: void FTM1_Init(void)
*
* Description:  Initializes the FTM1 channel 1 for input capture
*
*******************************************************************************/
void FTM1_Init(void)
{
    /* Counter modulo (free-running counter) */
    FTM1_MOD = 0xFFFF;
    /* Input capture - Capture on rising edge only */
    FTM1_C1SC = FTM_CnSC_ELSA_MASK;
    /* System clock prescaler factor 1 */
    FTM1_SC = FTM_SC_TOF_MASK | FTM_SC_CLKS(1) | FTM_SC_PS(0);
}

/*******************************************************************************
*
* Function: void FTM2_Init(void)
*
* Description:  Initializes the FTM2 module for complementary PWM generation
*               with dead time insertion and fault control.
*
*******************************************************************************/
void FTM2_Init(void)
{
    /* Fault control enabled for all channels with manual fault clearing,
       write protection disabled, FTM enabled */
    FTM2_MODE = FTM_MODE_FAULTM(2) | FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* FTM functional in debug mode */
    FTM2_CONF = FTM_CONF_BDMMODE(3);

    /* Initial value of the FTM counter */
    FTM2_CNTIN = 0x00000000;
    /* Modulo value of the FTM counter set according to PWM_MODULO macro
       value */
    FTM2_MOD = PWM_MODULO - 1;

    /* High-true pulses (set on channel (n) match, clear on channel (n+1)
      match) */
    FTM2_C0SC = FTM_CnSC_ELSB_MASK;
    FTM2_C1SC = FTM_CnSC_ELSB_MASK;
    FTM2_C2SC = FTM_CnSC_ELSB_MASK;
    FTM2_C3SC = FTM_CnSC_ELSB_MASK;
    FTM2_C4SC = FTM_CnSC_ELSB_MASK;
    FTM2_C5SC = FTM_CnSC_ELSB_MASK;

    /* Trigger 1 enabled, OUTMASK register is updated by the PWM synchronization,
       the maximum loading point is enabled */
    FTM2_SYNC = FTM_SYNC_TRIG1_MASK | FTM_SYNC_SYNCHOM_MASK | FTM_SYNC_CNTMAX_MASK;

    /* Fault enabled, PWM synchronization enabled, dead time insertion enabled,
       combine feature enabled, and complementary mode enabled for all channel
       pairs */
    FTM2_COMBINE = FTM_COMBINE_FAULTEN2_MASK | FTM_COMBINE_SYNCEN2_MASK | \
                   FTM_COMBINE_DTEN2_MASK | FTM_COMBINE_COMP2_MASK | \
                   FTM_COMBINE_COMBINE2_MASK | FTM_COMBINE_FAULTEN1_MASK | \
                   FTM_COMBINE_SYNCEN1_MASK | FTM_COMBINE_DTEN1_MASK | \
                   FTM_COMBINE_COMP1_MASK | FTM_COMBINE_COMBINE1_MASK | \
                   FTM_COMBINE_FAULTEN0_MASK | FTM_COMBINE_SYNCEN0_MASK | \
                   FTM_COMBINE_DTEN0_MASK | FTM_COMBINE_COMP0_MASK | \
                   FTM_COMBINE_COMBINE0_MASK;

    /* Dead time prescaler 1 (system clock div. by 1), dead time value set
       according to PWM_DEADTIME macro value */
    FTM2_DEADTIME = FTM_DEADTIME_DTPS(0) | FTM_DEADTIME_DTVAL(PWM_DEADTIME);

    /* Initialization trigger enabled */
    FTM2_EXTTRIG = FTM_EXTTRIG_INITTRIGEN_MASK;

    /* Even (high-side) channels active low, odd (low-side) channels active
       high */
    FTM2_POL = FTM_POL_POL4_MASK | FTM_POL_POL2_MASK | FTM_POL_POL0_MASK;

    /* HW trigger activates SWOCTRL, OUTMASK register synchronization,
       and FTM counter synchronization; software trigger activates SWOCTRL,
       OUTMASK, MOD, CNTIN, CV register synchronization; enhanced PWM
       synchronization mode selected; SWOCTRL register is updated by the PWM
       synchronization; FTM does not clear the TRIG1 bit when HW trigger 1 is
       detected */
    FTM2_SYNCONF = FTM_SYNCONF_HWSOC_MASK | FTM_SYNCONF_HWOM_MASK | \
                   FTM_SYNCONF_HWRSTCNT_MASK | FTM_SYNCONF_SWSOC_MASK | \
                   FTM_SYNCONF_SWOM_MASK | FTM_SYNCONF_SWWRBUF_MASK | \
                   FTM_SYNCONF_SYNCMODE_MASK | FTM_SYNCONF_SWOC_MASK | \
                   FTM_SYNCONF_HWTRIGMODE_MASK;

    /* All channels masked (forced to inactive state) */
    FTM2_OUTMASK = FTM_OUTMASK_CH5OM_MASK | FTM_OUTMASK_CH4OM_MASK | \
                   FTM_OUTMASK_CH3OM_MASK | FTM_OUTMASK_CH2OM_MASK | \
                   FTM_OUTMASK_CH1OM_MASK | FTM_OUTMASK_CH0OM_MASK;

    /* Fault input filter disabled, fault inputs 0 and 3 enabled */
    FTM2_FLTCTRL = FTM_FLTCTRL_FAULT3EN_MASK | FTM_FLTCTRL_FAULT0EN_MASK;

    /* Update of FTM settings */
    FTM2_CNT = 1;

    /* System clock div. by 1 selected as FTM source clock */
    FTM2_SC = FTM_SC_CLKS(1) | FTM_SC_PS(0);

    /* Set zero duty cycle */
    FTM2_C0V = 0x0000;
    FTM2_C1V = 0x0000;
    FTM2_C2V = 0x0000;
    FTM2_C3V = 0x0000;
    FTM2_C4V = 0x0000;
    FTM2_C5V = 0x0000;

    /* Update OUTMASK, SWOCTRL, and CV by software trigger */
    FTM2_SYNC |= FTM_SYNC_SWSYNC_MASK;

    /* Enable loading of the CV registers with their buffered values */
    FTM2_PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;
}
