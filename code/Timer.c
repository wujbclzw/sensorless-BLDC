#include <xc.h>
#include "global.h"

// use to halt control system when motor was blocked
void Init_Timer1( void )
{
    T1CON = 0x0030;         // Timer reset , 64/60us
    IFS0bits.T1IF = 0;      // Reset Timer1 interrupt flag
    IEC0bits.T1IE = 1;      // Enable Timer1 interrupt
    TMR1 = 0x0000;
    PR1  = 0xFFFF;          // 199.929 ms - 46875   37500 - 40ms
    // T1CONbits.TON = 1;   // Enable Timer1 and start the counter
}

// use to counter time for control system
void Init_Timer2( void )
{
    T2CON = 0x0020;     // Timer reset , 64/35us
    IFS0bits.T2IF = 0;  // Reset Timer1 interrupt flag
    IEC0bits.T2IE = 0;  // Enable Timer1 interrupt
    TMR2 = 0x0000;
    PR2 = 0xFFFF;       // 50ms - 
    // T2CONbits.TON = 1;  // Enable Timer1 and start the counter
}

// 
void Init_Timer3( void )
{
    T3CON = 0x0020;     // Timer reset , 64/35us
    IFS0bits.T3IF = 0;  // Reset Timer1 interrupt flag
    IEC0bits.T3IE = 0;  // Enable Timer1 interrupt
    TMR3 = 0x0000;
    PR3 = 0xFFFF;       // 50ms - 
    // T3CONbits.TON = 1;  // Enable Timer1 and start the counter
}

void Init_Timer23()
{
	T3CON = 0 ;
	T2CON = 0 ;
	
	IFS0bits.T2IF = 0;  // Reset Timer1 interrupt flag
    IEC0bits.T2IE = 0;  // Enable Timer1 interrupt
	
	T2CONbits.T32 = 1; 

	T2CONbits.TCKPS = 0b01 ;
	TMR3HLD = 0x00; 
	TMR2 = 0x00; 
	PR3 = 0xFFFF; 
	PR2 = 0xFFFF; 
	IPC2bits.T3IP = 0x01; 
	IFS0bits.T3IF = 0; 
	IEC0bits.T3IE = 0; 
	
}


void Init_Timer4( void )
{
    T4CON = 0x0030;     // Timer reset , 256/60 us   4.267us
    IFS1bits.T4IF = 0;  // Reset Timer1 interrupt flag
    IEC1bits.T4IE = 0;  // Enable Timer1 interrupt
    TMR4 = 0x0000;
    PR4 = 0xffff;       // 50ms - 
    // T3CONbits.TON = 1;  // Enable Timer1 and start the counter
}


void Init_Timer5( void )
{
    T5CON = 0x0000;     // Timer reset , 64/35us
    IFS1bits.T5IF = 0;  // Reset Timer1 interrupt flag
    IEC1bits.T5IE = 0;  // Enable Timer1 interrupt
    TMR5 = 0x0000;
    PR5 = 350;       // 50ms - 
    // T3CONbits.TON = 1;  // Enable Timer1 and start the counter
}

