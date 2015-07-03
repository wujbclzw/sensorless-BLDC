#include <xc.h>
#include "global.h"


void EnableInterrupts(void)
{
	/* Enable level 1-7 interrupts */
	/* No restoring of previous CPU IPL state performed here */
	INTCON2bits.GIE = 1;
	return;
}
void DisableInterrupts(void)
{
	/* Disable level 1-7 interrupts */
	/* No saving of current CPU IPL setting performed here */
	INTCON2bits.GIE = 0;
	return;
}
void InitInterrupts(void)
{
	/* Interrupt nesting enabled here */
	INTCON1bits.NSTDIS = 1;
	
	/* Set Timer1 interrupt priority to 2 */
	IPC0bits.T1IP = 2; 
	/* Set Timer2 interrupt priority to 5 */
	IPC1bits.T2IP = 3;
	/* Set Timer3 interrupt priority to 6 (level 7 is highest) */
	IPC2bits.T3IP = 4;
	/* Set Timer4 interrupt priority to 3 */
	IPC6bits.T4IP = 3; 
	
	/* Set Change Notice interrupt priority to 4 */
	IPC4bits.CNIP = 7;
	
	/* Reset Timer1 interrupt flag */
	IFS0bits.T1IF = 0;
	/* Reset Timer2 interrupt flag */
	IFS0bits.T2IF = 0;
	/* Reset Timer3 interrupt flag */
	IFS0bits.T3IF = 0;
	/* Reset Timer4 interrupt flag */
	IFS1bits.T4IF = 0;
	/* Reset change notice interrupt flag */
	IFS1bits.CNIF = 0;
	
	/* Enable Timer1 interrupt */
	IEC0bits.T1IE = 0;
	/* Enable Timer2 interrupt (PWM time base) */
	IEC0bits.T2IE = 0;
	/* Enable Timer3 interrupt */
	IEC0bits.T3IE = 0;
	/* Enable Timer4 interrupt (replacement for Timer2 */
	IEC1bits.T4IE = 0;
	/* Enable CN interrupts */
	IEC1bits.CNIE = 0;
	
	return;
}

void Init_System( void )
{
	// 
	SR 		= 0x0000 ;
	CORCON 	= 0x0000;
	INTCON1 = 0x0000;
	INTCON2 = 0x0000;	//disable all Interrupt
	INTCON3 = 0x0000;
	INTCON4 = 0x0000;
	INTTREG = 0x0000;
	//disable all interrupt
	IEC0 = 0x0000 ;
	IEC1 = 0x0000 ;
	IEC2 = 0x0000 ;
	IEC3 = 0x0000 ;
	IEC4 = 0x0000 ;
	IEC5 = 0x0000 ;
	IEC6 = 0x0000 ;
	IEC8 = 0x0000 ;
	IEC9 = 0x0000 ;	

	IFS0 = 0x0000 ;
	IFS1 = 0x0000 ;
	IFS2 = 0x0000 ;
	IFS3 = 0x0000 ;
	IFS4 = 0x0000 ;
	IFS5 = 0x0000 ;
	IFS6 = 0x0000 ;
	IFS8 = 0x0000 ;
	IFS9 = 0x0000 ;	

	// // disable all peripheral moudle unused 
//	PMD1 = 0xFCDA ;		// enable  PWM UART1 ADC1 ; disable Timer1-5 QEI1 I2C UART2 SPI1 SPI2 
//	PMD2 = 0x0F00 ;		// disable all input capture , Enable 4 output compare
//	PMD3 = 0x0082 ;		// disable comparator CRC I2C2 , Enable compare module
//	PMD4 = 0x0000 ; // 0x0000 ;		// enable referance clock and CTMU module
//	PMD6 = 0x0000 ; // 0x0000 ;		// enable PWM1-3
//	PMD7 = 0x0000 ; // 0x0018 ;		// disable DMA, PTG(peripheral Trigger generator)

	PMD1 = 0xF4DA ;		// enable Timer1-5 UART1 ADC1
	PMD2 = 0x0F02 ;		// disable capture , Enable output compare
	PMD3 = 0x0082 ;		// disable all
	PMD4 = 0x0000 ;		// enable referance clock and CTMU module
	PMD6 = 0x0000 ;		// enable PWM1-3
	PMD7 = 0x0010 ;		// disable DMA, enable PTG
	//setting the interrupt using in program
	InitInterrupts();	
}


void Init_Clock( void )
{
	// f = (M + 2 )/( 2 * 2 ) * 7.37M = 140M
	PLLFBD = CLOCK_PLLFBD ; // M=65		
	CLKDIVbits.PLLPOST=0; 	// N2=2
	CLKDIVbits.PLLPRE =0;	// N1=2
	RCONbits.SWDTEN = 0;	// disalbe watch dog


#if ( CLOCK_TYPE == EXTERNAL_CLOCK )
	// Clock switch to incorporate PLL
	__builtin_write_OSCCONH( 0b011 );    			// Initiate Clock Switch to
	// External oscillator with PLL (NOSC=0b001)
	__builtin_write_OSCCONL( OSCCON | 0x01 ); 		// Start clock switching
	while( OSCCONbits.COSC != 0b011 );

#elif ( CLOCK_TYPE == INTERNAL_CLOCK )

	// Clock switch to incorporate PLL
	__builtin_write_OSCCONH( 0b001 );    			// Initiate Clock Switch to
	// External oscillator with PLL (NOSC=0b001)
	__builtin_write_OSCCONL( OSCCON | 0x01 ); 		// Start clock switching
	while( OSCCONbits.COSC != 0b001 );

#endif

	// Wait for PLL to lock
	while (OSCCONbits.LOCK!= 1);
}


void Init_Device()
{
	Init_Clock() ;		// switch system clock
	Init_System();		// initilize interrupts and enable used peripheral device
	Init_IO_Port() ;
	Init_ADC () ;
	Init_Compare () ;
	Init_UART() ;
	Init_Timer1 () ;
	Init_Timer2 () ;
	Init_Timer3 () ;
	Init_Timer4 () ;
	Init_Timer5 ();
	Init_PWM();
}