#include <msp430.h> 
#include "def_principais.h"
#include "LCD.h"

//informações para criar novos caracteres, armazenadas na memória flash
const unsigned char novos_caract[]={0b00000001,//0
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00011111,
                                            0b00011111,//1
                                            0b00010000,
                                            0b00010000,
                                            0b00010000,
                                            0b00010000,
                                            0b00010000,
                                            0b00010000,
                                            0b00011111,//2
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,//3
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00011111,//4
                                            0b00000000,
                                            0b00000000,
                                            0b00000000,
                                            0b00000000,
                                            0b00000000,
                                            0b00011111,
                                            0b00011111,//5
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00000001,
                                            0b00011111,
                                            0b00011111,//6
                                            0b00000000,
                                            0b00000000,
                                            0b00000000,
                                            0b00000000,
                                            0b00000000,
                                            0b00000000,
                                            0b00011111,//7
                                            0b00010000,
                                            0b00010000,
                                            0b00010000,
                                            0b00010000,
                                            0b00010000,
                                            0b00011111};

const unsigned char nr_grande[10][4] = { {0x01, 0x02, 0x4C, 0x00}, //nr. 0
                                                {0x20, 0x7C, 0x20, 0x7C}, //nr. 1
                                                {0x04, 0x05, 0x4C, 0x5F}, //nr. 2
                                                {0x06, 0x05, 0x5F, 0x00}, //nr. 3
                                                {0x4C, 0x00, 0x20, 0x03}, //nr. 4
                                                {0x07, 0x04, 0x5F, 0x00}, //nr. 5
                                                {0x07, 0x04, 0x4C, 0x00}, //nr. 5
                                                {0x06, 0x02, 0x20, 0x03}, //nr. 7
                                                {0x07, 0x05, 0x4C, 0x00}, //nr. 8
                                                {0x07, 0x05, 0x20, 0x03}};//nr. 9

//-----------------------------------------------------------------------------------------------------------
void cria_novos_caract()//criação dos 8 novos caracteres
{
    unsigned char i, k, j=0, n=0x40;

    for(i=0;i<8;i++)
    {
        cmd_LCD(n,0);       //endereco base para gravar novo segmento
        for(k=0;k<7;k++)
            cmd_LCD(novos_caract[k+j],1);

        cmd_LCD(0x00,1);    //apaga ultima posição do end. da CGRAM para evitar algum dado espúrio
        j += 7;
        n += 8;
    }
}
//-----------------------------------------------------------------------------------------------------------
void escreve_BIG(unsigned char end, unsigned char nr)//escreve um número grandes no LCD
{
     cmd_LCD(end,0);                                //endereco de incio de escrita (1a linha)
     cmd_LCD(nr_grande[nr][0],1);
     cmd_LCD(nr_grande[nr][1],1);
     cmd_LCD(end+64,0);                             //desloca para a 2a linha
     cmd_LCD(nr_grande[nr][2],1);
     cmd_LCD(nr_grande[nr][3],1);
}

int state = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P6DIR = 0xFF;               // P6.0-P6.3 dedicados aos dados do LCD
    P3DIR = 0xFF;               // P3.2-P3.4 dedicados ao controle do LCD

    // configura interrupções botão 1 (alterna o que é mostrado no display)
    P1DIR = 0b00000000;         // Define P1.1 como entrada
    P1REN |= 0b00000010;        // Habilita o resistor de pull-up/pull-down para P1.1
    P1OUT |= 0b00000010;        // Configura P1.1 como pull-up (nível alto)
    P1IES = 0b00000010;         // Configura borda da interrupção no P1.1
    P1IE = 0b00000010;          // Habilita interrupção externa no P1.1

    // configura interrupções botão 2 (inicia ou para a contagem do cronômetro)
    P2DIR = 0b00000000;         // Define P2.1 como entrada
    P2REN |= 0b00000010;        // Habilita o resistor de pull-up/pull-down para P2.1
    P2OUT |= 0b00000010;        // Configura P2.1 como pull-up (nível alto)
    P2IES = 0b00000010;         // Configura borda da interrupção no P2.1
    P2IE = 0b00000010;          // Habilita interrupção externa no P2.1

    __bis_SR_register(GIE);     // Habilita interrupções globais

    while(1)
    {
        switch(state)
        {
            case 0:             // início
                escreve_BIG(0x8E, 0);
                break;
            case 1:             // data (dd/mm/aa)
                escreve_BIG(0x8E, 1);
                break;
            case 2:             // horário (hh:mm:ss)
                escreve_BIG(0x8E, 2);
                break;
            case 3:             // cronômetro (sss:ms)
                escreve_BIG(0x8E, 3);
                break;
        }
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    state += 1;  // troca estado
    if(state>3)
        state = 1;
    __delay_cycles(1000);
    P1IFG = 0; // limpa historico interrupção
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    __delay_cycles(1000);
    P2IFG = 0; // limpa historico interrupção
}
