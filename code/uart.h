#ifndef __UART_H__
    #define __UART_H__
void 	SCI_Send_Char(unsigned char ch);
void 	SCI_Send_Int(unsigned int num);
void 	SCI_Send_String( char *s);
void	Init_UART( ) ;
	
#endif
