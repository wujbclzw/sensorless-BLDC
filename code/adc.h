#ifndef __ADC_H__
    #define __ADC_H__

void	Init_ADC( ) ;
// unsigned int ADC_RdDWJ() ;
// unsigned int ADC_CurrentBus();
// unsigned int ADC_CurrentU();
// unsigned int ADC_CurrentV();
// unsigned int ADC_CurrentW();


// unsigned int ADC_BackEMF_U();
// unsigned int ADC_BackEMF_V();
// unsigned int ADC_BackEMF_W();
// unsigned int ADC_BackEMF_Mid();

void Init_Compare() ;

// #define COMPARE_A_MID	{	CM4CONbits.CCH = 0b00 ;		Delay(1) ;	}		// C4IN1-/AN7
// #define COMPARE_B_MID	{	CM4CONbits.CCH = 0b01 ;		Delay(1) ;  }		// OA1/AN3
// #define COMPARE_C_MID	{	CM4CONbits.CCH = 0b10 ;		Delay(1) ;	}		// OA2/AN0


#endif
