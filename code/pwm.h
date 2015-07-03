#ifndef __PWM_H__
    #define __PWM_H__

// Remark: the PWMxH and PWMxL was swaped.
#define  	H_PWM_L_OFF		0xC202 
#define  	H_OFF_L_ON		0xC382 
#define  	H_OFF_L_OFF		0xC302
#define  	H_PWM_L_PWM		0xC002
#define  	H_OFF_L_PWM		0xC102

#define		PWM_U	IOCON1	
#define		PWM_V	IOCON2	
#define		PWM_W	IOCON3	


void Init_PWM(void) ;



#endif
