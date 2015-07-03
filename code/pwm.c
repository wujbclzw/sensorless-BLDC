#include <xc.h>
#include "global.h"

// Initial function for PWM
// PWM设置
// 频率：20K 
// 上下管互补输出 关于中心对齐 
// 带死区设置

void Init_PWM(void)
{

	PTCON	= 0x0000;				//1:1fractional frequency,T=0.1us,free running model,                                 
	PTCON2 	= 0x0001;				// divide-by-2 
	PTPER 	= 2*PWM_Period + 1 ;           // 3500 - 20K

	CHOP 	= 0x0000;				// disable chop
	MDC 	= 0x0000;				// master duty cycle 

 
	// Independent Time base mode
	// center-aligned mode enable
	// positive dead time actively applied for all output modes
	PWMCON1	= 0x0604;					// enable triggle interrupt
	PWMCON2	= 0x0204;
	PWMCON3	= 0x0204;

	// PWM I/O control register
	IOCON1 	= H_OFF_L_OFF ;			// if 0xC400  Set PWM Mode to Complementary, swap PWMH and PWML
	IOCON2 	= H_OFF_L_OFF ;			// if 0xC940  PWM1H-PWM  PWM1L-io-low 
	IOCON3 	= H_OFF_L_OFF ;			// if 0xC940  PWM1H-PWM  PWM1L-io-low 

	PDC1	= 100;
	PDC2	= 100;
	PDC3	= 100;

	// Primary phase shift register
	PHASE1 	= PWM_Period ;
	PHASE2 	= PWM_Period ;
	PHASE3 	= PWM_Period ;
	// SPHASE1 = 0;
	// SPHASE2 = 0;
	// SPHASE3 = 0;
	
	// pwm dead-time register and alternate dead-time register
	// DTRx are noneffective when DTC = 00
	// dead area are controled by ALTDTRx
	DTR1 	= 0x0000;  
	DTR2 	= 0x0000;
	DTR3 	= 0x0000;
	ALTDTR1 = DEAD_AREA;
	ALTDTR2 = DEAD_AREA;
	ALTDTR3 = DEAD_AREA;

    // SEVTCMP: Special Event Compare Count Register 
    // Phase of ADC capture set relative to PWM cycle: 0 offset and counting up
	SEVTCMP = PWM_Period - 100 ;


	// triggle for pwm interrupts
	TRIG1 	= 200 ;
	TRIG2 	= 0x0000 ;
	TRIG3 	= 0x0000 ;
	// trigger control register
	// trigger output every trigger event, no divder frequence and no delay
	TRGCON1	= 0x1000;
	TRGCON2	= 0x0000;
	TRGCON3	= 0x0000;	

	
	// fault current-limit control register
	FCLCON1	= 0x0003;	//fault input disable
	FCLCON2	= 0x0003;
	FCLCON3	= 0x0003;
	

	// PWM interrupt setting
	IEC5bits.PWM1IE 	= 0 ;
	IEC5bits.PWM2IE 	= 0 ;
	IEC6bits.PWM3IE 	= 0 ;
	
	IFS5bits.PWM1IF 	= 0 ;
	IFS5bits.PWM2IF 	= 0 ;
	IFS6bits.PWM3IF 	= 0 ;
	
	//setting for IO port 
	// pwm1/2/3|L/H output open-drain off
	TRISB	&= 0x03FF	; 
	ODCB	&= 0x03FF	;
}

