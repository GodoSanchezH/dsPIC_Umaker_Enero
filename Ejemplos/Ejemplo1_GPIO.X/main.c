/* 
 * File:   main.c
 * Author: HP
 *
 * Created on 26 de enero de 2023, 08:54 PM
 */

#include <stdio.h>//c
#include <stdlib.h>//c
#include <stdint.h>// con signo y sin signo uint 
#include "Config.h"// nuestras macros de clock y los fusibles
#include <libpic30.h>//la funcion delay
#include <xc.h>//las estructuras de los registros


void Ejemplo1(void);

int main(int argc, char** argv) {

    //Establecemos los pines Anx como Digitales
    AD1PCFGL |= (1<<2) | (1<<3)| (1<<4);
    
    
    //definir como salida al RB0,RB1,RB2,RB10
    //0-> SALIDA  
    //1-> ENTRADA 
    TRISB &= ~(1<<0) &  ~(1<<1) & ~(1<<2) & ~(1<<10);
    
    /*TRISBbits.TRISB0=0;
    TRISBbits.TRISB1=0;
    TRISBbits.TRISB2=0;
    TRISBbits.TRISB10=0;*/
    
    
    //Entradas digitales rb7 rb14 rb15
    TRISB |= (1<<4) |(1<<14)|(1<<15);
    
    //TRISBbits.TRISB4=1;
    //TRISBbits.TRISB14=1;
    //TRISBbits.TRISB15=1;
    
    //activar la resistencia pull up
    CNPU1bits.CN1PUE = 1;
    for(;;){
    
        //pull down
        if ((PORTB & (1<<15))) {

            LATB |= (1<<0)|(1<<10);
            LATB &= ~(1<<1) &~(1<<2);
            
        }
        /*
        if (PORTBbits.RB15 == 1) {
             LATBbits.LATB0 = 1;
             LATBbits.LATB10 = 1;
             LATBbits.LATB1 = 0;
             LATBbits.LATB2 = 0;
        }*/

        
        
        //PULL UP
        else if (!(PORTB & (1<<14))) {
            
            LATB |= (1<<1)|(1<<2);
            LATB &= ~(1<<10) &~(1<<0);
        }
        
        if (!(PORTB & (1<<4))) {
        LATB |= (1<<1)|(1<<2)|(1<<10)|(1<<0);
        }

        
        /*
        else if (PORTBbits.RB14 != 1) {
             LATBbits.LATB0 = 0;
             LATBbits.LATB10 = 0;
             LATBbits.LATB1 = 1;
             LATBbits.LATB2 = 1;
        }*/

        
        
        else LATB &= ~(1<<1) &~(1<<2) & ~(1<<10) &~(1<<0);
        


        
       
    }
    
    
    return (EXIT_SUCCESS);
}

void Ejemplo1(void){
    
 LATB ^= ((1<<0)|(1<<10) |(1<<1) |(1<<2));
        __delay_ms(500);
    
 //LATBbits.LATB0 = 1;       
 //LATBbits.LATB1 = 1;  
 //LATBbits.LATB2 = 1;  
 //LATBbits.LATB10 = 1;  
 //__delay_ms(500);
 //LATBbits.LATB0 = 0;       
 //LATBbits.LATB1 = 0;  
 //LATBbits.LATB2 = 0;  
 //LATBbits.LATB10 = 0;  
 //__delay_ms(500);
}


