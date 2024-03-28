#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
    P6DIR = 0b01111111;         // Define P6 como saída
    P6OUT = 0b00000000;         // Inicia P6 desligado

    //P1DIR = 0b00000000;       // Define P1.0 como entrada
    P1REN |= 0b00000010;        // Habilita o resistor de pull-up/pull-down para P1.1
    P1OUT |= 0b00000010;        // Configura P1.1 como pull-up (nível alto)

// teclado matricial
// P1.2 ... P1.5 - Saída (escrita)
// P2.3 ... P2.6 - Entrada (leitura)

    P1DIR |= 0b00111100;
    P2DIR |= 0b10000111;

    const unsigned char vetor[] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111};
    volatile unsigned int i;
    volatile unsigned char j = 0;

    while(1){
        // Verifica se o botão em P1.1 foi pressionado
        if((P1IN & 0b00000010) == 0)    // Verifica se o bit correspondente a P1.1 é 0 (botão pressionado)
        {
            switch(j)                   // Verifica o estado atual do LED
            {
            case 0:
                P6OUT = vetor[0];
                break;
            case 1:
                P6OUT = vetor[1];
                break;
            case 2:
                P6OUT = vetor[2];
                break;
            case 3:
                P6OUT = vetor[3];
                break;
            case 4:
                P6OUT = vetor[4];
                break;
            case 5:
                P6OUT = vetor[5];
                break;
            case 6:
                P6OUT = vetor[6];
                break;
            case 7:
                P6OUT = vetor[7];
                break;
            case 8:
                P6OUT = vetor[8];
                break;
            case 9:
                P6OUT = vetor[9];
                break;
            }

            while((P1IN & 0b00000010) == 0);  // Aguarda até que o botão seja solto, mantendo o loop
            for(i = 10000; i > 0; i--);         // Delay simples para debounce do botão

            j++;                                // Incrementa o estado do LED
            j &= 0b00001001;                    // Limita o valor de j entre 0 e 9
            }
        }
    }
}

