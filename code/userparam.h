#ifndef __USERPARAM_H_H__
    #define __USERPARAM_H_H__


		
	// external crystral clock 
	#define 	EXTERNAL_CLOCK	1
	// internal colck 	
	#define		INTERNAL_CLOCK	2
	// select clock type
	#define		CLOCK_TYPE	EXTERNAL_CLOCK

	#if ( CLOCK_TYPE == EXTERNAL_CLOCK )

		#define		CPU_FREQUENCE		70000000	// (x +2 ) * 5 / 4 * 10e6  / 2
		#define		CLOCK_PLLFBD		110	
	#elif	(CLOCK_TYPE == INTERNAL_CLOCK )
		#define		CPU_FREQUENCE		59881250	// (x +2 ) * 7.37 / 4 * 10e6  / 2
		#define		CLOCK_PLLFBD		63
	#endif


	// UART 
	#define		SCI_BAUD_RATE	115200		
	#define		SCI_BUAD_REG_SETTING	(( ( ( CPU_FREQUENCE /16) + (SCI_BAUD_RATE/2) )/SCI_BAUD_RATE ) -1)


	// PWM
	#define		PWM_FREQUENCE  15000		// pwm frequence, center align 
	#define 	PWM_Period	( CPU_FREQUENCE / PWM_FREQUENCE / 2 )
	#define 	DEAD_AREA	70			// 70 - 1us when the frequence of PWM is 10K

	// max pwm duty
	#define		MAX_DUTY		(PWM_Period + DEAD_AREA + 1)
	#define		MIN_DUTY		200 			// DEAD_AREA
	#define		MOTOR_START_DUTY	( (PWM_Period>>4) + DEAD_AREA ) 			
	
	// 


	// wait pwm cycles for COMPBEMF when motor has been started
	#define 	WAITCYCLES_COMPBEMF 	200



	#define 	MAX_CURRENT			180


#endif
