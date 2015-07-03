#include <xc.h>
#include "global.h"

// extern unsigned char  CalibrateADCFlag ;



void __attribute__ ( (interrupt, no_auto_psv) ) _T1Interrupt( void )
{
	IFS0bits.T1IF = 0;
	
	// nADStableCnt ++ ;	
	LEDRED = ~LEDRED ;

	//bMotorBlocked = 1 ;

}


void __attribute__ ( (interrupt, no_auto_psv) ) _T3Interrupt( void )
{
	IFS0bits.T3IF = 0;
	T2CONbits.TON = 0;

	LEDRED = ~LEDRED ;

	TMR3HLD = 0;
	TMR2 = 0 ;
	T2CONbits.TON = 1;

}


void __attribute__((__interrupt__,no_auto_psv)) _CNInterrupt(void)
{
	// Insert ISR Code Here	
	Disable_PWM ;
	
	PWM_U = H_OFF_L_OFF ;
	PWM_V = H_OFF_L_OFF ;
	PWM_W = H_OFF_L_OFF ;
	
	while(1)
	{
		Delay_nms(500);
		LEDRED = 1 ; 
		Delay_nms(500);
		LEDRED = 0 ; 
	}
	
	IFS1bits.CNIF = 0;
}



void __attribute__ ( (interrupt, no_auto_psv) ) _U1RXInterrupt( void )
{
	// chr = U1RXREG;
	IFS0bits.U1RXIF = 0;
}


void __attribute__ ( (interrupt, no_auto_psv) ) _U1TXInterrupt( void )
{
	IFS0bits.U1TXIF = 0;
}
