#include "global.h"
#include <xc.h>



// FICD  Communicate on PGEC1 and PGED1 & JTAG is disabled
_FICD( ICS_PGD2 & JTAGEN_OFF);
// FPOR    I2C1 mapped to SDA1/SCL1 pins & I2C2 mapped to SDA2/SCL2 pins & WDT Window is 25% of WDT period
_FPOR( ALTI2C1_OFF & ALTI2C2_OFF & WDTWIN_WIN25 ) ;
// FWDT  Watchdog Timer Postscaler bits (1:32,768) & Prescaler bit (1:128) & ..
_FWDT(WDTPOST_PS32768 & WDTPRE_PR128 & PLLKEN_ON & WINDIS_OFF & FWDTEN_OFF);
// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF & PWMLOCK_OFF);

// Enable Clock Switching and Configure Primary Oscillator in XT mode
#if ( CLOCK_TYPE == EXTERNAL_CLOCK )
	_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT & IOL1WAY_OFF);
#elif ( CLOCK_TYPE == INTERNAL_CLOCK )
	_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE & IOL1WAY_OFF);
#endif

// General Segment Write-Protect bit off  & code-protect off
_FGS(GWRP_OFF & GCP_OFF);



#define 	LOCKTIME   		2000		// PWM_FREQUENCE - 1s
#define		ENDSPEED		800			// (1000 rpm) - 72.8 * N ;  N -- polar number
#define		CYCLES_PER_ONE_INCREASERATE  	50


unsigned int StartUp_Ramp  ;
unsigned int nLoopCounter , StartUp_Lock_Time , lastIndex;

// 1,5,4,6,2,3
unsigned char TrunDirect[2][8] = { {0,3,6,2,5,1,4,0} , {0,5,3,1,6,4,2,0} };

unsigned char TrunOrder ;

union   
{
	struct
	{
		unsigned RunMotor:1;  /* run motor indication */
		unsigned OpenLoop:1;  /* open loop/clsd loop indication */
		unsigned LowVoltage:1; /* btn 1 pressed indication */
		unsigned Btn2Pressed:1; /* btn 2 pressed indication */
		unsigned ChangeMode:1; /* mode changed indication - from open to clsd loop */
		unsigned ChangeSpeed:1; /* speed doubled indication */
		unsigned OffsetFlag:1;
		unsigned ChangePhaseFlg:1 ;
		unsigned RestartFlg:2 ;
		unsigned rsvd:6 ;
	} bit;
	unsigned int Word;
} uGF;        // general flags


tParkParm    	ParkParm;
tSincosParm  	SincosParm;
tSVGenParm 		SVGenParm;  

extern unsigned char nDelaySensor  , nCalcSensor , fltABC[3] ; //

unsigned char 	RunDirection ;
unsigned char 	sensor ; // , nCmpSensor ,lstCmpSensor , bCmp2nd ;
unsigned int 	duty , setDuty ;

unsigned int 	rdAD , nCnt ; 	// ,nTmp 

int nfltCurr , nBatteryVolt;

// unsigned int nDetat ;
void StopMotor(void) ;
void ReStartMotor(void ) ;
void InitControlParameter(void) ;
void CalcParkAngle(void ) ;
void ChangePhase2(unsigned char nSensor) ;


// #define Read_SenSor(x)  { x = (PORTC) & 0x0038 ;	x >>= 3; 	}

int main()
{

	//step 1 Initial device 
	Init_Device();

	// step 2 charge capacitor  and turn on relay 
	LEDRED = 1 ;

	// step 3 initial variables
	uGF.Word = 0 ;
	InitControlParameter() ;
	InitialBEMFVariables() ;

	// step 4 enable ADC , PWM and global interrupts 
	// PWM will trigger adc interrupts
	Enalbe_ADC ;
	Enable_PWM ;
	EnableInterrupts() ;
	// Enable CN interrupts 
	// IFS1bits.CNIF = 0;
	// IEC1bits.CNIE = 1;

	// step 5 main loop  wait for ADC interrupts
	//  rdAD = 0 ;

	LEDRED = 0 ;
	LEDYLW = 0 ;

	// waiting for Battary detection
	Delay_nms(100);

	while(1)
	{

		StopMotor() ;
		InitControlParameter() ;

		while(uGF.bit.LowVoltage)
		{
			Delay_nms(250);
			LEDRED = 0 ;
			Delay_nms(250) ;
			LEDRED = 1 ;
		}

		// start motor button
		if(!uGF.bit.RunMotor)
		{
			// if(0 == uGF.bit.RestartFlg)
			{
				while( rdAD > 100 )  { LEDRED = 1 ;  LEDYLW = 0 ; }                //wait here until button 1 is pressed 
				while( rdAD < 200 )  { LEDYLW = 1 ;  LEDRED = 0 ; }                   //when button 1 is released 
			}
			// Inital offsets
			uGF.bit.RunMotor = 1;               //then start motor
		}		
		// initilize variable for rectangel control method
		ReStartMotor() ;		

		LEDRED = 1 ; 
		LEDYLW = 1 ;
		// idle loop , waiting for ADC interrupts
		while(uGF.bit.RunMotor)
		{
			// SCI send data
			// if (uGF.bit.OpenLoop == 0)	//  (nLoopCounter & 0x003) == 0x0000)
			// {
			// 	// if(nLoopCounter != lastIndex)
			// 	{
			// 		// SCI_Send_Char( fltABC[0] ) ;
			// 		//SCI_Send_Char( nCalcSensor) ;
			// 		// SCI_Send_Char( sensor ) ;
			// 	}
			// }

			
			// check for quit condition
			if ( rdAD < 150 )
			{
				uGF.bit.RunMotor = 0 ;
				break ;
			}

		}
	}

	return 0 ;
}


void DoContorl()
{
	if(uGF.bit.OpenLoop)
	{
		ParkParm.qVq = ( 1400 + (StartUp_Ramp<<1 ) + ( ( LOCKTIME - StartUp_Lock_Time )>>1 ) );
        ParkParm.qVd = 0 ;
	}
}



void __attribute__ ( (interrupt, no_auto_psv) ) _AD1Interrupt( void )
{
	IFS0bits.AD1IF = 0 ;

	// Speed control plotmeter
	rdAD = ADC1BUF0 ;

	// Battery voltage check: if voltage is too low , stop motor.
	nBatteryVolt += ((int)ADC1BUF3  - nBatteryVolt)>>3 ;
	if( nBatteryVolt < CRITICAL_BAT_VOLT ) 
	{
		uGF.bit.LowVoltage = 1 ;
		uGF.bit.RunMotor = 0 ;
	}

	// nCurrent = ADC1BUF1 ;
	// nfltCurr += ((int)ADC1BUF1  - nfltCurr)>>3 ;
	// if current is too lager
	// if(nfltCurr > MAX_CURRENT)	uGF.bit.RunMotor = 0 ;	

	nLoopCounter ++ ;
	
	if( uGF.bit.RunMotor )
	{

		if(uGF.bit.OpenLoop == 1)
		{		

			// ClarkePark() ;							// Ia , Ib --> Ialpha , Ibeta
			// Estim() ; 				// stator angle estimate
	
			DoContorl() ; 				// control Iq	make motor start with constant current
	
			CalcParkAngle() ;			// angle add
	
			/* the angle is given by parkparm */
			SincosParm.qAngle = ParkParm.qAngle;
			
			SinCos();
	
			ParkParm.qSin = SincosParm.qSin;
			ParkParm.qCos = SincosParm.qCos;
	
			InvPark() ;					// Valpha ,Vbeta
	
			CalcRefVec() ;				// V1,V2,V3
	
			CalcSVGen() ;				// PWM1, PWM2, PWM3
	
			if(uGF.bit.ChangeMode == 1)
			{

				Disable_PWM ;
				PWMCON1bits.CAM = 0 ;
				PWMCON1bits.DTC = 2 ;		// disable dead area
				PWMCON2bits.CAM = 0 ;
				PWMCON2bits.DTC = 2 ;		// disable dead area
				PWMCON3bits.CAM = 0 ;
				PWMCON3bits.DTC = 2 ;		// disable dead area

				PWM_U = H_OFF_L_OFF ;
				PWM_V = H_OFF_L_OFF ;
				PWM_W = H_OFF_L_OFF ;
	
				duty = MOTOR_START_DUTY ;
				PDC1 = duty ;
				PDC2 = duty ;
				PDC3 = duty ;
				nLoopCounter = 0 ;
				uGF.bit.OpenLoop = 0 ;

 				PR1 = PWM_Period  ;
				IFS0bits.T1IF = 0;      // Reset Timer1 interrupt flag
				// IEC0bits.T1IE = 1;      // Enable Timer1 interrupt

				Enable_PWM ;			
				// Timer1_Start ;
				// Timer2_Start ;
			}
		}
		else
		{
			// adjust duty accorrding to potormeter

			if( 0x1F == (nLoopCounter & 0x1F) )
			{
				setDuty = (rdAD<<2) - 300 ;

				if(setDuty < MOTOR_START_DUTY ) setDuty = MOTOR_START_DUTY ;
				else if(setDuty > MAX_DUTY ) setDuty = MAX_DUTY ;

				if (duty < setDuty)			duty ++ ;
				else if(duty > setDuty)		duty -- ;

				PDC1 = duty ;
				PDC2 = duty ;
				PDC3 = duty ;
				// TRIG1 	= duty - 100 ; 
			}
			if(nLoopCounter > WAITCYCLES_COMPBEMF )		uGF.bit.ChangePhaseFlg = 1 ; 
		}

	}
}

// PWM interrupt
// The trigger is enabled to generate the PWM interrupt
// this interrupt is enabled when the mosfet is on 

void __attribute__ ( (interrupt, no_auto_psv) ) _PWM1Interrupt( void )
{
	
	// Test_IO2_Port = ~Test_IO2_Port ;
	IFS5bits.PWM1IF 	= 0 ;			//clear interrupt flag
	
	if( ( uGF.bit.OpenLoop == 0 ) && (uGF.bit.RunMotor == 1) )
	{

		CompBEMF() ;

		if(uGF.bit.ChangePhaseFlg)
		{
			if (sensor != nDelaySensor)
			{
				ChangePhase() ;
				sensor = nDelaySensor ;
			}
		}
	}

	// Test_IO2_Port = ~Test_IO2_Port ;
}




void CalcParkAngle(void )
{
	static uint nCycleNumber = 0 ;

	if(uGF.bit.OpenLoop)
	{
		// begin wiht the lock sequence, for field alligniament
		if(StartUp_Lock_Time < LOCKTIME)
		{
			StartUp_Lock_Time ++ ;
		}
		else if ( StartUp_Ramp < ENDSPEED)
		{
			if( nCycleNumber >= CYCLES_PER_ONE_INCREASERATE )
			{
				StartUp_Ramp ++ ;
				nCycleNumber = 0; 
			}
			else
			{
				nCycleNumber ++ ;
			}
		}
		else
		{
			uGF.bit.ChangeMode = 1;
			// uGF.bit.OpenLoop = 0;
			StartUp_Ramp = ENDSPEED ;
		}

		if(RunDirection)
			ParkParm.qAngle +=  StartUp_Ramp ;
		else
			ParkParm.qAngle -=  StartUp_Ramp ;
	}
}


void InitControlParameter(void)
{
	// current measure calibrate
	rdAD = 1000 ;

	// bettery voltage detection
	nBatteryVolt = 1000 ;

	// park 
	ParkParm.qIa = 0 ;
	ParkParm.qIa = 0 ;
	ParkParm.qAngle = 0;
	ParkParm.qVq  = 8000 ;				// start voltage
	ParkParm.qVd  = 0 ; 

	// SVPWM period setting
	SVGenParm.iPWMPeriod = PWM_Period ;

}


void ReStartMotor(void )
{

	PWM_U = H_PWM_L_PWM ;
	PWM_V = H_PWM_L_PWM ;
	PWM_W = H_PWM_L_PWM ;

	// enable PWM interrupt
	IFS5bits.PWM1IF 	= 0 ;
	IEC5bits.PWM1IE 	= 1 ;

	// motor running direction
	RunDirection = DirectionBit ;

	// variable initial
	sensor = 0 ;
	nCnt = 0 ;

}


void StopMotor(void )
{

	Timer1_Stop ;
	Timer2_Stop ;
	TMR1 = 0 ;
	TMR2 = 0 ;

	Disable_PWM ;

	PWM_U = H_OFF_L_OFF ;
	PWM_V = H_OFF_L_OFF ;
	PWM_W = H_OFF_L_OFF ;
	

	PWMCON1	= 0x0604;					// enable triggle interrupt
	PWMCON2	= 0x0204;
	PWMCON3	= 0x0204;

	// disable PWM interrupt
	IEC5bits.PWM1IE 	= 0 ;
	IFS5bits.PWM1IF 	= 0 ;
	
	Nop();
	Nop();
	Nop();

	Enable_PWM ;


	Timer1_Stop;
	Timer2_Stop;
	Timer3_Stop;
	Timer4_Stop;
	Timer5_Stop;
	TMR1 = 0X0000;
	TMR2 = 0X0000;
	TMR3 = 0X0000;
	TMR4 = 0X0000;
	TMR5 = 0X0000;

	// variables initilize
	// uGF.Word = 0;                   // clear flags
	uGF.bit.RunMotor = 0 ;
	uGF.bit.ChangeMode = 0 ;
	uGF.bit.ChangePhaseFlg = 0;
	/* setup to openloop */
	uGF.bit.OpenLoop = 1;
	
	
	StartUp_Ramp = 0 ;
	StartUp_Lock_Time = 0 ;
	nLoopCounter = 0 ;

	TrunOrder = 127 ;
}


// end of file main.c
