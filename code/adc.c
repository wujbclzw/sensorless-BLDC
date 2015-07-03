#include<xc.h>
#include "global.h"



// 设置为同时采样4个通道 
// A4  -- 0通道 -- V相电流
// AN0 AN1 AN2 -- 123 通道 -- W相反电动势 / 电位计 / U相电流

void Init_ADC ()
{

	// AD1CON1bits.ADON = 1;	//start ADC sample

	// // set port configration
	ANSELA = ANSELB = ANSELC = 0x0000 ;
	ANSELA |= 0x0003 ;	// RA0_AN0  RA1_AN1
	ANSELB |= 0x000F ;	// RB0-3
	ANSELC |= 0x0007 ;	// RC0 RC1 RC2
	
	// //configure ADC register
	AD1CON1 = 0 ;
	// // Signed fractional (DOUT = sddd dddd dd00 0000)
    AD1CON1bits.FORM = 0;    
	// // PWM primary Special Event Trigger ends sampling and starts conversion
	AD1CON1bits.SSRC = 3;
	AD1CON1bits.SSRCG = 0;
 //    // Simultaneous Sample Select bit (only applicable when CHPS = 01 or 1x)
    AD1CON1bits.SIMSAM = 1;  
 //    // Sampling begins immediately after last conversion completes. 
 //    // SAMP bit is auto set.
    AD1CON1bits.ASAM = 1;  

    AD1CON2 = 0;
    // Samples CH0, CH1 simultaneously (when CHPS = 1x)
    AD1CON2bits.CHPS = 2;  

    AD1CON3 = 0;
 //    // A/D Conversion Clock Select bits = 9 * Tcy
    AD1CON3bits.ADCS = 12;  


 //     /* ADCHS: ADC Input Channel Select Register */
    AD1CHS0 = 0;
 //    // CH0 is AN3 for POT
    AD1CHS0bits.CH0SA = 3;

    // CH1 positive input is AN0, CH2 positive input is AN1, CH3 positive input is AN2  if Ch123A = 0
    // if opamp is enable ch1 -- OA2 , CH2 --AN1  CH3 --AN2 when CH123SA = 0 ;
    // referance : DS70000657H-page 331
    AD1CHS123 = 0 ;
    AD1CHS123bits.CH123SA = 0;

  
 // /* ADCSSL: ADC Input Scan Select Register */
    AD1CSSL = 0 ;
    AD1CSSH = 0 ;

 //    // Turn on A/D module
 	AD1CON1bits.ADON = 0;	//start ADC sample

 // 	// AD Interruput
 	IFS0bits.AD1IF = 0 ;
 	IEC0bits.AD1IE = 1 ;

}



void Init_Compare()
{
	CMSTAT = 0x0000 ;
	CVRCON = 0 ;

	// Comparator 1
	CM1CON = 0x8000;
	// CM1CONbits.CON = 1 ;	//Enalbe comparator
	CM1CONbits.COE = 1 ;	// output disable
	_RP55R = 0x18 ;
	// CM1CONbits.CPOL = 0 ; 	// output is not inverted
	// CM1CONbits.OPMODE = 0 ; //disable AMp 
	// CM1CONbits.CREF = 0 ;
	// CM1CONbits.CCH = 0 ;	// 

	CM1MSKSRC = 0 ;
	CM1MSKCON = 0 ;
	CM1FLTR  = 0x000B ;

	// Comparator 3 
	CM3CON = 0x8000;

	CM3MSKSRC = 0 ;
	CM3MSKCON = 0 ;
	CM3FLTR  = 0x000B ;

	// Comparator 4
	CM4CON = 0x8000;

	CM4MSKSRC = 0x0000 ;
	CM4MSKCON = 0x0000 ;
	CM4FLTR = 0x000B ;
	

	// AMP 2
	CM2CON = 0x0000 ;
	
	// Test_IO2 tied to c4out
	// _RP36R = 0b110010 ;
	
}



















