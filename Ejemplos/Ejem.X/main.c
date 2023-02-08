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
#include "Uart.h"

char caracter = 'a';
uint16_t entero = 55;
double p = 3.1416;
char ss[] = "hola mundo";

int main(int argc, char** argv) {

    UART_dsPIC_Init(9600);
    for(;;){

        //UART_dsPIC_putS("Hola Bienvenidos\n\r");
        //UART_dsPIC_putS("dsPIC umaker\n\r\n\r");
        
        printf("El caracter es :%c\n\r",caracter);
        printf("El entero es :%u\n\r",entero);
        printf("El flotante es :%0.3f\n\r",p);
        printf("El string es :%s\n\r\n\r",ss);
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