#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR |= BIT2+BIT3;
	P1SEL |= BIT2+BIT3;
	TA0CCR0 = 20900-1;
	TA0CCTL1 = OUTMOD_7;
	TA0CCR1 = 523;
	TA0CCTL2 = OUTMOD_7;
	TA0CCR2 = 128;
	TA0CTL = TASSEL_2 + MC_1 + TACLR;

	__no_operation();
	
	while(1)
	{
	    TA0CCR1 = 0;
	    __delay_cycles(250000);
	    TA0CCR1 = 523;
	    //if(TA0CCR1 > 523)
	    //{
	    //    TA0CCR1 = 0;
	    //}
	}

	return 0;
}
