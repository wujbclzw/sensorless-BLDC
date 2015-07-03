#ifndef __GLOBAL_H__
    #define __GLOBAL_H__

// #if __XC16_VERSION == 1011
// #warning "XC16 v1.11 detected. It is recommended that a newer version of XC16 be used."
// #endif

#include "io.h"
#include "adc.h"
#include "uart.h"
#include "delay.h"
#include "Timer.h"
#include "sys.h"
#include "pwm.h"
#include "park.h"
#include "svgen.h"
#include "userparam.h"
#include "compareBEMF.h"


// ------ data type define
#define	uint	unsigned int
#define uint16	unsigned int 	
#define uchar	unsigned char 	


// led port  
#define LEDRED 	PORTBbits.RB7 
#define LEDYLW 	PORTBbits.RB8


// steering direction
#define DirectionBit		1


//Timer define
#define Timer1_Start 	T1CONbits.TON = 1
#define Timer2_Start 	T2CONbits.TON = 1
#define Timer23_Start 	T2CONbits.TON = 1
#define Timer3_Start 	T3CONbits.TON = 1
#define Timer4_Start 	T4CONbits.TON = 1
#define Timer5_Start 	T5CONbits.TON = 1

#define Timer1_Stop  	T1CONbits.TON = 0
#define Timer2_Stop  	T2CONbits.TON = 0
#define Timer23_Stop  	T2CONbits.TON = 0
#define Timer3_Stop  	T3CONbits.TON = 0
#define Timer4_Stop  	T4CONbits.TON = 0
#define Timer5_Stop  	T5CONbits.TON = 0

// define ADC and PWM enable
#define		Enalbe_ADC				AD1CON1bits.ADON = 1
#define		Disalbe_ADC				AD1CON1bits.ADON = 0
#define		Enable_PWM				PTCONbits.PTEN   = 1 
#define		Disable_PWM				PTCONbits.PTEN   = 0 
#define 	Enalbe_PWM1_Interrupt	IEC5bits.PWM1IE = 1 
#define 	Disalbe_PWM1_Interrupt	IEC5bits.PWM1IE = 0 

#define		Test_IO1_Port 	PORTAbits.RA4 
#define		Test_IO2_Port 	PORTBbits.RB4 


// global procedurce


#define ReadCompare7Timers(x)	{	\
					Nop();	\
					Nop();	\
					x =  CMSTATbits.C4OUT ; 	\
					Nop();	\
					Nop();	\
					x +=  CMSTATbits.C4OUT ; 	\
					Nop();	\
					Nop();	\
					x +=  CMSTATbits.C4OUT ; 	\
					Nop();	\
					Nop();	\
					x +=  CMSTATbits.C4OUT ; 	\
					Nop();	\
					Nop();	\
					x +=  CMSTATbits.C4OUT ; 	\
					Nop();	\
					Nop();	\
					x +=  CMSTATbits.C4OUT ; 	\
		}

// global variables from main
// extern unsigned int nDetat ; 

#endif