#include <xc.h>
#include "global.h"

void Init_UART()
{
	// Config IO port 
    TRISBbits.TRISB6 = 0 ;	//U1Rx  input		//PGEC2 - RP38
	TRISBbits.TRISB5 = 1 ;	//U1TX  output		//PGED2 - RP37
	ODCB &= 0xFFDF ;
	// remapping U1RX to RP34   , U1Tx to RP35
	RPINR18bits.U1RXR = 37;
	_RP38R = 1;		// or RPOR0bits.RP35R = 1 ;
	
	// interrupt priority
	_U1RXIP = 4 ;
	_U1TXIP = 4 ;
    IFS0bits.U1TXIF = 0;    // Clear the Transmit Interrupt Flag
    IEC0bits.U1TXIE = 0;    // Enable Transmit Interrupts
    IFS0bits.U1RXIF = 0;    // Clear the Recieve Interrupt Flag
    IEC0bits.U1RXIE = 0;    // Enable Recieve Interrupts
	
	// See section 19.3.1 of datasheet.
    //  U1BRG = (Fcy/(16*BaudRate))-1
    //  U1BRG = (70M/(16*128000))-1
    //  U1BRG = 33
    U1BRG = SCI_BUAD_REG_SETTING ;            // 75 - 57600  ,33-128000
	U1STA = 0x0000 ;
	U1MODE = 0x0000 ;
    U1MODEbits.UARTEN = 1;  // And turn the peripheral on
    U1STAbits.UTXEN = 1;
}


void SCI_Send_Char(unsigned char ch)
{
	//int cnt = 0;
	while(U1STAbits.UTXBF) 
	{
		;
	}
	if(U1STAbits.UTXBF==0)
	{
		U1TXREG = ch;
	}
}


void SCI_Send_Int(unsigned int num)
{
	unsigned char tmp;
	tmp = (unsigned char)num ;
	SCI_Send_Char(tmp );
	tmp = (unsigned char)(num >> 8 ) ;
	SCI_Send_Char(tmp ) ;
}


void SCI_Send_String( char *s)
{
	while(*s != '\0')
	{
		SCI_Send_Char(*s) ;
		s++ ;
	}
}
