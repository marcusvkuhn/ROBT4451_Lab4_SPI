#include <msp430.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "usciSpi.h"
#include "usciUart.h"


#define TRANSMIT_CHAIN_CHAR 1
#define TRANSMIT_CHAIN_BUFFER 0

void main(void){
    //----- UART Init
    usciA1UartInit();
    //----- SPI Init
    usciB1SpiInit(1,1,0x02,0);

    char rxString[50];
    int buffLen;
    char rxBuffer[BUFFER_SZ];

    if(TRANSMIT_CHAIN_CHAR){

        int i;
        //---------- enable interrupts
        __enable_interrupt();   // enable global interrupts
        UCB1IE = 0;             // disable SPI interrupts
        UCA1IE |= UCRXIE;       // enable UART (A1) interrupts

        //---------- blink LED
        P1DIR |= 0x01;                  // configure P1.0 as output
        do {
            P1OUT ^= 0x01;              // toggle P1.0
            for(i=10000; i>0; i--);     // delay
        } while(1);
    }

    else if(TRANSMIT_CHAIN_BUFFER){

        do {
            //---------- disable interrupts
            //__enable_interrupt();   // enable global interrupts
            UCB1IE = 0;             // disable SPI interrupts
            UCA1IE &= ~UCRXIE;       // enable UART (A1) interrupts

            usciA1UartGets(rxString);       // wait for a string
            buffLen = strlen(rxString);
            numStringToInt(rxString, rxBuffer); // atoi on each byte in rxString and store in buffer
            usciB1SpiTxBuffer(rxBuffer, buffLen); // transmit buffer of integers using usciB1SpiPutChar
        } while (1);
    }
}