#include <msp430.h>
/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>


/* Variáveis globais */
volatile unsigned int rise_timestamp = 0;
volatile unsigned int fall_timestamp = 0;
volatile unsigned int interval = 0;
volatile unsigned int dist = 0;

void main(void)
{
    // Para o watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    TA0CTL = TASSEL_2 | MC_2 | TACLR;
        /* TASSEL_2 ->  Timer A clock source select: 2 - SMCLK
         * MC_2 ->  Timer A mode control: 2 - Continous up
         * TACLR ->  Timer A counter clear
         * Configuração da fonte do clock do timer 1 */

    TA0CCTL1 = CAP | CCIS_0 | CM_3 | SCS | CCIS_0 | CCIE;
    /* Timer tipo A:
     * CAP ->  Capture mode
     * CM_3 ->  Capture mode: both edges
     * SCS -> Capture sychronize
     * CCIS_0 -> Capture input select: 0 - CCIxA
     * CCIE -> Capture/compare interrupt enable
     * Configura comparador 0 do timer 1 para captura */

    P1DIR = BIT0 + BIT4; // P1.0 e P1.4 como saída

    /* Pull down em P1.2 e habilita TA0.1 capture compare */
    P1REN = BIT2;
    P1SEL = BIT2;

    __bis_SR_register(GIE);

    //P1OUT &= ~BIT0;

    while(1)
    {
        // pulso de 10us para iniciar leitura do sonar
        P1OUT |= BIT4;
        __delay_cycles(10);
        P1OUT &= ~BIT4;

        __delay_cycles(500000);

    }
}



// Timer0_A5 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(TA0IV,14))
  {
    case  0: break;                          // No interrupt
    case  2:                                 // CCR1 used

        if (TA0CCTL1 & CCI)                 /* Borda de subida */
        {
            rise_timestamp = TA0CCR1;
        }
        else                                /* Borda de descida */
        {
            fall_timestamp = TA0CCR1;
            interval = fall_timestamp - rise_timestamp;
            dist = interval / 58;
            if(dist < 20)
                P1OUT |= BIT0;
            else
                P1OUT &= ~BIT0;
        }

    case  4: break;                          // CCR2 not used
    case  6: break;                          // reserved
    case  8: break;                          // reserved
    case 10: break;                          // reserved
    case 12: break;                          // reserved
    case 14: P1OUT ^= 0x01;                  // overflow
             break;
    default: break;
  }
}
