#include <msp430.h>
#include "def_principais.h"
#include "LCD.h"

// P6.0 a P6.3 --> DADOS
// P3.2 --> RST
// P3.3 --> ENABLE
// P3.4 --> BACKLIGHT



int main(void)
{
    unsigned int k,i,j = 0;
    unsigned char teclado[4][4] =  {{'1', '2', '3', '11'},
                                    {'4', '5', '6', '12'},
                                    {'7', '8', '9', '13'},
                                    {'14', '0', '15', '16'}};

    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    P1DIR |= 0b00111101;
    P1OUT |= 0b00111100;

    P2DIR &= 0b10000111;
    P2REN |= 0b01111000;
    P2OUT |= 0b01111000;

    P3DIR = 0xFF;
    P6DIR = 0xFF;

    inic_LCD_4bits();

    set_bit(CONTR_LCD, BkL);

    //escreve_LCD("Teclado + LCD!");
    cmd_LCD(0xC0,0);

    while(1){
        for(i=0; i<4; i++) // varredura coluna
        {
            clr_bit(P1OUT, i+2); // testa cada coluna

            for(j=0; j<4; j++) // varredura linha
            {
                if(!tst_bit(P2IN, j+3)) // testa a linha
                {
                    cpl_bit(P1OUT,0);
                    cmd_LCD(teclado[j][i],1);
                    while(!tst_bit(P2IN, j+3));
                }
            }
            set_bit(P1OUT, i+2);
        }
    }

    return 0;
}
