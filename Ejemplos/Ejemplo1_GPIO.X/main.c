/* 
 * File:   main.c
 * Author: HP
 *
 * Created on 26 de enero de 2023, 08:54 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Config.h"
#include <libpic30.h>
#include <p33FJ32MC202.h>


int main(int argc, char** argv) {

    //Establecemos los pines Anx como Digitales
    AD1PCFGL |= (1<<2) | (1<<3)| (1<<4);
    
    
    //definir como salida al RB0,RB1,RB2,RB10
    //0-> SALIDA  
    //1-> ENTRADA 
    TRISB &= ~(1<<0) &  ~(1<<1) & ~(1<<2) & ~(1<<10);
     
    
    for(;;){
    
        
        LATB ^= ((1<<0) |(1<<1) |(1<<2)|(1<<10));
        __delay_ms(500);
    
    }
    
    
    return (EXIT_SUCCESS);
}

