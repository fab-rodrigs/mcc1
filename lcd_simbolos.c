#include <msp430.h> 
#include "def_principais.h"
#include "LCD.h"

const unsigned char customChar1[] = {
  0b00000,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00000
};

const unsigned char customChar2[] = {
  0b00000,
  0b11011,
  0b11011,
  0b00100,
  0b01110,
  0b01110,
  0b01010,
  0b00000
};

int main(void)
{
    unsigned char k,i,j = 0;

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P6DIR = 0xFF;           // P6.0-P6.3 dedicados aos dados do LCD
    P3DIR = 0xFF;           // P3.2-P3.4 dedicados ao controle do LCD

    set_bit(CONTR_LCD, BkL);
    inic_LCD_4bits(); // inicializa o LCD

    // Definindo os caracteres personalizados na CGRAM
    cmd_LCD(0x40,0); // endereça a primeira posição de MEM da CGRAM
    for(k=0; k<8; k++)
        cmd_LCD(customChar1[k],1);   // elementos do vetor para formar o novo símbolo
    for(k=0; k<8; k++)
        cmd_LCD(customChar2[k],2);

    // Posicionando o cursor e escrevendo os caracteres personalizados
    cmd_LCD(0x80,0); // posiciona o cursor na primeira linha do LCD
    cmd_LCD(0,1);    // imprime o primeiro caractere personalizado

    cmd_LCD(0x81,0); // posiciona o cursor na segunda posição do LCD
    cmd_LCD(1,1);    // imprime o segundo caractere personalizado

    while(1){
    }

    return 0;
}
