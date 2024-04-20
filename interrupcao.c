#include <msp430.h> 

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	P1DIR = 0b00000001;       // configurar led como saída

	// configura interrupções
	P1REN |= 0b00000010;        // habilita resistor up down
	P1OUT |= 0b00000010;        // habilita pull-up
	P1IES = 0b00000010;                   // configura borda da interrupção
	P1IE = 0b00000010;                    // habilita interrupção externa no P1.1
	__bis_SR_register(GIE);     // habilita interrupções globais

	while(1);
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    	int i;
    	P1OUT ^= 0b00000001;  // piscar LED
    	for(i=1000000; i>0; i--);     //delay
    	P1IFG = 0; // limpa historico interrupção
}
