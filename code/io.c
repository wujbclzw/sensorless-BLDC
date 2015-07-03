#include "global.h"
#include <xc.h>


void Init_IO_Port ()
{
	
	TRISA = 0x0003 ;	// output, except AN0/1 -- RA0/1 	 
	PORTA = 0x0000 ;	// output low 
	ODCA  = 0xFFEF ;	// open-drain   	RA4 

	TRISB = 0x000F ;	// output , except AN2-AN5 -- RB0-3    RB4 as test IO 
	PORTB = 0x0000 ;	// output low 
	ODCB  = 0x024F ;	// open-drain RB7-8 LED out  RB10-15 ad PWM out  RB4 as test IO Rb5 as uart1 tx

	TRISC = 0x0007 ;	// output , except AN6/7/8 -- RC0/1/2    
						// RC7 - output for PPM						
	PORTC = 0x0000 ;	// output low  
	ODCC  = 0xFF7F ;	// open-drain 
	
	// Analog Select  (set in file "ADC.c")
	ANSELA = 0x0000;
	ANSELB = 0x0000;
	ANSELC = 0x0000;
	// // input change notification Interrupt
	CNENA = 0x0000;
	CNENB = 0x0000;
	CNENC = 0x0000;	
	CNPUA = 0x0000;
	CNPUB = 0x0000;
	CNPUC = 0x0000;
	CNPDA = 0x0000;
	CNPDB = 0x0000;
	CNPDC = 0x0000;
	
	// // set IO port interrupt enable bits
	// CNPUCbits.CNPUC3 = 1;
	// CNPUCbits.CNPUC4 = 1;
	// CNPUCbits.CNPUC7 = 1;
	
	// CNPDCbits.CNPDC3 = 1;
	// CNPDCbits.CNPDC4 = 1;
	// CNPDCbits.CNPDC7 = 1;
	
	// CNENCbits.CNIEC3 = 1;		// sensor A B C
	// CNENCbits.CNIEC4 = 1;		
	// CNENCbits.CNIEC7 = 1;
}


