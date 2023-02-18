/* 
 * File:   main.c
 * Author: gssan
 *
 * Created on 7 de febrero de 2023, 08:47 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "Config.h"
#include <libpic30.h>
#include <p33FJ32MC202.h>
#include "Uart.h"
#include "ADC.h"
char caracter = 'a';
uint16_t entero = 55;
double p = 3.1416;
char ss[] = "hola mundo";

char rx;

void __attribute__ ((interrupt,no_auto_psv)) _U1RXInterrupt(void){

    if (U1STAbits.OERR ==1) {
    U1STAbits.OERR =0;
    }
    else if((U1STAbits.FERR==0)&&(U1STAbits.PERR==0)){
    
        
    rx = (char) U1RXREG;
    }

    
    switch(rx){
    
        case 'a':
            LATBbits.LATB15 = 1;
            break;
        case 'b':
            LATBbits.LATB15 = 0;
            break;    
        case 'c':
            LATBbits.LATB14 = 1;
            break;
        case 'd':
            LATBbits.LATB14= 0;
            break;
        case 'e':
            LATBbits.LATB13 = 1;
            break;
        case 'f':
            LATBbits.LATB13 = 0;
            break;
        case 'g':
            LATBbits.LATB12 = 1;
            break;
        case 'h':
            LATBbits.LATB12 = 0;
            break;
    }
IFS0bits.U1RXIF = 0;
}
uint16_t adc_read[6];
int main(int argc, char** argv) {
    TRISB &= ~((1<<15)|(1<<14)|(1<<13)|(1<<12));
      ADC_Init(bits12);
    UART_dsPIC_Init(9600);
  
    for(;;){
        
        adc_read[0] = AnalogRead(An0);
        adc_read[1] = AnalogRead(An1);
        adc_read[2] = AnalogRead(An2);
        adc_read[3] = AnalogRead(An3);
        adc_read[4] = AnalogRead(An4);
        adc_read[5] = AnalogRead(An5);
        printf("%u,%u,%u,%u,%u,%u\n\r",adc_read[0],adc_read[1],adc_read[2],adc_read[3],adc_read[4],adc_read[5]);
        __delay_ms(500);
    }
    
    return (EXIT_SUCCESS);
}

int write(int handle,  void *buffer, unsigned int  len){


    uint16_t i=0;
    
    switch(handle){
        default:
            for (i = 0; i < len; i++) 
                UART_dsPIC_putC(*(char*)buffer++);  
    
    }
    return(len);
}