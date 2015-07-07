#include "global.h"
#include <xc.h>



#define		FLT_RAISING_CHK		1
#define		FLT_FALLING_CHK		1


// define in main.c
extern unsigned char  RunDirection  ;


unsigned char nTmpChar[3] , senABC[3] ,  fltABC[3] , nCalcSensor, nDelaySensor ;
int   cnt , nPeriod , i ;
unsigned int nDelayCycles , nlagCycles ;


// Initialize the variables 
// those vars are used for detecting sensor info by BEMF
void InitialBEMFVariables( void )
{
	fltABC[0] = 0 ; 
	fltABC[1] = 0 ; 
	fltABC[2] = 0 ; 
	senABC[0] = 0 ;
	senABC[1] = 0 ;
	senABC[2] = 0 ;

	cnt = 0 ;

	nCalcSensor  = 0 ;
	nDelayCycles = 0 ;
	nDelaySensor = 0;
}


// runtime : 7.5ns
void CompBEMF( void )
{

	cnt++ ;

	//if( nCalcSensor == 1 || nCalcSensor == 6 )
	nTmpChar[0] = CMSTATbits.C3OUT ;			// W
	nTmpChar[1] = CMSTATbits.C4OUT ;			// V
	nTmpChar[2] = CMSTATbits.C1OUT ;			// U

	// for ( i = 0; i < 2; ++i)
	{
		// Nop() ;
		// Nop() ;
		Nop() ;
		Nop() ;
		Nop() ;
		nTmpChar[0] += CMSTATbits.C3OUT ;			// W
		nTmpChar[1] += CMSTATbits.C4OUT ;			// V
		nTmpChar[2] += CMSTATbits.C1OUT ;			// U
	}


	for( i = 0 ; i< 3 ; i++)
	{
		// fltABC[i] = nTmpChar[i]<<1 ;
		fltABC[i] =  (nTmpChar[i]) ; // (fltABC[i]>>1)  +

		if(senABC[i] == 1)  
		{
			if( fltABC[i] > FLT_RAISING_CHK ) 
			{
				senABC[i] = 0 ; 
				// Test_IO2_Port = ~Test_IO2_Port ;

				// eastimate the period of change phase
				// the sensor delay is set to be Period/12 
				if(i == 0 ) 
				{
					nPeriod +=  (cnt - nPeriod ) >> 2 ;
					cnt = 0 ;
					nDelayCycles = (nPeriod >>4)  ;
				}
			}
		}
		else 
		{
			if( fltABC[i] < FLT_FALLING_CHK )	senABC[i] = 1 ;		
		}
	} 
	
	
	// combine sensor info according to direction flag
	if(RunDirection)
	{
		nCalcSensor = senABC[1] + (senABC[2] << 1) + (senABC[0] << 2) ;
	}
	else
	{
		nCalcSensor = senABC[2] + (senABC[0] << 1) + (senABC[1] << 2) ;
		nCalcSensor = (0x07 - nCalcSensor) ;
	}


	// delay for Period/12
	if(nCalcSensor != nDelaySensor)
	{	
		nlagCycles++ ;
		if (nlagCycles >= nDelayCycles)
		{
			nDelaySensor = nCalcSensor ;
			nlagCycles = 0 ;
		}
	}

}



void ChangePhase(void) 
{
	switch(nDelaySensor)
	{
		case 1:
			{
				PWM_W = H_OFF_L_OFF;
				Nop();
				Nop();				
				PWM_V = H_OFF_L_ON;
				PWM_U = H_PWM_L_OFF;				
				break ;
			}
		case 3:
			{
				PWM_U = H_OFF_L_OFF;
				PWM_V = H_OFF_L_ON;
				PWM_W = H_PWM_L_OFF;
				break ;
			}
		case 2:
			{
				PWM_V = H_OFF_L_OFF;				
				PWM_U = H_OFF_L_ON;
				PWM_W = H_PWM_L_OFF;
				break ;
			}
		case 6:
			{
				PWM_W = H_OFF_L_OFF;				
				PWM_U = H_OFF_L_ON;
				PWM_V = H_PWM_L_OFF;
				break ;
			}
		case 4:
			{
				PWM_U = H_OFF_L_OFF;				
				PWM_W = H_OFF_L_ON;
				PWM_V = H_PWM_L_OFF;
				break ;
			}
		case 5:
			{
				PWM_V = H_OFF_L_OFF;
				PWM_W = H_OFF_L_ON;				
				PWM_U = H_PWM_L_OFF;
				break ;
			}

		default:
			{
				PWM_U = H_OFF_L_OFF;
				PWM_V = H_OFF_L_OFF;
				PWM_W = H_OFF_L_OFF;
				break ;
			}
	}

}


