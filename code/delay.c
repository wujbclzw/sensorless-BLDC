//filename:delay.c 
//author: Wu J.B
// delay functions for microchip
//delay n cycles
#include <xc.h>

void Delay(unsigned int n )
{
	unsigned int i ;
  	for( i = 0 ; i < n ; i++)                       	//  
  	{                       	//
		Nop();                	//  
  	}                       	//
}

// 70Mhz -- 2334
// delay 1 milisecond
void Delay_1ms( )
{
	Delay(7735) ;
}
//delay n milisecond
void Delay_nms(unsigned long n)
{
  	do                      	//  
  	{                       	//      Delay(1000);
		n = n-1;              	// 
		Delay_1ms() ;
  	}                       	//  the delay time is 1ms
  	while( n> 1);           	//
 	return;                 	//
}
