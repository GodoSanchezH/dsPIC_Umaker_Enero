
#include "Config.h"
#include "Uart.h"
#include <xc.h>
#include <stdio.h>

void UART_dsPIC_Init(uint32_t Baudios){
//configurar los pines TX - RX
//TX RB8
    TRISB &= ~(1<<8);
    RPOR4bits.RP8R = 0B00011;//TX
//RX RB9
    TRISB |= (1<<9);
    RPINR18bits.U1RXR = 9;//RX
    
 //CONFIG UART
    
 // APAMOS LA UART
    U1MODEbits.UARTEN =0;

 //stobit 1 bit
    U1MODEbits.STSEL =0;
//paridad y data bits
    U1MODEbits.PDSEL=0b00;
 // 16 clock
    U1MODEbits.BRGH = 0;
 //calculo los baudios
    U1BRG = (uint16_t)((FCY/16/Baudios)-1);
 
 //habilitamos la tx
    U1STAbits.UTXEN = 1;
    
 //habilitamos la rx
    U1STAbits.URXISEL = 0b00;
    
 //interrupcion
    IEC0bits.U1RXIE = 1;
    IFS0bits.U1RXIF = 0;
    IPC2bits.U1RXIP = 7;
    
    
    // Encendemos LA UART
    U1MODEbits.UARTEN =1;   
    
}
void UART_dsPIC_putC(char c){

    while(U1STAbits.UTXBF ==1);
    U1TXREG = c;
}
void UART_dsPIC_putS(char *s){

    while(*s)
        UART_dsPIC_putC(*s++);
}
